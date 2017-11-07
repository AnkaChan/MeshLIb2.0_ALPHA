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

			TetType* dynamic_construct_tet(int  v[4]);



		protected:
			typedef TInterface <TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType> TIf;
			typedef TIterators<TIf> TIt;

			HalfFaceType* dynamic_construct_half_face(TVertexType ** pTV);
			FaceType* dynamic_construct_face(KeyVec& fkey);
			EdgeType* dynamic_construct_edge(KeyVec& fkey);
			TEdgeType* dynamic_construct_tedge(KeyVec& tekey);
			FMap fMap;
			HFMap hfMap;
			HEMap heMap;
			EMap eMap;
			TEMap teMap;

			std::list<FaceType*> newCreatedFaces;
			std::list<HalfFaceType*> newCreatedHFs;
			std::list<EdgeType*> newCreatedEdges;

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
		TetType* CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::dynamic_construct_tet(int  v[4])
		{
			//set the tet->id
			++maxTId;
			TetType* pT = new TetType;
			pT->id() = maxTId;
			//set TVertices of the Tet

			for (int k = 0; k < 4; k++)
			{
				TVertexType * pTV = new TVertexType();
				pT->setTVertex(pTV, k);
				pTV->id() = k;

				VertexType * pV = m_map_Vertices[v[k]];
				
				pV->tvertices()->push_back(pTV);

				pTV->set_tet(pT);
				pTV->set_vert(pV);
			}

			//set half faces
			int order[4][3] = { { 1, 2, 3 },{ 2, 0, 3 },{ 0, 1, 3 },{ 1, 0, 2 } };

			TVertexType   * pTV[3];
			HalfFaceType * pHF[4];

			for (int i = 0; i < 4; i++)
			{
				for (int k = 0; k < 3; k++)
				{
					pTV[k] = TetTVertex(pT, order[i][k]);
				}
				pT->setHalfFace(dynamic_construct_half_face(pTV), i);
				pHF[i] = TetHalfFace(pT, i);
			}

			//Seting the dual half edges

			for (int i = 0; i < 3; i++)
			{
				HalfEdgeType * pH0 = HalfFaceHalfEdge(pHF[i]);
				pH0 = HalfEdgeNext(pH0);
				HalfEdgeType * pH1 = HalfFaceHalfEdge(pHF[(i + 1) % 3]);
				pH1 = HalfEdgePrev(pH1);

				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				KeyVec teKey;
				teKey.push_back(pT->id());
				int teIds[2];
				teIds[0] = HalfEdgeSource(pH0)->id();
				teIds[1] = HalfEdgeTarget(pH0)->id();
				if (teIds[0] < teIds[1]) {
					teKey.push_back(teIds[0]);
					teKey.push_back(teIds[1]);
				}
				else {
					teKey.push_back(teIds[1]);
					teKey.push_back(teIds[0]);
				}

				TEdgeType * pTE = dynamic_construct_tedge(teKey);
				assert(pTE != NULL);
				pTE->SetTet(pT);
				pH0->SetTEdge(pTE);
				pH1->SetTEdge(pTE);

				if (pH0->source()->id() < pH0->target()->id())
				{
					pTE->SetLeft(pH0);
					pTE->SetRight(pH1);
				}
				else
				{
					pTE->SetLeft(pH1);
					pTE->SetRight(pH0);
				}

				pTE->key(0) = pTE->left()->source()->id();
				pTE->key(1) = pTE->left()->target()->id();

				VertexType * v = m_map_Vertices[pTE->key(0)];
				v->tedges()->push_back(pTE);
			}

			HalfEdgeType * pH0 = HalfFaceHalfEdge(pHF[3]);
			for (int i = 0; i < 3; i++)
			{
				HalfEdgeType * pH1 = HalfFaceHalfEdge(pHF[2 - i]);
				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				KeyVec teKey;
				teKey.push_back(pT->id());
				int teIds[2];
				teIds[0] = HalfEdgeSource(pH0)->id();
				teIds[1] = HalfEdgeTarget(pH0)->id();
				if (teIds[0] < teIds[1]) {
					teKey.push_back(teIds[0]);
					teKey.push_back(teIds[1]);
				}
				else {
					teKey.push_back(teIds[1]);
					teKey.push_back(teIds[0]);
				}

				TEdgeType * pTE = dynamic_construct_tedge(teKey);
				assert(pTE != NULL);
				//set TEdge->Tet
				pTE->SetTet(pT);
				//set HalfEdge->TEdge
				pH0->SetTEdge(pTE);
				pH1->SetTEdge(pTE);

				if (pH0->source()->id() < pH0->target()->id())
				{
					pTE->SetLeft(pH0);
					pTE->SetRight(pH1);
				}
				else
				{
					pTE->SetLeft(pH1);
					pTE->SetRight(pH0);
				}
				pTE->key(0) = pTE->left()->source()->id();
				pTE->key(1) = pTE->left()->target()->id();

				pH0 = HalfEdgeNext(pH0);
			}
			//markNewBoundarys();
			return pT;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		HalfFaceType* CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::dynamic_construct_half_face(TVertexType ** pTV)
		{
			HalfFaceType * pHF = new HalfFaceType;
			assert(pHF != NULL);

			VertexType * pV[3];

			for (int i = 0; i < 3; i++)
			{
				pV[i] = TVertexVertex(pTV[i]);
			}

			HalfEdgeType * pH[3];
			for (int i = 0; i < 3; i++)
			{
				pH[i] = new HalfEdgeType;
				assert(pH[i] != NULL);

				pH[i]->SetHalfFace(pHF);
				pH[i]->SetSource(pTV[i]);
				pH[i]->SetTarget(pTV[(i + 1) % 3]);
				pTV[i]->set_halfedge(pH[i]);
			}

			for (int i = 0; i < 3; i++)
			{
				pH[i]->SetNext(pH[(i + 1) % 3]);
				pH[i]->SetPrev(pH[(i + 2) % 3]);
			}

			pHF->SetHalfEdge(pH[0]);

			for (int i = 0; i < 3; i++)
			{
				pHF->key(i) = pV[i]->id();
			}

			//bubble

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2 - i; j++)
				{
					if (pHF->key(j) > pHF->key(j + 1))
					{
						int tmp = pHF->key(j);
						pHF->key(j) = pHF->key(j + 1);
						pHF->key(j + 1) = tmp;
					}
				}
			}

			assert(pHF->key(0) < pHF->key(1));
			assert(pHF->key(1) < pHF->key(2));

			//find or create halfface's face
			KeyVec fKey;
			fKey.push_back(pHF->key(0));
			fKey.push_back(pHF->key(1));
			fKey.push_back(pHF->key(2));
			auto iterF = fMap.find(fKey);
			if (iterF != fMap.end()) {
				//if the face exists, it must contain only one halfface.
				FaceType * pF = iterF->second;
				pHF->SetFace(pF);
				if (pF->left() == NULL) {
					pF->SetLeft(pHF);
					pHF->SetDual(pF->right());
				}
				else {
					pF->SetRight(pHF);
					pHF->SetDual(pF->left());
				}
			}
			else {
				//if the face does not exist, we create a new face, and set this halfface as left
				FaceType * pF = dynamic_construct_face(fKey);
				pF->SetLeft(pHF);
			}
			return pHF;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType * CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::dynamic_construct_edge(KeyVec & eKey)
		{
			EdgeType* pE = new EdgeType;
			eMap.insert(EMapPair(eKey, pE));
			return pE;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline FaceType* CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::dynamic_construct_face(KeyVec& fKey)
		{
			FaceType* pF = new FaceType;

			fMap.insert(FMapPair(fKey, pF));
			newCreatedFaces.push_back(pF);
			return pF;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TEdgeType * CDynamicTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::dynamic_construct_tedge(KeyVec & tekey)
		{
			TEdgeType * pTE= new TEdgeType;
			teMap.insert(TEMapPair(tekey, pTE));

			KeyVec eKey = { tekey[1], tekey[2] };

			EMap::iterator eIter = eMap.find(eKey);
			EdgeType * pE;
			if (eIter != eMap.end()) {
				pE = eIter->second;
			}
			else {
				pE = dynamic_construct_edge(eKey); 
			}
			pTE->SetEdge(pE);
			std::list<TEdgeType*>* pTEList = (std::list<TEdgeType*>*)pE->edges();
			pTEList->push_back(pTE);
			return pTE;
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