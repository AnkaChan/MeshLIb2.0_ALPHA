#pragma once
#include <MeshLib\core\TetMesh\TMeshLibHeaders.h>
#include <cmath>
#include <MeshLib\toolbox\DebugSetting.h>
#include <omp.h>
#define STRING_EN(pE) (pE->k*(TIf::EdgeVertex1(pE)->position()-TIf::EdgeVertex2(pE)->position()).norm2())
#define NUM_PRINTING 100
#define LINE_SEARCH_C1 0.01
#define LINE_SEARCH_RATIO 0.5
#define MIN_STEP 0.00001

namespace MeshLib {
	namespace TMeshLib {
		struct _vertexVHarmonic {
			CPoint newPos;
			double step = 0.5;
			double totalK;
			double localHarmonicEnergy = 0;
			
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
			void calculateEdgeWeightsBoundaryHarmonic();
			void setInitialMap(TMeshType* pInitalMapTMesh);
			void setInitialMapOnBoundary(TMeshType* pInitalMapTMesh);
			void adjustVertices();
			void adjustVerticesWithBoundarys();
			void adjustVerticesBoundaryHarmonic();
			void adjustVerticesBoundaryHarmonic_parallel();
			bool dynamicStep = false;
			void setEpison(double newEpison) {
				epison = newEpison;
			};
			void setStep(double newStep) {
				step = newStep;
			};
			void setStopGradientThreshold(double newGradientThreshold);
			double dynamicStepSize = 50;
			void lineSearchStep(V* pV, CPoint grad);
			enum ConvergeCondition { minError, maxGradient };
			ConvergeCondition convergeCondition = maxGradient;
			bool enableLineSearch = true;
		private:
			double totalEnergy();
			double TiangleEdgeWeight(E* pE);
			double HalfEdgeStringConstraint(HE * pHE);
			double edgeWeight(E* pE);
			void calculateTotalKForEachV();
			void calculateLocalHEnergyForEachV();
			double calculateVertexLocalHarmonicEnergy(V* pV);
			double calculateVertexNewPosLocalHarmonicEnergy(V* pV);
			TMeshType * pTMesh;
			double epison = 0.000001;
			double step = 0.001;
			double gradientThreshold = 0.000007;
			bool hasConverged();
			int numV = 0;
			double minErr = 1000000000.0f;
			double maxGradientNorm = 0;
			double printInformation();
			std::list<T *> negativeTetsList;
		};

		template <typename TIf>
		class VolumetricHarmonic : public VolumetricHarmonicCore<typename TIf::TVType, typename TIf::VType, typename TIf::HEType,
			typename TIf::TEType, typename TIf::EType, typename TIf::HFType, typename TIf::FType, typename TIf::TType>
		{};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setpTMesh(TMeshType * new_pTMesh)
		{
			pTMesh = new_pTMesh;
			numV = pTMesh->vertices().size();
			pTMesh->prepare_for_mp();
			/*for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
				pV->newPos = pV->position();
			}*/
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateEdgeWeights()
		{
			for (TIf::EPtr pE : TIt::TM_EIterator(pTMesh)) {
				if (pE->boundary()) {
					pE->k = 0;
				}
				else {
					pE->k = edgeWeight(pE);
				}
			}
			calculateTotalKForEachV();
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateEdgeWeightsBoundaryHarmonic()
		{
			for (TIf::EPtr pE : TIt::TM_EIterator(pTMesh)) {
				if (pE->boundary()) {
					pE->k = TiangleEdgeWeight(pE);
				}
				else {
					pE->k = 0;
				}
			}
			calculateTotalKForEachV();
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setInitialMap(TMeshType * pInitialMapTMesh)
		{
			for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
				TIf::VPtr pVImage = pInitialMapTMesh->idVertex(pV->id());
				pV->position() = pVImage->position();
			}
			calculateLocalHEnergyForEachV();
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
			calculateLocalHEnergyForEachV();
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
			do 
			{
				maxGradientNorm = 0;
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
			} while (!hasConverged());
			cout << "Algorithm Converged.\n";
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::adjustVerticesBoundaryHarmonic()
		{
			double originalEnergy = 0;
			double newEnergy = totalEnergy();
			int count = 0;
			do
			{
				maxGradientNorm = 0;
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
				if (count >= NUM_PRINTING) {
					count = 0;
					std::cout << "New Harmonic Energy: " << newEnergy << ". Step ERR: " << (newEnergy - originalEnergy) << "\n";
				}
				if (dynamicStep) {
					if (abs(newEnergy - originalEnergy) < minErr) {
						minErr = abs(newEnergy - originalEnergy);
						if (dynamicStepSize * minErr < step) {
							step = dynamicStepSize * minErr;
						}
					}
				}
				
			} while (!hasConverged());
			cout << "Algorithm Converged.\n";
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::adjustVerticesBoundaryHarmonic_parallel()
		{
			double originalEnergy = 0;
			double newEnergy = totalEnergy();
			int count = 0;
			double newCenter_0, newCenter_1, newCenter_2;

			do
			{
				maxGradientNorm = 0;
				newCenter_0 = 0;
				newCenter_1 = 0; 
				newCenter_2 = 0;

				#pragma omp parallel for reduction(+:newCenter_0, newCenter_1, newCenter_2)
				for (int i = 0; i < numV; ++i) {
					CPoint nP;
					TIf::VPtr pV = pTMesh->vertices_vec[i];
					CPoint& P = pV->position();
					if (pV->boundary()) {
						for (auto pNV : TIt::V_VIterator(pV)) {
							TIf::EPtr pEV_NV = TIf::VertexEdge(pV, pNV);
							nP += pNV->position()*pEV_NV->k;
						}
						CPoint d = P - nP / pV->totalK;
						CPoint tangentComponent = d - (P * d) * P;
						double dNorm = tangentComponent.norm();
						#pragma omp critical  
						{
							if (dNorm > maxGradientNorm) {
								maxGradientNorm = dNorm;
							}
						}
						if (enableLineSearch && (dNorm > gradientThreshold)) {
							lineSearchStep(pV, tangentComponent);
						}
						else {
							pV->newPos = P - step * tangentComponent;
						}
					
						newCenter_0 += pV->newPos[0];
						newCenter_1 += pV->newPos[1];
						newCenter_2 += pV->newPos[2];
						//pV->newPos = P - step * d;

					}
					else {
						cout << "This adjustVerticesBoundaryHarmonic() method only works for Tmesh whose vertices are all on boundary." << endl;
					}
				}

				CPoint newCenter(newCenter_0, newCenter_1, newCenter_2);
				newCenter /= numV;

				#pragma omp parallel for 
				for (int i = 0; i < numV; ++i) {
					TIf::VPtr pV = pTMesh->vertices_vec[i];
					CPoint& P = pV->position();
					P = pV->newPos - newCenter;
					P /= P.norm();
				}

				originalEnergy = newEnergy;
				newEnergy = totalEnergy();
				if (abs(newEnergy - originalEnergy) < minErr) {
					minErr = abs(newEnergy - originalEnergy);
				}
				++count;
				if (count >= NUM_PRINTING) {
					count = 0;
					std::cout << "New Harmonic Energy: " << newEnergy << ".\n"
							<< "Step ERR: " << (newEnergy - originalEnergy) << ". "
							<< ". Max gradient norm: " << maxGradientNorm << std::endl;
				}

			} while (!hasConverged());
			cout << "Algorithm Converged.\n";
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::setStopGradientThreshold(double newGradientThreshold)
		{
			 gradientThreshold = newGradientThreshold;

		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::lineSearchStep(V * pV, CPoint grad)
		{
			double gradNorm2 = grad.norm2();
			double newLocalHEnergy;

			pV->step = 0.5;
			pV->newPos = pV->position() - pV->step * grad;
			pV->newPos /= pV->newPos.norm();
			newLocalHEnergy = calculateVertexNewPosLocalHarmonicEnergy(pV);
			double err =  pV->localHarmonicEnergy - newLocalHEnergy;
			while (err < LINE_SEARCH_C1 * pV->step * gradNorm2) {
				pV->step *= LINE_SEARCH_RATIO;
				if (pV->step < MIN_STEP) {
					break;
				}
				pV->newPos = pV->position() - pV->step * grad;
				pV->newPos /= pV->newPos.norm();
				newLocalHEnergy = calculateVertexNewPosLocalHarmonicEnergy(pV);
				err = pV->localHarmonicEnergy - newLocalHEnergy;
			} ;
			pV->localHarmonicEnergy = newLocalHEnergy;
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
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateTotalKForEachV()
		{
			#pragma omp parallel for
			for (int i = 0; i < numV; ++i) {
				V* pV = pTMesh->vertices_vec[i];
				pV->totalK = 0;
				pV->localHarmonicEnergy = 0;
				for (TIf::EPtr pE : TIt::V_EIterator(pV)) {
					pV->totalK += pE->k;
				}
			}
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateLocalHEnergyForEachV()
		{
			#pragma omp parallel for
			for (int i = 0; i < numV; ++i) {
				V* pV = pTMesh->vertices_vec[i];
				pV->localHarmonicEnergy = calculateVertexLocalHarmonicEnergy(pV);
			}
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateVertexLocalHarmonicEnergy(V* pV)
		{
			double hEnergy = 0;
			for (TIf::EPtr pE : TIt::V_EIterator(pV)) {
				hEnergy += STRING_EN(pE);
			}
			return hEnergy;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::calculateVertexNewPosLocalHarmonicEnergy(V * pV)
		{
			double hEnergy = 0;
			for (TIf::EPtr pE : TIt::V_EIterator(pV)) {
				TIf::VPtr pVOther = TIf::EdgeVertex1(pE) == pV ? TIf::EdgeVertex2(pE) : TIf::EdgeVertex1(pE);
				hEnergy += pE->k * (pVOther->position() - pV->newPos).norm2();
			}
			return hEnergy;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::hasConverged()
		{
			switch (convergeCondition)
			{
			case minError:
				if (minErr < epison) {
					return true;
				}
				else {
					return false;
				}
				break;
			case maxGradient:
				if (maxGradientNorm < gradientThreshold) {
					return true;
				}
				else
				{
					return false;
				}
			default:
				break;
			}
			return true;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double VolumetricHarmonicCore<TV, V, HE, TE, E, HF, F, T>::printInformation()
		{
			cout << "New harmonic Energy: "  ;
		}
	}
}