#pragma once
#include <MeshLib/algorithm/Shelling/TetSheller.h> 
#include <MeshLib/core/TetMesh/TInterface.h>
#include <MeshLib/core/TetMesh/titerators2.h>
#include <MeshLib/core/Geometry/Circumsphere.h>
#include <MeshLib/core/TetMesh/BaryCoordinates/BaryCoordinates3D.h>
#include "SphericalDistribution.h"
#include "TriangularDistribution.h"
#include <memory>
#include <list>
#include <random>

#define PI 3.1415926535897932

namespace MeshLib {
	namespace TMeshLib {
		class _faceParameterization {
		public:
			bool isBoundaryInPara = false;
		};


		template< typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class D3Parameterization {
		public:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef TIterators<TV, V, HE, TE, E, HF, F, T> TIt;
			D3Parameterization(TIf::TMeshPtr pTMesh, std::shared_ptr<std::list<TIf::TPtr>> pShellingList):
				m_pTMesh(pTMesh),
				m_pShellingList(pShellingList),
				m_circumSphere(m_pShellingList->front())
			{
				m_TIter = m_pShellingList->begin();
				++m_TIter;

				normalizeTMesh2CircumSphere();
			}

			/* return true if succeeded */
			bool mapNextTetToSphereRand();
		private:
			std::shared_ptr<std::list<TIf::TPtr>> m_pShellingList;
			std::set<TIf::HFPtr> boundaryFacesSet;
			TIf::TMeshPtr m_pTMesh;
			CTetCircumSphere m_circumSphere;
			typename std::list<TIf::TPtr>::iterator m_TIter;

			void normalizeTMesh2CircumSphere();

			void map1Point(TIf::TPtr pNextT);
			void map2Faces(TIf::TPtr pNextT);
			void map3Faces(TIf::TPtr pNextT);

			CPoint pickPointRand(TIf::HFPtr pHF); 
			bool checkVisibility(CPoint Image);

		};
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool D3Parameterization<TV, V, HE, TE, E, HF, F, T>::mapNextTetToSphereRand()
		{

			if (m_TIter == m_pShellingList->end()) {
				return false;
			}

			TIf::TPtr pNextT = *m_TIter;

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
			return true;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void D3Parameterization<TV, V, HE, TE, E, HF, F, T>::normalizeTMesh2CircumSphere()
		{
			for (TIf::VPtr pV : TIt::TM_VIterator(m_TMesh)) {
				CPoint &p = pV->position();
				p = (p - m_circumSphere.getCenter()) / m_circumSphere.getRaduis();
			}
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void D3Parameterization<TV, V, HE, TE, E, HF, F, T>::map1Point(TIf::TPtr pNextT)
		{

			TIf::HFPtr pHF = findOverlapHF(pNextT);

			do {
				CPoint pImage = pickPointRand(pHF);
				
			} while (checkVisibility(pImage));
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		CPoint D3Parameterization<TV, V, HE, TE, E, HF, F, T>::pickPointRand(TIf::HFPtr pHF)
		{
			static CTriangularDistribution triDst;

			CBaryCoordinates3D baryCoords(pHF);
			CPoint baryRandPoint = triDst.getUnif();
			CPoint direction = baryCoords.bary2Decartes(baryRandPoint);
			return direction / direction.norm();
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool D3Parameterization<TV, V, HE, TE, E, HF, F, T>::checkVisibility(CPoint Image)
		{


			return false;
		}
	}
}