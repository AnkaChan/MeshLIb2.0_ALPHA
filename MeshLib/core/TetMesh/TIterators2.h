#pragma once
#include <set>
#include <list>
#include <assert.h>
#include <iterator>
#include "TInterface.h"
namespace MeshLib
{
	namespace TMeshLib
	{
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		struct TIteratorCore : private TInterface< TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType> {
			/*!
			\brief TMesh Vertex Iterator

			go through all the vertices in the TetMesh
			*/
			class TM_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				TM_VIterator(TMeshPtr pTMesh) : m_pMesh(pTMesh), m_iter(pTMesh->vertices().begin()) {};
				/*! dereferencing */
				VPtr value() { return *m_iter; };
				VPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_VIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_VIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_VIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_VIterator operator++(int) { TM_VIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_VIterator begin() { return TM_VIterator(m_pMesh); };
				TM_VIterator end() { return TM_VIterator(m_pMesh, m_pMesh->vertices().end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pMesh->vertices().end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_VIterator(TMeshPtr pTMesh, typename std::list<VPtr>::iterator iter) : m_pMesh(pTMesh), m_iter(iter) {};
				/*! TetMesh pointer */
				TMeshPtr       m_pMesh;
				/*! Current vertex list iterator */
				//int             m_id;
				typename std::list<VPtr>::iterator m_iter;
			};

			/*!
			\brief TVertex Vertex Iterator

			go through all the adjacent vertices in of a vertex
			*/
			class V_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				V_VIterator(VPtr pV) : m_pV(pV), pEdges(VertexEdgeList(pV))
				{
					m_iter = pEdges->begin();;
				}

				/*! dereferencing */
				VPtr value() { 
					EPtr e = *m_iter;
					VPtr v1 = EdgeVertex1(e);
					VPtr v2 = EdgeVertex2(e);
					return v1 == m_pV ? v2 : v1; 
				};
				VPtr operator*() {
					EPtr e = *m_iter;
					VPtr v1 = EdgeVertex1(e);
					VPtr v2 = EdgeVertex2(e);
					return v1 == m_pV ? v2 : v1;
				};

				/*! bool operator*/
				bool operator==(const V_VIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const V_VIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				V_VIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				V_VIterator  operator++(int) { V_VIterator tmp(m_pV, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				V_VIterator begin() { return V_VIterator(m_pV); };
				V_VIterator end() { return V_VIterator(m_pV, pEdges->end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				V_VIterator(VPtr pV, typename std::list<EPtr>::iterator iter) :m_pV(pV), pEdges(VertexEdgeList(pV)), m_iter(iter) {};
				/*! Pointer to the center vertex*/
				VPtr m_pV;
				/*! center vertex */
				std::list<EPtr> * pEdges;
				/*! set iterator */
				typename std::list<EPtr>::iterator m_iter;

			};

			/*!
			\brief MeshTet Iterator

			go through all the Tets in a mesh
			*/
			class TM_TIterator : public std::iterator<std::forward_iterator_tag, TPtr> {
			public:
				TM_TIterator(TMeshPtr pTMesh) : m_pMesh(pTMesh), m_iter(pTMesh->tets().begin()) {};
				/*! dereferencing */
				TPtr value() { return *m_iter; };
				TPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_TIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_TIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_TIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_TIterator operator++(int) { TM_TIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_TIterator begin() { return TM_TIterator(m_pMesh); };
				TM_TIterator end() { return TM_TIterator(m_pMesh, m_pMesh->tets().end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pMesh->tets().end(); };

			private:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_TIterator(TMeshPtr pTMesh, typename std::list<TPtr>::iterator iter) : m_pMesh(pTMesh), m_iter(iter) {};
				/*! pointer to a TetMesh */
				TMeshPtr m_pMesh;
				/*! tet iterator */
				typename std::list<TPtr>::iterator m_iter;
			};

			/*!
			\brief TetHalfFace Iterator

			go through all the halfaces of a Tet in a mesh
			*/
			class T_HFIterator : public std::iterator<std::forward_iterator_tag, HFPtr>
			{
			public:
				T_HFIterator(TPtr pT) : m_pTet(pT), m_id(0) {};

				/*! dereferencing */
				HFPtr operator*() { assert(m_id < 4); return TetHalfFace(m_pTet, m_id); };
				HFPtr value() { assert(m_id < 4);  return TetHalfFace(m_pTet, m_id); };

				/*! bool operator*/
				bool operator==(const T_HFIterator& otherIter) { return m_id == otherIter.m_id; };
				bool operator!=(const T_HFIterator& otherIter) { return m_id != otherIter.m_id; };

				/*! iterate ++ */
				T_HFIterator& operator++() { ++m_id; return *this; };
				/*! ++iterator */
				T_HFIterator operator++(int) { T_HFIterator tmp(m_pMesh, m_iter); ++m_id; return tmp; };

				/*! return the begin and end iterators*/
				T_HFIterator begin() { return T_HFIterator(m_pTet); };
				T_HFIterator end() { return T_HFIterator(m_pTet, 4); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == 4; };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				T_HFIterator(TPtr pT, int id) : m_pTet(pT), m_id(id) {};
				/*! current tet */
				CTet *             m_pTet;
				/*! current half_face */
				int                m_id;
			};

			/*!
			\brief Mesh Edge Iterator
			go through all the edges of a Tet in a mesh
			*/
			class TM_EIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
			public :
				TM_EIterator(TMeshPtr pTMesh) : m_pMesh(pTMesh), m_iter(pTMesh->edges().begin()) {};
				/*! dereferencing */
				EPtr value() { return *m_iter; };
				EPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_EIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_EIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_EIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_EIterator operator++(int) { TM_EIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_EIterator begin() { return TM_EIterator(m_pMesh); };
				TM_EIterator end() { return TM_EIterator(m_pMesh, m_pMesh->edges().end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pMesh->edges().end(); };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_EIterator(TMeshPtr pTMesh, typename std::list<EPtr>::iterator iter) : m_pMesh(pTMesh), m_iter(iter) {};
				/*! pointer to the mesh */
				TMeshPtr m_pMesh;
				/*! edge list iterator */
				typename std::list<EPtr>::iterator m_iter;
			};

			/*!
			\brief Edge->TEdge Iterator
			go through all the tedges of an edge
			*/
			class E_TEIterator : public std::iterator<std::forward_iterator_tag, TEPtr> {
			public:
				E_TEIterator(EPtr pE) : m_pEdge(pE), m_iter(EdgeTEdgeList(pE)->begin()) {};
				/*! dereferencing */
				TEPtr value() { return *m_iter; };
				TEPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const E_TEIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const E_TEIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				E_TEIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				E_TEIterator operator++(int) { TM_EIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				E_TEIterator begin() { return E_TEIterator(m_pEdge); };
				E_TEIterator end() { return E_TEIterator(m_pEdge, EdgeTEdgeList(m_pEdge)->end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == EdgeTEdgeList(m_pEdge)->end(); };
			private:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				E_TEIterator(EPtr pE, typename std::list<TEPtr>::iterator iter) : m_pEdge(pE), m_iter(iter) {};
				/*! pointer to the edge */
				EPtr m_pEdge;
				/*! TEdge list iterator */
				typename std::list<TEPtr>::iterator m_iter;
			};

			/*!
			\brief Vertex->Edge Iterator
			go through all the edges of a vertex
			*/
			class V_EIterator : public std::iterator<std::forward_iterator_tag, EPtr>{
			public:
				V_EIterator(VPtr pV) : m_pV(pV), m_iter(VertexEdgeList(pV)->begin()) {};

				/*! dereferencing */
				EPtr value() { return *m_iter; };
				EPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const V_EIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const V_EIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				V_EIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				V_EIterator  operator++(int) { V_EIterator tmp(m_pV, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				V_EIterator begin() { return V_EIterator(m_pV); };
				V_EIterator end() { return V_EIterator(m_pV, VertexEdgeList(m_pV)->end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			private:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				V_EIterator(VPtr pV, typename std::list<EPtr>::iterator iter) :m_pV(pV), m_iter(iter) {};
				/*! Edge List of the point */
				VPtr m_pV;
				/*! Edge list iterator */
				typename std::list<EPtr>::iterator m_iter;
			};

			/*!
			\brief Tet->Edge Iterator
			go through all the edges of a Tet
			*/
			class T_EIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
			public:
				T_EIterator(TPtr pT) : m_pT(pT)
				{
					for (int i = 0; i < 4; i++)
					{
						HFPtr pHF = TetHalfFace(pT, i);
						HEPtr pHE = HalfFaceHalfEdge(pHF);
						for (int j = 0; j < 3; j++)
						{
							TEPtr pTE = HalfEdgeTEdge(pHE);
							EPtr pE = TEdgeEdge(pTE);
							m_edges.insert(pE);
							pHE = HalfEdgeNext(pHE);
						}
					}
					m_iter = m_edges.begin();
				};

				/*! dereferencing */
				EPtr value() { return *m_iter; };
				EPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const T_EIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const T_EIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				T_EIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				T_EIterator  operator++(int) { T_EIterator tmp(m_pT, m_edges, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				T_EIterator begin() { return T_EIterator(m_pT, m_edges, m_edges.begin()); };
				T_EIterator end() { return T_EIterator(m_pT, m_edges, m_edges.end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			private:

				T_EIterator(TPtr pT, std::set<EPtr> edges, typename std::set<EPtr>::iterator iter) : m_pT(pT), m_iter(iter), m_edges(edges) {};

				/*! Pointer to the tet */
				TPtr m_pT;
				/*! set of edges adjacent to the central vertex */
				std::set<EPtr> m_edges;
				/*! edge set iterator */
				typename std::set<EPtr>::iterator m_iter;
			};

			/*!
			\brief Mesh->Face Iterator
			go through all the faces of a Mesh
			*/
			class TM_FIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
			public:
				TM_FIterator(TMeshPtr pTMesh) : m_pTMesh(pTMesh), m_iter(m_pTMesh->faces().begin()) {};

				/*! dereferencing */
				FPtr value() { return *m_iter; };
				FPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_FIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_FIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_FIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_FIterator operator++(int) { TM_FIterator tmp(m_pTMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_FIterator begin() { return TM_FIterator(m_pTMesh); };
				TM_FIterator end() { return TM_FIterator(m_pTMesh, m_pTMesh->faces().end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pTMesh->faces().end(); };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_FIterator(TMeshPtr pTMesh, typename std::list<FPtr>::iterator iter) : m_pTMesh(pTMesh), m_iter(iter) {};
				/*! pointer to the mesh */
				TMeshPtr m_pTMesh;
				/*! edge list iterator */
				typename std::list<FPtr>::iterator m_iter;
			};

			/*!
			\brief Edge->Face Iterator
			go through all the faces adjacent to an edge
			*/
			class E_FIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
			public:
				E_FIterator(EPtr pE) {
					std::list<TEPtr> * TEdges = EdgeTEdgeList(pE);
					for (TEPtr pT : *TEdges)
					{
						HEPtr pL = TEdgeLeftHalfEdge(pT);
						HEPtr pR = TEdgeRightHalfEdge(pT);
								    
						HFPtr pHF = HalfEdgeHalfFace(pL);
						FPtr   pF = HalfFaceFace(pHF);
						m_faces.insert(pF);

						pHF = HalfEdgeHalfFace(pR);
						pF  = HalfFaceFace(pHF);
						m_faces.insert(pF);
					}

					m_iter = m_faces.begin();
				}

				/*! dereferencing */
				FPtr value() { return *m_iter; };
				FPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const E_FIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const E_FIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				E_FIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				E_FIterator  operator++(int) { E_FIterator tmp(m_faces, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				E_FIterator begin() { return E_FIterator( m_faces, m_faces.begin()); };
				E_FIterator end() { return E_FIterator( m_faces, m_faces.end()); };

				/*! old style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			private:

				E_FIterator(std::set<FPtr> faces, typename std::set<FPtr>::iterator iter) : m_iter(iter), m_faces(faces) {};

				/*! set of edges adjacent to the central vertex */
				std::set<FPtr> m_faces;
				/*! edge set iterator */
				typename std::set<FPtr>::iterator m_iter;
			};


		};

	}
}