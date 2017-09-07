#pragma once
#include <MeshLib/algorithm/Shelling/TetSheller.h> 
#include <MeshLib/core/TetMesh/TInterface.h>
#include <MeshLib/core/TetMesh/titerators2.h>
#include <MeshLib/core/Geometry/Circumsphere.h>
#include <MeshLib/core/TetMesh/BaryCoordinates/BaryCoordinates3D.h>
#include <MeshLib/toolbox/DebugSetting.h>
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
		class _tetParameterization {
		public:
			bool mapped;
		};
		class CFaceD3Parameterization : public CFace, public _faceParameterization {};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class D3ParameterizationCore {
		public:
			D3ParameterizationCore(CTMesh< TV, V, HE, TE, E, HF, F, T > * pTMesh, std::shared_ptr<std::list<T *>> pShellingList):
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

				setMinAngleDegree(minAngle);
			}

			/* return true if succeeded */
			bool mapNextTetToSphereRand();
			double setMinAngleDegree(double newMinAngle);

			CTetCircumSphere getCircumSphere() { return m_circumSphere; };
			const std::set<HF *> & getBoundaryHFSet() {return boundaryFacesSet; };
		private:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef TIterators<TIf> TIt;

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
			bool findOverlapHFs(T * pT, int numHFs, HF** HFArray);
			bool checkVisibility(CPoint Image, HF * pBottomHF);

			void makeBoundary(HF * pHF);
			void removerFromBoundary(HF * pHF);
			bool isBoundary(HF * pHF);
			double directedVolume(CPoint Image, HF * pHF);
			double orientedVolume(T* pT);
			bool adjustVertices(T* pT, HF** overlapedHFs);
			bool isAvaliablePosition(CPoint newA, TV* pTV);
			void splitEdge(T* pNextT, TE* pTE);

			double step = 0.05;
			double minAngle = 10;
			int maxIteration = 100;
			double maxCosAngle;
			double getMaxCosAngle(T * pT);
			CPoint halfFaceNormal(HF * pHF);
		};
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::mapNextTetToSphereRand()
		{

			if (m_TIter == m_pShellingList->end()) {
				return false;
			}

			T * pNextT = *m_TIter;

			int n = countIntersectionFace(pNextT);
			cout << "Tet " << pNextT->id() << ", has " << n << " insert faces.\n";

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
			pNextT->mapped = true;
			++m_TIter;
			return true;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::setMinAngleDegree(double newMinAngle)
		{
			minAngle = PI * newMinAngle / 180.0;
			maxCosAngle = abs(cos(minAngle));
			return maxCosAngle;
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
			removerFromBoundary(TIf::HalfFaceDual( pHF ));

			for (HF * pHFNewBound : TIt::T_HFIterator(*m_TIter)) {
				if (pHFNewBound != pHF){
					makeBoundary(pHFNewBound);
				}
			}
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::map2Faces(T * pNextT)
		{
			double oV = orientedVolume(pNextT);
			cout << "Oriented volume fot this Tet: " << oV <<"\n";
			HF* OverlapedHFs[2];
			bool succeed = findOverlapHFs(pNextT, 2, OverlapedHFs);
			// If truely found two overlaped halffaces
			assert(succeed);
			//cout << "Find 2 overlap faces: " << OverlapedHFs[0] << ", " << OverlapedHFs[1] << "\n";
			if (oV > 0) {
				removerFromBoundary(TIf::HalfFaceDual( OverlapedHFs[0]) );
				removerFromBoundary(TIf::HalfFaceDual( OverlapedHFs[1]) );

				for (HF * pHFNewBound : TIt::T_HFIterator(pNextT)) {
					if (pHFNewBound != OverlapedHFs[0] && pHFNewBound != OverlapedHFs[1]) {
						makeBoundary(pHFNewBound);
					}
				}
				return;
			}

			if (adjustVertices(pNextT, OverlapedHFs)) {
				removerFromBoundary(TIf::HalfFaceDual(OverlapedHFs[0]));
				removerFromBoundary(TIf::HalfFaceDual(OverlapedHFs[1]));

				for (HF * pHFNewBound : TIt::T_HFIterator(pNextT)) {
					if (pHFNewBound != OverlapedHFs[0] && pHFNewBound != OverlapedHFs[1]) {
						makeBoundary(pHFNewBound);
					}
				}
			}
			else {
				TE* pTE;
				splitEdge(pNextT, pTE);
			}

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
				// default isBoundary value is false. 
				if (isBoundary(pHF))
					return pHF;
			}

			/* Suppose never gets here. */
			return NULL;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::findOverlapHFs(T * pT, int numHFs, HF** HFArray)
		{
			int i = 0;
			for (auto pHF : TIt::T_HFIterator(pT)) {
				if (isBoundary(pHF)){
					HFArray[i] = pHF;
					++i;
				}
				if (i > numHFs) {
					return false;
				}
			}
			if (i != numHFs) {
				return false;
			}
			return true;	
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

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::orientedVolume(T * pT)
		{
			CPoint A = pT->vertex(0)->position();
			CPoint B = pT->vertex(1)->position();
			CPoint C = pT->vertex(2)->position();
			CPoint D = pT->vertex(3)->position();
			CPoint AB = B - A;
			CPoint AC = C - A;
			CPoint AD = D - A;

			double orientation_product = AB * (AC ^ AD);
			return orientation_product;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::adjustVertices(T * pT, HF ** overlapedHFs)
		{
			CPoint d[4];
			CPoint pointsBeforeAdjusting[4];
			pointsBeforeAdjusting[0] = pT->vertex(0)->position();
			pointsBeforeAdjusting[1] = pT->vertex(1)->position();
			pointsBeforeAdjusting[2] = pT->vertex(2)->position();
			pointsBeforeAdjusting[3] = pT->vertex(3)->position();

			//calculate step weight
			std::map<TV*, double> edgeWeightsMap;
			HF *pHF0 = overlapedHFs[0];
			HF *pHF1 = overlapedHFs[1];
			HE * pHEDual = NULL;
			HE * pHE;
			for (auto pHE0 : TIt::HF_HEIterator(pHF0)) {
				for (auto pHE1 : TIt::HF_HEIterator(pHF1)) {
					if (pHE1 == TIf::HalfEdgeDual(pHE0)) {
						pHEDual = pHE1;
						break;
					}
				}
				if (pHEDual != NULL) {
					pHE = pHE0;
					break;
				}
			}
			
			double edgeLength0 = TIf::EdgeLength(TIf::HalfEdgeEdge(pHE));
			edgeWeightsMap.insert(std::map<TV*, double>::value_type(TIf::HalfEdgeTTarget(pHE), edgeLength0));
			edgeWeightsMap.insert(std::map<TV*, double>::value_type(TIf::HalfEdgeTSource(pHE), edgeLength0));
			TV* pTVOut0 = TIf::HalfEdgeTTarget(TIf::HalfEdgeNext(pHE));
			TV* pTVOut1 = TIf::HalfEdgeTTarget(TIf::HalfEdgeNext(pHEDual));
			double edgeLength1 = (TIf::TVertexVertex(pTVOut0)->position() - TIf::TVertexVertex(pTVOut1)->position()).norm();
			edgeWeightsMap.insert(std::map<TV*, double>::value_type(pTVOut0, edgeLength1));
			edgeWeightsMap.insert(std::map<TV*, double>::value_type(pTVOut1, edgeLength1));

			cout << "Adjusting tet " << pT->id() << ".\n";
			cout << "Initial oriented volume: " << orientedVolume(pT) << ".\n";

			int numIterations = 0;
			while (abs(getMaxCosAngle(pT)) > maxCosAngle || orientedVolume(pT) < 0) {
				int k = 0;
				for (auto pTV : TIt::T_TVIterator(pT)) {
					HF*  pHF = TIf::TVertexOppositeHalfFace(pTV);
					CPoint & A = TIf::TVertexVertex(pTV)->position();
					CPoint V[3];
					ShowInDebug(cout << "Adjusting vertex: " << pTV->vert()->id() << "\n";);
					ShowInDebug(cout << "Opposite Halfface: ";);
					int i = 0;
					for (auto pV : TIt::HF_VIterator(pHF)) {
						V[i] = pV->position();
						++i;
						ShowInDebug(cout << pV->id() << ", ";);
					}
					CPoint BA = A - V[0];
					CPoint BC = V[0] - V[1];
					CPoint BD = V[0] - V[2];

					ShowInDebug(cout << "\nOriented volume: " << (BA ^ BC) * BD << "\n";);

					d[k] = CPoint(BC[1] * BD[2] - BD[1] * BC[2], -(BC[0] * BD[2] - BD[0] * BC[2]), BC[0] * BD[1] - BD[0] * BC[1]);
					d[k] = -d[k] / d[k].norm();
					CPoint OA = A;
					d[k] = d[k] - (d[k] * OA)*OA;
					d[k] = d[k] / d[k].norm();
					//cout << "\nOriented volume after: " << AB * (AC ^ AD) << endl;
					ShowInDebug(cout << "Move direction: " << d[k] << endl;);
					++k;

				}
				k = 0;
				int numFixPoints = 0;
				for (auto pTV : TIt::T_TVIterator(pT)) {
					CPoint & A = TIf::TVertexVertex(pTV)->position();
					CPoint newA = A;
					newA += edgeWeightsMap[pTV] * step * d[k];
					newA = newA / newA.norm();
					if (isAvaliablePosition(newA, pTV)) {
						A = newA;
					}
					else {
56						numFixPoints++;
					}
					++k;
				}
				cout << "Number of fixed points: " << numFixPoints << "\n";
				if (numFixPoints == 4 || numIterations > maxIteration) {
					if (numFixPoints == 4) {
						cout << "All the 4 points are not movable.\n";
					}
					else {
						cout << "Over max Iteration.\n";
					}
					if (orientedVolume(pT) > 0) {
						cout << "Oriented Volume stops at " << orientedVolume(pT) << ".\n";
						return true;
					}
					else
					{
						cout << "*****************************************************************\n"
							<< "ATTENTION:\n";
						cout << "Oriented Volume stops at negative value, failed.\n";
						cout << "*****************************************************************\n";
						return false;
					}
				}
				cout << "New oriented volume: " << orientedVolume(pT) << ".\n";
				cout << "New max cos angle: " << abs(getMaxCosAngle(pT)) << ", target is less than: " << maxCosAngle << ".\n";
				++numIterations;
			}
			return true;
			
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::isAvaliablePosition(CPoint newA, TV * pTV)
		{
			bool avaliable = true;
			CPoint oA = TIf::TVertexVertex(pTV)->position();
			TIf::TVertexVertex(pTV)->position() = newA;
			V* pV = TIf::TVertexVertex(pTV);
			for (auto pTVTet : TIt::V_TVIterator(pV)) {
				T* pT = TIf::TVertexTet(pTVTet);
				double maxCosAngleThisTet = getMaxCosAngle(pT);
				if (pT->mapped && abs(maxCosAngleThisTet) > abs(maxCosAngle)) {
					avaliable = false;
					break;
				}
				double orientedVolumeThisTet = orientedVolume(pT);
				if (pT->mapped && orientedVolumeThisTet < 0) {
					avaliable = false;
					break;
				}
			}

			TIf::TVertexVertex(pTV)->position() = oA;
			return avaliable;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::splitEdge(T * pNextT, TE * pTE)
		{
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::getMaxCosAngle(T * pT)
		{
			double maxCos = 0.0;
			for (auto pTE : TIt::T_TEIterator(pT)) {
				HE* pHEL = TIf::TEdgeLeftHalfEdge(pTE);
				HE* pHER = TIf::TEdgeRightHalfEdge(pTE);

				HF* pHFL = TIf::HalfEdgeHalfFace(pHEL);
				HF* pHFR = TIf::HalfEdgeHalfFace(pHER);

				CPoint nL = halfFaceNormal(pHFL);
				CPoint nR = halfFaceNormal(pHFR);

				if (abs(maxCos) < abs(nL*nR)) {
					maxCos = abs(nL*nR);
				}
			}
			return maxCos;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline CPoint D3ParameterizationCore<TV, V, HE, TE, E, HF, F, T>::halfFaceNormal(HF * pHF)
		{
			HE* pHE = TIf::HalfFaceHalfEdge(pHF);
			CPoint  p[3];
			for (int k = 0; k < 3; k++)
			{
				p[k] = pHE->target()->position();
				pHE = TIf::HalfEdgeNext(pHE);
			}
			CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
			return fn / fn.norm();
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