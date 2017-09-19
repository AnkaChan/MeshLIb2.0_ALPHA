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

using std::cout;
using std::endl;

namespace MeshLib {
	namespace TMeshLib {
		class _vertexInDecomposition {
			bool inDecompositionList = false;
			bool inCandidateList = false;
		};
		class CVertexDecomposition : public CVertex, public _vertexInDecomposition {};


		template< typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class CVertexDecomposer {
		public:

			struct VertexDecompositionResult
			{
				T * pstartSimplex;
				std::shared_ptr<std::list<V*>> pVertexDecompositionList;
			};

			CVertexDecomposer(typename TIf::MeshPtr pTMesh) :
				m_pTMesh(pTMesh), 
				m_pVertexDecompositionList(new std::list<V*>)
			{};

			bool vertexDecompositionGreedily(T * pStartT);
		private:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef TIterators<TIf> TIt;

			typename TIf::MeshPtr m_pTMesh;
			T * m_pstartSimplex;
			std::shared_ptr<std::list<V*>> m_pVertexDecompositionList;
			std::list<V*> candidateList;
			std::set<HF*> surfaceHalfFaceSet;

			void putInDecompositionList(V * pV) {
				m_pVertexDecompositionList->push_back(pV);
				pV->inDecompositionList = true;
			};
			void removeFromDecompositionList(std::list<V*>::iterator iter) {
				(*iter)->inDecompositionList = false;
				m_pVertexDecompositionList->erase(iter);
			};

			void putInCandidateList(V * pV) {
				candidateList.push_back(pV);
				pV->inCandidateList = true;
			};
			void removeFromCandidateList(std::list<V*>::iterator iter) {
				(*iter)->inCandidateList = false;
				candidateList.erase(iter);
			};

			void putInSurfaceHFSet(HF * pHF) {
				surfaceHalfFaceSet.insert(pV);
			};
			void removeFromSurfaceHFSet(HF* pHF) {
				surfaceHalfFaceSet.erase(pHF);
			};

			V * getNextVGreedily();
			bool isVertexDecomposition(V * pNextV) {

			}

			void getLink(V * pV, std::set<HF*> pLinkSet);
		};

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool CVertexDecomposer<TV, V, HE, TE, E, HF, F, T>::vertexDecompositionGreedily(T * pStartT)
		{
			m_pstartSimplex = pStartT;

			// Make Initial candidate List
			for (auto pV : TIt::T_VIterator(pT)) {
				pV->inDecompositionList = true;
				pV->inCandidateList = false;
				for (auto pNeiV : TIt::V_VIterator(pV)) {
					if (!pNeiV->inDecompositionList) {
						putInCandidateList(pNeiV);
					}
				}
			}

			for (auto pHF : TIt::T_HFIterator(pT)) {
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
		V * CVertexDecomposer<TV, V, HE, TE, E, HF, F, T>::getNextVGreedily()
		{
			for (auto vIter = candidateList.begin(); vIter != candidateList.end(); ++vIter) {
				V * pV = *vIter;

				std::set<HF*> linkSet; 
				getLink(pV, linkSet);


				if (isD2(linkSet)) {

					removeFromCandidateList(vIter);
					return pV;
				}
			}
			return NULL;
		}
	}
}