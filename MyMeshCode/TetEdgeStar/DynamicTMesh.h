#pragma once
#include <MeshLib/core/TetMesh/basetmesh.h>
#include <MeshLib/core/TetMesh/TIterators2.h>
#include <Array>
namespace MeshLib {
	namespace TMeshLib {
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		class CDynamicTMesh : public CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>
		{
		public:
			typedef std::vector<int> KeyVec;
			typedef std::pair<KeyVec, FaceType*> FMapPair;
			typedef std::pair<KeyVec, HalfFaceType*> HFMapPair;
			typedef std::pair<KeyVec, HalfEdgeType*> HEMapPair;
			typedef std::pair<KeyVec, EdgeType*> EMapPair;
			typedef std::pair<KeyVec, TEdgeType*> TEMapPair;

			typedef std::map<KeyVec, FaceType*> FMap;
			typedef std::map<KeyVec, HalfFaceType*> HFMap;
			typedef std::map<KeyVec, HalfEdgeType*> HEMap;
			typedef std::map<KeyVec, EdgeType*> EMap;
			typedef std::map<KeyVec, TEdgeType*> TEMap;

			void _load_t(const char * input, bool checkOrientation = false);
			VertexType* faceStar(EdgeType * pTE, CPoint2 splitPoint);
			void tetEdgeSplit(TEdgeType * pSplitTE, VertexType * pSplitV, TetType ** pNewT1s );
			void deleteTet(TetType * pT);
			int maxTId;

			void make2StandardFKey(KeyVec & key);
			void make2StandardHFKey(KeyVec & key);


		protected:
			typedef TInterface <TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType> TIf;
			typedef TIterators<TIf> TIt;

			void dynamic_construct_tet();
			FMap fMap;
			HFMap hfMap;
			HEMap heMap;
			EMap eMap;
			TEMap teMap;

			/*
				get face's key, which is a std::vector<int> of size 3, contains id of its three vertices,
				and the order of ids are sorted from small to big.
			*/
			void getKey(FaceType * pF, std::vector<int>& key);
			/*
				get halfface's key, which is a std::vector<int> of size 3, contains id of its three vertices,
				and the order of ids is consistent with the rotating order of the halfface,
				to make the key unique, we require that the sequence starts with the smallest id
			*/
			void getKey(HalfFaceType * pHF, std::vector<int>& key);
			/*
				get face's key, which is a std::vector<int> of size 2 contains id of its two vertices,
				and the order of ids are sorted from small to big.
			*/
			void getKey(EdgeType * pE, std::vector<int>& key);
			/*
				get halfface's key, which is a std::vector<int> of size 3 contains of id the tet it belongs to and its two vertices,
				and the order of ids is consistent with the direction of the halfedge,
				the first number is tet id

			*/
			void getKey(HalfEdgeType * pHe, std::vector<int>& key);
			/*
				get face's key, which is a std::vector<int> of size 3 contains contains of id the tet it belongs to and its two vertices,
				the first number is tet id
				and the order of vertex ids are sorted from small to big.
			*/
			void getKey(TEdgeType * pTE, std::vector<int>& key);
		};

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load_t(const char * input, bool checkOrientation)
		{
			CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load_t(input, checkOrientation);
			maxTId = 0;
			for (auto pT : m_pTets) {
				if (pT->id() > maxTId) {
					maxTId = pT->id();
				}
			}

			// make map using keys, those map will replace the  td::list<Type> named "m_pType" in original Tmesh.
			// in other words, those  std::list<Type> named "m_pType" are not realiable anymore, so don't use them in 
			// dynamic Tmesh.
			for (auto pF : m_pFaces) {
				KeyVec key;
				getKey(pF , key);
				fMap.insert(FMapPair(key, pF));
			}

			for (auto pHF : m_pHalfFaces) {
				KeyVec key;
				getKey(pHF, key);
				hfMap.insert(HFMapPair(key, pHF));
			}

			for (auto pHE : m_pHalfEdges) {
				KeyVec key;
				getKey(pHE, key);
				heMap.insert(HEMapPair(key, pHE));
			}
			
			for (auto pE : m_pEdges) {
				KeyVec key;
				getKey(pE, key);
				eMap.insert(EMapPair(key, pE));
			}

			for (auto pTE : m_pTEdges) {
				KeyVec key;
				getKey(pTE, key);
				teMap.insert(TEMapPair(key, pTE));
			}

			std::cout << "load done!\n" << endl;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::faceStar(EdgeType * pTE, CPoint2 splitPoint)
		{
			return NULL;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::tetEdgeSplit(TEdgeType * pSplitTE, VertexType * pSplitV, TetType ** pNewTets)
		{
			TetType * pT = TEdgeTet(pSplitTE);

			std::array<int, 4> T0, T1, T2;
			for (int i = 0; i < 4; i++) {
				VertexType * pV = pT->vertex(i);
				T0[i] = pV->id();
			}

			deleteTet(pT);

			VertexType * pVOnTE1 = TEdgeVertex1(pSplitTE);
			VertexType * pVOnTE2 = TEdgeVertex2(pSplitTE);
			int id1 = pVOnTE1->id(), id2 = pVOnTE2->id();

			T1 = T0;
			T2 = T0;

			for (int i = 0; i < 4; i++) {
				if (T1[i] == id1) {
					T1[i] = pSplitV->id();
					break;
				}
			}
			for (int i = 0; i < 4; i++) {
				if (T2[i] == id2) {
					T2[i] = pSplitV->id();
					break;
				}
			}

			TetType pNewT1 = new TetType;
			TetType pNewT2 = new TetType;

			_construct_tet(pNewT1, maxTId + 1, T1.data());
			_construct_tet(pNewT2, maxTId + 2, T2.data());
			maxTId += 2;

			pNewTets[0] = pNewT1;
			pNewTets[1] = pNewT2;

		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::deleteTet(TetType * pT)
		{
			// first step: delete 4 halffaces, and corresponding face if face becomes empty
			KeyVec hfkey, fkey;
			hfkey.reserve(3);
			fkey.reserve(3);
			for (int i = 0; i < 4; ++i) {
				HalfFaceType * pHF = (HalfFaceType *)pT->half_face(i);
				hfkey.clear();
				getKey(pHF, hfkey);
				hfMap.erase(hfkey);
				// delete pHF from corresponding face
				FaceType * pF = HalfFaceFace(pHF);
				fkey.clear();
				getKey(pF, fkey);
				if (pF->left() == pHF) {
					pF->SetLeft(NULL);
				}
				else {
					pF->SetRight(NULL);
				}
				//if face becomes empty, delete face
				if (pF->left() == NULL && pF->right() == NULL) {
					fMap.erase(fkey);
					delete pF;
				}
			}

			// second step: delete tedge, halfedge, and edge if it becomes empty
			KeyVec keyL, keyR, ekey;
			keyL.reserve(3);
			keyR.reserve(3);
			for (TEdgeType * pTE : TIt::T_TEIterator(pT)) {
				bool edge_empty = false;
				HalfEdgeType *pHEL = (HalfEdgeType *) pTE->left();
				HalfEdgeType *pHER = (HalfEdgeType *) pTE->right();

				keyL.clear();
				keyR.clear();
				getKey(pHEL, keyL);
				getKey(pHER, keyR);
				heMap.erase(keyL);
				heMap.erase(keyR);
				delete pHEL;
				delete pHER;

				EdgeType * pE = TEdgeEdge(pTE);
				std::list<TEdgeType*> * pTEList = EdgeTEdgeList(pE);
				pTEList->remove(pTE);
				ekey.clear();
				if (pTEList->empty()) {
					getKey(pE, ekey);
					edge_empty = true;
				}

				KeyVec tekey;
				getKey(pTE, tekey);
				teMap.erase(tekey);
				delete pTE;
				if (edge_empty) {
					eMap.erase(ekey);
					delete pE;
				}
			}
			
			// third step: to delete all the tvertices
			for (int i = 0; i < 4; ++i) {
				TVertexType * pTV = (TVertexType *)pT->tvertex(i);
				std::list<TVertexType*> * pTVList = VertexTVertexList(TVertexVertex(pTV));
				// delete pTV from corresponding vertex's TVertexList
				pTVList->remove(pTV);
				delete pTV;
			}

			//at last, delete tet
			m_map_Tets.erase(pT->id());
			delete pT;
		}


		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::make2StandardFKey(KeyVec & key)
		{
			std::sort(key.begin(), key.end());
			 
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::make2StandardHFKey(KeyVec & key)
		{
			int smallest_id = key.front();
			int first_pos = 0;
			for (int i = 1; i < 3; i++) {
				if (smallest_id > key[i]) {
					smallest_id = key[i];
					first_pos = i;
				}
			}

			switch (first_pos) {
			case 0:
				break;
			case 1:
				key = { key[1], key[2], key[0] };
				break;
			case 2:
				key = { key[2], key[0], key[1] };
			default:
				break;
			}
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::getKey(FaceType * pF, std::vector<int>& key)
		{
			HalfFaceType * pHF = (HalfFaceType*) (pF->left() != NULL ? pF->left() : pF->right());
			assert(pHF != NULL);
			HalfEdgeType * pHE = pHF->half_edge();
			for (int i = 0; i < 3; i++) {
				key.push_back(HalfEdgeSource(pHE)->id());
				pHE = pHE->next();
			}
			make2StandardFKey(key);
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::getKey(HalfFaceType * pHF, std::vector<int>& key)
		{
			HalfEdgeType * pHE = pHF->half_edge();
			for (int i = 0; i < 3; i++) {
				key.push_back(HalfEdgeSource(pHE)->id());
				pHE = pHE->next();
			}
			make2StandardHFKey(key); 
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::getKey(EdgeType * pE, std::vector<int>& key)
		{
			key.push_back(EdgeVertex1(pE)->id());
			key.push_back(EdgeVertex2(pE)->id());

			if (key[0] > key[1]) {
				int tmp = key[1];
				key[1] = key[0];
				key[0] = tmp;
			}
			
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::getKey(HalfEdgeType * pHE, std::vector<int>& key)
		{
			TetType * pT = HalfEdgeTet(pHE);
			key.push_back(pT->id());
			key.push_back(HalfEdgeSource(pHE)->id());
			key.push_back(HalfEdgeTarget(pHE)->id());
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::getKey(TEdgeType * pTE, std::vector<int>& key)
		{
			TetType * pT = TEdgeTet(pTE);
			key.push_back(pT->id());
			EdgeType * pE = TEdgeEdge(pTE);
			key.push_back(EdgeVertex1(pE)->id());
			key.push_back(EdgeVertex2(pE)->id());

			if (key[1] > key[2]) {
				int tmp = key[2];
				key[2] = key[1];
				key[1] = tmp;
			}
		}


	}
}