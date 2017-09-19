#pragma once
#include "MeshLib\core\TetMesh\TMeshLibHeaders.h"
namespace MeshLib {
	namespace TMeshLib {
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class NegativeAdjustCore {
		public:
			NegativeAdjustCore() {};
			void setInputTMesh(TMeshType* new_pTMesh) {
				pTMesh = new_pTMesh;
			};
			bool adjust();

			double setMinAngleDegree();
		private:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef CTMesh< TV, V, HE, TE, E, HF, F, T > TMeshType;
			typedef TIterators<TIf> TIt;

			TMeshType * pTMesh;
			double orientedVolume(T* pT);
			double step = 0.05;
			double minAngle = 0;
			int maxIteration = 100;
			double maxCosAngle 1.0;
			double getMaxCosAngle(T * pT);
			bool isAvaliablePosition(CPoint newA, TV * pTV);
		};

		template <typename TIf>
		class NegativeAdjust : public NegativeAdjustCore<typename TIf::TVType, typename TIf::VType, typename TIf::HEType,
			typename TIf::TEType, typename TIf::EType, typename TIf::HFType, typename TIf::FType, typename TIf::TType>
		{};
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool NegativeAdjustCore<TV, V, HE, TE, E, HF, F, T>::adjust()
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
						numFixPoints++;
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
		inline double NegativeAdjustCore<TV, V, HE, TE, E, HF, F, T>::setMinAngleDegree()
		{
			minAngle = PI * newMinAngle / 180.0;
			maxCosAngle = abs(cos(minAngle));
			return maxCosAngle;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline double NegativeAdjustCore<TV, V, HE, TE, E, HF, F, T>::orientedVolume(T * pT)
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
		inline double NegativeAdjustCore<TV, V, HE, TE, E, HF, F, T>::getMaxCosAngle(T * pT)
		{
			return maxCosAngle;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool NegativeAdjustCore<TV, V, HE, TE, E, HF, F, T>::isAvaliablePosition(CPoint newA, TV * pTV)
		{
			bool avaliable = true;
			CPoint oA = TIf::TVertexVertex(pTV)->position();
			TIf::TVertexVertex(pTV)->position() = newA;
			V* pV = TIf::TVertexVertex(pTV);
			for (auto pTVTet : TIt::V_TVIterator(pV)) {
				T* pT = TIf::TVertexTet(pTVTet);
				double maxCosAngleThisTet = getMaxCosAngle(pT);
				if ( abs(maxCosAngleThisTet) > abs(maxCosAngle)) {
					avaliable = false;
					break;
				}
				double orientedVolumeThisTet = orientedVolume(pT);
				if ( orientedVolumeThisTet < 0) {
					avaliable = false;
					break;
				}
			}

			TIf::TVertexVertex(pTV)->position() = oA;
			return avaliable;
		}
	}
}