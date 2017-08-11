#pragma once
#include <MeshLib/algorithm/Shelling/TetSheller.h> 
#include <MeshLib/core/TetMesh/TInterface.h>
#include <MeshLib/core/TetMesh/titerators2.h>
#include <MeshLib/core/Geometry/Circumsphere.h>
#include <MeshLib/core/TetMesh/BaryCoordinates/BaryCoordinates3D.h>
#include "SphericalDistribution.h"
#include "TriangularDistribution.h"

#include <assert.h>
#include <memory>
#include <list>
#include <random>
#include <cmath>

#define PI 3.1415926535897932

namespace MeshLib {
	namespace TMeshLib {
		class _faceParameterization {
		public:
			bool isBoundaryInPara = false;
		};

		class CFaceD3Parameterization : public CFace, public _faceParameterization {};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class D3ParameterizationCore {
		public:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef TIterators<TIf> TIt;
			D3ParameterizationCore(typename TIf::TMeshPtr pTMesh, std::shared_ptr<std::list<T *>> pShellingList):
				m_pTMesh(pTMesh),
				m_pShellingList(pShellingList),
				m_circumSphere(m_pShellingList->front())
			{
				m_TIter = m_pShellingList->begin();
				++m_TIter;

				//normalizeTMesh2CircumSphere();

				CPoint v[4] = { CPoint( 1, 0, -1.0f / sqrt(2)), 
								CPoint(-1, 0, -1.0f / sqrt(2)),
								CPoint(0, -1,  1.0f / sqrt(2)),
								CPoint(0,  1,  1.0f / sqrt(2)) 
				};
				int i = 0;
				for (V * pV : TIt::T_VIterator(m_pShellingList->front())) {
					pV->position() = v[i] / v[i].norm();
					++i;
				}

				for (HF * pHF : TIt::T_HFIterator(m_pShellingList->front())) {
					makeBoundary(pHF);
				}
			}

			/* return true if succeeded */
			bool mapNextTetToSphereRand();

			CTetCircumSphere getCircumSphere() { return m_circumSphere; };
		private:
			std::shared_ptr<std::list<T *>> m_pShellingList;
			std::set<HF *> boundaryFacesSet;
			typename TIf::TMeshPtr m_pTMesh;
			CTetCircumSphere m_circumSphere;
			typename std::list<T *>::iterator m_TIter;

			void normalizeTMesh2CircumSphere();

			int countIntersectionFace(T * pNextT);
			void map1Point(T * pNextT);
			void map2Faces(T * pNextT);
			void map3Faces(T * pNextT);

			CPoint pickPointRand(HF * pHF); 
			CPoint pickCentricPoint(HF * pHF);
			HF* findOverlapHF(T * pT);
			bool checkVisibility(CPoint Image, HF * pBottomHF);

			void makeBoundary(HF * pHF);
			void removerFromBoundary(HF * pHF);
			bool isBoundary(HF * pHF);
			double directedVolume(CPoint Image, HF * pHF);

		};
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::mapNextTetToSphereRand()
		{

			if (m_TIter == m_pShellingList->end()) {
				return false;
			}

			T * pNextT = *m_TIter;

			int n = countIntersectionFace(pNextT);
			switch (n)
			{
			case 1:
				map1Point(pNextT);
				break;
			case 2:
				map2Faces(pNextT);
				break;
			case 3:
				map3Faces(pNextT);
				break;
			default:
				break;
			}
			++m_TIter;
			return true;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::normalizeTMesh2CircumSphere()
		{
			for (TIf::VPtr pV : TIt::TM_VIterator(m_pTMesh)) {
				CPoint &p = pV->position();
				p = (p - m_circumSphere.getCenter()) / m_circumSphere.getRaduis();
			}
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline int D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::countIntersectionFace(T * pNext)
		{
			int n = 0;
			for (auto pHF : TIt::T_HFIterator(pNext)) {
				if (isBoundary(pHF))
					++n;
			}
			return n;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::map1Point(T * pNextT)
		{

			HF * pHF = findOverlapHF(pNextT);
			CPoint pImage;
			
			/*do {
				pImage = pickPointRand(pHF);
			} while (!checkVisibility(pImage, TIf::HalfFaceDual(pHF)));
			*/
			pImage = pickCentricPoint(pHF);

			V * pV = TIf::TVertexVertex(TIf::HalfFaceOppositeTVertex(pHF));
			pV->position() = pImage;
			removerFromBoundary(pHF);

			for (HF * pHFNewBound : TIt::T_HFIterator(*m_TIter)) {
				if (pHFNewBound != pHF){
					makeBoundary(pHFNewBound);
				}
			}
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::map2Faces(T * pNextT)
		{
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::map3Faces(T * pNextT)
		{
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		CPoint D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::pickPointRand(HF * pHF)
		{
			static CTriangularDistribution triDst;

			CBaryCoordinates3D baryCoords(pHF);
			CPoint baryRandPoint = triDst.getUnif();
			CPoint direction = baryCoords.bary2Decartes(baryRandPoint);
			return direction / direction.norm();
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline CPoint D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::pickCentricPoint(HF * pHF)
		{
			CBaryCoordinates3D baryCoords(pHF);
			CPoint direction = baryCoords.bary2Decartes(CPoint(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f));
			return direction / direction.norm();
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline HF* D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::findOverlapHF(T * pT)
		{
			for (auto pHF : TIt::T_HFIterator(pT)) {
				if (isBoundary(pHF))
					return pHF;
			}

			/* Suppose never gets here. */
			return NULL;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::checkVisibility(CPoint Image, HF * pBottomHF)
		{
			for (HF * pHF : boundaryFacesSet) {
				if (pBottomHF == pHF && directedVolume(Image, pHF) > 0) {
					return false;
				}
				else if (pBottomHF != pHF && directedVolume(Image, pHF) < 0) {
					return false;
				}
			}
			return true;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::makeBoundary(HF * pHF)
		{
			boundaryFacesSet.insert(pHF);
			F * pF = TIf::HalfFaceFace(pHF);
			pF->isBoundaryInPara = true;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::removerFromBoundary(HF * pHF)
		{
			boundaryFacesSet.erase(pHF);
			F * pF = TIf::HalfFaceFace(pHF);
			pF->isBoundaryInPara = false;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::isBoundary(HF * pHF)
		{
			return TIf::HalfFaceFace(pHF)->isBoundaryInPara;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::directedVolume(CPoint Image, HF * pHF)
		{
			CPoint v[4];
			v[0] = Image;
			int i = 1;
			for (V * pV : TIt::HF_VIterator(pHF)) {
				v[i] = pV->position();
				++i;
			}
			CPoint AB = v[1] - v[0];
			CPoint AC = v[2] - v[0];
			CPoint AD = v[3] - v[0];

			return  AB * (AC ^ AD);
		}

		template <typename TIf>
		class D3Parameterization : public D3ParameterizationCore<typename TIf::TVType, typename TIf::VType, typename TIf::HEType,
			typename TIf::TEType, typename TIf::EType, typename TIf::HFType, typename TIf::FType, typename TIf::TType> 
		{
		public:
			D3Parameterization(typename TIf::TMeshPtr pTMesh, std::shared_ptr<std::list<typename TIf::TPtr>> pShellingList) : D3ParameterizationCore(pTMesh, pShellingList) {};
		};
	}
}