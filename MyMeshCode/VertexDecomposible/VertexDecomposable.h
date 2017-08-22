#pragma once
#include <memory>
#include <vector>
#include <list>
#include <random>
#include <iostream>

#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/vertex.h>
#include <MeshLib/core/TetMesh/tvertex.h>
#include <MeshLib/core/TetMesh/edge.h>
#include <MeshLib/core/TetMesh/tedge.h>
#include <MeshLib/core/TetMesh/face.h>
#include <MeshLib/core/TetMesh/halfface.h>
#include <MeshLib/core/TetMesh/halfedge.h>
#include <MeshLib/core/TetMesh/tet.h>
#include <MeshLib/core/TetMesh/titerators2.h>
#include <MeshLib/core/TetMesh/tinterface.h>
#include "C2DMesh.h"

using std::cout;
using std::endl;

namespace MeshLib {

	namespace TMeshLib {
		class _vertexInDecomposition {
		public:
			bool inDecompositionList = false;
			bool inCandidateList = false;
		};

		class _tetInDecomposition {
		public:
			bool inDecompositionList = false;
		};

		class CVertexDecomposition : public CVertex, public _vertexInDecomposition {};
		class CTetDecomposition : public CTet, public _tetInDecomposition {};

		template< typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class CVertexDecomposerCore {
		private:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef TIterators<TIf> TIt;
		public:

			struct VertexDecompositionResult
			{
				T * pstartSimplex;
				std::shared_ptr<std::list<V*>> pVertexDecompositionList;
			};

			CVertexDecomposerCore() :
				m_pVertexDecompositionList(new std::list<V*>)
			{};

			bool vertexDecompositionGreedily(T * pStartT);
			void setInputTMesh(typename TIf::TMeshPtr pTMesh) {
				m_pTMesh = pTMesh;
			};
		private:
			typename TIf::TMeshPtr m_pTMesh;
			T * m_pstartSimplex;
			std::shared_ptr<std::list<V*>> m_pVertexDecompositionList;
			std::list<V*> candidateList;
			std::set<HF*> surfaceHalfFaceSet;

			void putInDecompositionList(V * pV) {
				m_pVertexDecompositionList->push_back(pV);
				pV->inDecompositionList = true;
			};
			void removeFromDecompositionList(typename std::list<V*>::iterator iter) {
				(*iter)->inDecompositionList = false;
				m_pVertexDecompositionList->erase(iter);
			};

			void putInCandidateList(V * pV) {
				candidateList.push_back(pV);
				for (TV * pTV : TIt::V_TVIterator(pV)) {
					T * pT = TIf::TVertexTet(pTV);
					for (HF * pHF : TIt::T_HFIterator(pT)) {
						HF * pHFDual = TIf::HalfFaceDual(pHF);
						if (TIf::HalfFaceTet(pHFDual)->inDecompositionList) {
							TIf::HalfFaceTet(pHF)->inDecompositionList = true;
						}
					}
				}
				pV->inCandidateList = true;
			};
			void removeFromCandidateList(typename std::list<V*>::iterator iter) {
				(*iter)->inCandidateList = false;
				candidateList.erase(iter);
			};

			void putInSurfaceHFSet(HF * pHF) {
				surfaceHalfFaceSet.insert(pHF);
			};
			void removeFromSurfaceHFSet(HF* pHF) {
				surfaceHalfFaceSet.erase(pHF);
			};

			V * getNextVGreedily();
			bool isVertexDecomposition(V * pNextV) {

			}

			void getLink(V * pV, std::set<HF*>& pLinkSet);
		};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool CVertexDecomposerCore<TV, V, HE, TE, E, HF, F, T>::vertexDecompositionGreedily(T * pStartT)
		{
			m_pstartSimplex = pStartT;
			m_pstartSimplex->inDecompositionList = true;
			// Make Initial candidate List
			for (auto pV : TIt::T_VIterator(m_pstartSimplex)) {
				pV->inDecompositionList = true;
				pV->inCandidateList = false;
				for (auto pNeiV : TIt::V_VIterator(pV)) {
					if (!pNeiV->inDecompositionList) {
						putInCandidateList(pNeiV);
					}
				}
			}

			for (auto pHF : TIt::T_HFIterator(m_pstartSimplex)) {
				putInSurfaceHFSet(pHF);
			}

			while (!candidateList.empty()) {
				V * pNextV = getNextVGreedily();
				if (pNextV == NULL)
					return false;
				else
					putInDecompositionList(pNextV);

				for (auto pNeiV : TIt::V_VIterator(pNextV)) {
					if (!pNeiV->inDecompositionList && !!pNeiV->inCandidateList) {
						putInCandidateList(pNeiV);
					}
				}
			}

			if (m_pVertexDecompositionList->size() == (m_pTMesh->vertices().size() - 4))
				return true;
			else
				return false;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		V * CVertexDecomposerCore<TV, V, HE, TE, E, HF, F, T>::getNextVGreedily()
		{
			for (auto vIter = candidateList.begin(); vIter != candidateList.end(); ++vIter) {
				V * pV = *vIter;

				std::set<HF*> linkSet; 
				getLink(pV, linkSet);
				std::set<V*> setVInLink;

				FList fList;
				VList vList;

				for (HF* pHF : linkSet) {
					FaceElement fe;
					fe.id = fList.size();
					int i = 0;
					for (V* pV : TIt::HF_VIterator(pHF)) {
						fe.vertices[i] = pV->id();
						++i;
						setVInLink.insert(pV);
					}
					fList.push_back(fe);
				}

				for (V * pV : setVInLink) {
					VertexElement ve(pV->id(), pV->position());
					vList.push_back(ve);
				}

				C2DMesh linkMesh;
				linkMesh.readArray(vList, fList);

				if (linkMesh.isD2()) {
					removeFromCandidateList(vIter);
					return pV;
				}
			}
			return NULL;
		}
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void CVertexDecomposerCore<TV, V, HE, TE, E, HF, F, T>::getLink(V * pV, std::set<HF*>& pLinkSet)
		{
			for (TV * pTV : TIt::V_TVIterator(pV)) {
				T * pT = TIf::TVertexTet(pTV);
				for (HF * pHF : TIt::T_HFIterator(pT)) {
					HF * pHFDual = TIf::HalfFaceDual(pHF);
					if (TIf::HalfFaceTet(pHFDual)->inDecompositionList) {
						pLinkSet.insert(pHFDual);
					}
				}
			}
		}

		template <typename TIf>
		class CVertexDecomposer : public CVertexDecomposerCore<typename TIf::TVType, typename TIf::VType, typename TIf::HEType,
			typename TIf::TEType, typename TIf::EType, typename TIf::HFType, typename TIf::FType, typename TIf::TType>
		{};
	}
}