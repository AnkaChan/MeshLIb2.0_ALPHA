#pragma once
#include "MeshLib\core\TetMesh\TMeshLibHeaders.h"
#include <cmath>
#define STRING_EN(pE) (pE->k*pow((TIf::EdgeVertex1(pE)->position()-TIf::EdgeVertex2(pE)->position()).norm(),2))

namespace MeshLib {
	namespace TMeshLib {
		struct _vertexVHarmonic {
			CPoint newPos;
		};
		class CVertexVHarmonic : public CVertex,public _vertexVHarmonic {};

		struct _edgeVHarmonic {
			double k;
		};
		class CEdgeVHarmonic : public CEdge, public _edgeVHarmonic {};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class VolumetricHarmonicCore {
		private:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef CTMesh< TV, V, HE, TE, E, HF, F, T > TMeshType;
			typedef TIterators<TIf> TIt;
		public:
			VolumetricHarmonicCore() {};
			void setpTMesh(TMeshType * new_pTMesh);
			void calculateEdgeWeights();
			void setInitialMap(TMeshType* pInitalMapTMesh);
			void adjustVertices();
			bool dynamicStep = false;
			void setEpison(double newEpison) {
				epison = newEpison;
			};
			void setStep(double newStep) {
				step = newStep;
			};
		private:
			double totalEnergy();
			double edgeWeight(E* pE);
			TMeshType * pTMesh;
			double epison;
			double step;
		};

		template <typename TIf>
		class VolumetricHarmonic : public VolumetricHarmonicCore<typename TIf::TVType, typename TIf::VType, typename TIf::HEType,
			typename TIf::TEType, typename TIf::EType, typename TIf::HFType, typename TIf::FType, typename TIf::TType>
		{};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setpTMesh(TMeshType * new_pTMesh)
		{
			pTMesh = new_pTMesh;
			/*for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
				pV->newPos = pV->position();
			}*/
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateEdgeWeights()
		{
			for (TIf::EPtr pE : TIt::TM_EIterator(pTMesh)) {
				if (pE->boundary()) {
					continue;
				}
				else {
					pE->k = edgeWeight(pE);
				}

			}
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setInitialMap(TMeshType * pInitialMapTMesh)
		{
			for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
				TIf::VPtr pVImage = pInitialMapTMesh->idVertex(pV->id());
				pV->position() = pVImage->position();
			}
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::adjustVertices()
		{
			double originalEnergy = 0;
			double newEnergy = totalEnergy();
			
			while (abs(newEnergy - originalEnergy) > epison)
			{
				for (auto pV : TIt::TM_VIterator(pTMesh)) {
					CPoint nP;
					CPoint& P = pV->position();
					double totalK = 0;
					if (!pV->boundary()) {
						for (auto pNV : TIt::V_VIterator(pV)) {
							TIf::EPtr pEV_NV = TIf::VertexEdge(pV, pNV);
							nP += pNV->position()*pEV_NV->k;
							totalK += pEV_NV->k;
						}
						CPoint d = P - nP / totalK;
						pV->newPos = P - step * d;
					}
				}
				for (auto pV : TIt::TM_VIterator(pTMesh)) {
					if (!pV->boundary()) {
						pV->position() = pV->newPos;
					}
				}
				originalEnergy = newEnergy;
				newEnergy = totalEnergy();
				cout << "New Energy: " << newEnergy << ". ERR: " << newEnergy - originalEnergy << ".\n";
			}
			cout << "Algorithm Converged.\n";
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::totalEnergy()
		{
			double energy = 0;
			for (TIf::EPtr pE : TIt::TM_EIterator(pTMesh)) {
				energy += STRING_EN(pE);
			}
			return energy;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::edgeWeight(E * pE)
		{
			double w = 0;
			for (TIf::TEPtr pTE : TIt::E_TEIterator(pE)) {
				TIf::HFPtr pHFL = TIf::TEdgeLeftHalfFace(pTE);
				TIf::HFPtr pHFR = TIf::TEdgeRightHalfFace(pTE);

				CPoint nL = TIf::HalfFaceNormal(pHFL);
				CPoint nR = TIf::HalfFaceNormal(pHFR);
				nR = -nR;

				double cosTheta = nL * nR;
				double sinTheta = (nL ^ nR).norm();

				TIf::TEPtr pTEOpposite = TIf::TEdgeDualTEdge(pTE);
				w += TIf::EdgeLength(TIf::TEdgeEdge(pTEOpposite)) * (cosTheta / sinTheta);
			}
			return w;
		}
	}
}