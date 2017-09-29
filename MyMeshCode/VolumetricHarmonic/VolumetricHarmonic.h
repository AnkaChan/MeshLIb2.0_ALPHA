#pragma once
#include <MeshLib\core\TetMesh\TMeshLibHeaders.h>
#include <cmath>
#include <MeshLib\toolbox\DebugSetting.h>
#define STRING_EN(pE) (pE->k*pow((TIf::EdgeVertex1(pE)->position()-TIf::EdgeVertex2(pE)->position()).norm(),2))

namespace MeshLib {
	namespace TMeshLib {
		struct _vertexVHarmonic {
			CPoint newPos;
		};
		class CVertexVHarmonic : public CVertex,public _vertexVHarmonic {};

		struct _edgeVHarmonic {
			double k = 0;
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
			void setInitialMapOnBoundary(TMeshType* pInitalMapTMesh);
			void adjustVertices();
			void adjustVerticesWithBoundarys();
			void adjustVerticesBoundaryHarmonic();
			bool dynamicStep = false;
			void setEpison(double newEpison) {
				epison = newEpison;
			};
			void setStep(double newStep) {
				step = newStep;
			};
		private:
			double totalEnergy();
			double TiangleEdgeWeight(E* pE);
			double HalfEdgeStringConstraint(HE * pHE);
			double edgeWeight(E* pE);
			TMeshType * pTMesh;
			double epison = 0.000001;
			double step = 0.001;
			double dynamicStepSize = 50;
		};

		template <typename TIf>
		class VolumetricHarmonic : public VolumetricHarmonicCore<typename TIf::TVType, typename TIf::VType, typename TIf::HEType,
			typename TIf::TEType, typename TIf::EType, typename TIf::HFType, typename TIf::FType, typename TIf::TType>
		{};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setpTMesh(TMeshType * new_pTMesh)
		{
			pTMesh = new_pTMesh;
			//pTMesh->prepare_for_mp();
			/*for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
				pV->newPos = pV->position();
			}*/
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateEdgeWeights()
		{
			for (TIf::EPtr pE : TIt::TM_EIterator(pTMesh)) {
				if (pE->boundary()) {
					pE->k = TiangleEdgeWeight(pE); 
					//pE->k = edgeWeight(pE);
				}
				else {
					//pE->k = edgeWeight(pE);
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
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setInitialMapOnBoundary(TMeshType * pInitalMapTMesh)
		{
			for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
				if (pV->boundary()) {
					TIf::VPtr pVImage = pInitalMapTMesh->idVertex(pV->id());
					pV->position() = pVImage->position();
				}
				else {
					pV->position() = CPoint(0, 0, 0);
				}
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
						CPoint tangentComponent = d - (P * d) * P;
						pV->newPos = P - step * tangentComponent;
						newCenter += pV->newPos;
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
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::adjustVerticesWithBoundarys()
		{
			double originalEnergy = 0;
			double newEnergy = totalEnergy();
			int numV = pTMesh->vertices().size();

			while (abs(newEnergy - originalEnergy) > epison)
			{
				CPoint newCenter(0, 0, 0);
				for (auto pV : TIt::TM_VIterator(pTMesh)) {
					CPoint nP;
					CPoint& P = pV->position();
					double totalK = 0;
					for (auto pNV : TIt::V_VIterator(pV)) {
						TIf::EPtr pEV_NV = TIf::VertexEdge(pV, pNV);
						nP += pNV->position()*pEV_NV->k;
						totalK += pEV_NV->k;
					}
					CPoint d = P - nP / totalK;
					CPoint tangentComponent = d - (P * d) * P;
					pV->newPos = P - step * tangentComponent;
					newCenter += pV->newPos;

				}
				newCenter /= numV;
				for (auto pV : TIt::TM_VIterator(pTMesh)) {
					CPoint& P = pV->position();
					P = pV->newPos - newCenter;
					P /= P.norm();
				}
				originalEnergy = newEnergy;
				newEnergy = totalEnergy();
				cout << "New Energy: " << newEnergy << ". ERR: " << newEnergy - originalEnergy << ".\n";
			}
			cout << "Algorithm Converged.\n";
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::adjustVerticesBoundaryHarmonic()
		{
			double originalEnergy = 0;
			double newEnergy = totalEnergy();
			int numV = pTMesh->vertices().size();
			double minErr = 0.1;
			int count = 0;

			while (abs(newEnergy - originalEnergy) > epison)
			{
				CPoint newCenter(0, 0, 0);
				for (auto pV : TIt::TM_VIterator(pTMesh)) {
					CPoint nP;
					CPoint& P = pV->position();
					double totalK = 0;
					if (pV->boundary()) {
						ShowInDebug(std::vector<double> weights;);
						for (auto pNV : TIt::V_VIterator(pV)) {
							TIf::EPtr pEV_NV = TIf::VertexEdge(pV, pNV);
							nP += pNV->position()*pEV_NV->k;
							ShowInDebug(weights.push_back(pEV_NV->k););
							totalK += pEV_NV->k;
						}
						CPoint d = P - nP / totalK;
						CPoint tangentComponent = d - (P * d) * P;
						pV->newPos = P - step * tangentComponent;
						newCenter += pV->newPos;
						
						//pV->newPos = P - step * d;
						
					}
					else {
						cout << "This adjustVerticesBoundaryHarmonic() method only works for Tmesh whose vertices are all on boundary." << endl;
						return;
					}
				}
				newCenter /= numV;

				for (auto pV : TIt::TM_VIterator(pTMesh)) {
					CPoint& P = pV->position();
					P = pV->newPos - newCenter;
					P /= P.norm();
				}

				originalEnergy = newEnergy;
				newEnergy = totalEnergy();
				++count;
				if (count >= 1) {
					count = 0;
					std::cout << "New Harmonic Energy: " << newEnergy << ". Step ERR: " << abs(newEnergy - originalEnergy) << "\n";
				}
				if (dynamicStep) {
					if (abs(newEnergy - originalEnergy) < minErr) {
						minErr = abs(newEnergy - originalEnergy);
						if (dynamicStepSize * minErr < step) {
							step = dynamicStepSize * minErr;
						}
					}
				}

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
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::TiangleEdgeWeight(E * pE)
		{
			TIf::HFPtr boundaryHalfFaces[2];
			int i = 0;
			for (auto pF : TIt::E_FIterator(pE)) {
				if (pF->boundary()) {
					TIf::HFPtr pHFBoundary =
						TIf::FaceLeftHalfFace(pF) == NULL ? TIf::FaceRightHalfFace(pF) : TIf::FaceLeftHalfFace(pF);
					boundaryHalfFaces[i] = pHFBoundary;
					++i;
					if (i == 2) {
						break;
					}
				}
			}
			TIf::HEPtr pHE0, pHE1;
			for (TIf::HEPtr pHE : TIt::HF_HEIterator(boundaryHalfFaces[0])) {
				if (TIf::HalfEdgeEdge(pHE) == pE) {
					pHE0 = pHE;
				}
			}
			for (TIf::HEPtr pHE : TIt::HF_HEIterator(boundaryHalfFaces[1])) {
				if (TIf::HalfEdgeEdge(pHE) == pE) {
					pHE1 = pHE;
				}
			}

			return HalfEdgeStringConstraint(pHE0) + HalfEdgeStringConstraint(pHE1);
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::HalfEdgeStringConstraint(HE * pHE)
		{

			CPoint v0 = TIf::HalfEdgeVec(TIf::HalfEdgeNext(pHE));
			CPoint v1 = TIf::HalfEdgeVec(TIf::HalfEdgePrev(pHE));

			return -(v0*v1) / (v0^v1).norm();
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