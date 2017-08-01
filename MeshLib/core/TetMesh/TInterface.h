#pragma once
#include <list>

namespace MeshLib
{
	namespace TMeshLib
	{
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		class CTMesh;
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		struct TInterface {
		public:
			typedef TVertexType     TVType;
			typedef VertexType      VType;
			typedef HalfEdgeType    HEType;
			typedef TEdgeType       TEType;
			typedef TetType         TType;
			typedef EdgeType        EType;
			typedef HalfFaceType    HFType;
			typedef FaceType        FType;

			typedef TVertexType  *  TVPtr;
			typedef VertexType   *  VPtr;
			typedef HalfEdgeType *  HEPtr;
			typedef TEdgeType    *  TEPtr;
			typedef TetType      *  TPtr;
			typedef EdgeType     *  EPtr;
			typedef HalfFaceType *  HFPtr;
			typedef FaceType     *  FPtr;

			typedef CTMesh<TVType, VType, HEType, TEType, EType, HFType, FType, TType> TMeshType;
			typedef TMeshType * TMeshPtr;

			//Access Vertex data members
			/*! Vertex->Edge List */
			static std::list<EdgeType*> * VertexEdgeList(VertexType * pVertex);
			/*! Vertex->TEdge List */
			static std::list<TEdgeType*> * VertexTEdgeList(VertexType * pVertex);
			/*! Vertex->HalfFace List */
			static std::list<HalfFaceType*> * VertexHalfFaceList(VertexType * pVertex);
			/*! Vertex->TVertex List */
			static std::list<TVertexType*> * VertexTVertexList(VertexType * pVertex);

			/*! Vertex->Edge */
			static EdgeType   * VertexEdge(VertexType * v1, VertexType * v2);

			//Access TVertex data memebers
			static VertexType * TVertexVertex(TVertexType * pTVertex);
			static TetType * TVertexTet(TVertexType * pTVertex);
			static HalfEdgeType * TVertexHalfEdge(TVertexType * pTVertex);

			//Access TEdge data memebers
			static HalfEdgeType * TEdgeLeftHalfEdge(TEdgeType * pTEdge);
			static HalfEdgeType * TEdgeRightHalfEdge(TEdgeType * pTEdge);
			static EdgeType *		TEdgeEdge(TEdgeType * pTEdge);
			static TEdgeType *	TEdgeDualTEdge(TEdgeType * pTEdge);
			static TetType *		TEdgeTet(TEdgeType * pTEdge);


			//Access HalfEdge data members
			/*! HalfEdge->source vertex */
			static VertexType * HalfEdgeSource(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->target vertex */
			static VertexType * HalfEdgeTarget(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->source tvertex */
			static TVertexType * HalfEdgeTSource(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->target tvertex */
			static TVertexType * HalfEdgeTTarget(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->dual halfedge */
			static HalfEdgeType * HalfEdgeDual(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->next HalfEdge */
			static HalfEdgeType * HalfEdgeNext(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->prev HalfEdge */
			static HalfEdgeType * HalfEdgePrev(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->TEdge TEdge */
			static TEdgeType * HalfEdgeTEdge(HalfEdgeType * pHalfEdge);
			/*! HalfEdge->HalfFace */
			static HalfFaceType * HalfEdgeHalfFace(HalfEdgeType * pHalfEdge);

			//Access Edge data members
			/*! TEdge list of the edge */
			static std::list<TEdgeType*> * EdgeTEdgeList(EdgeType * pEdge);
			/*! Edge->Vertex1 */
			static VertexType* EdgeVertex1(EdgeType * pEdge);
			/*! Edge->Vertex2 */
			static VertexType* EdgeVertex2(EdgeType * pEdge);
			/*! length of the edge*/
			static double EdgeLength(EdgeType * pEdge);

			//Access HalfFace data memebers
			/*! HalfFace->HalfEdge */
			static HalfEdgeType* HalfFaceHalfEdge(HalfFaceType * pHalfFace);
			/*! HalfFace->face */
			static FaceType* HalfFaceFace(HalfFaceType * pHalfFace);
			/*! HalfFace->Tet */
			static TetType * HalfFaceTet(HalfFaceType * pHalfFace);
			/*! HalfFace->dual half face */
			static HalfFaceType * HalfFaceDual(HalfFaceType * pHalfFace);

			//Face
			/*! access the left half face of a face */
			static HalfFaceType * FaceLeftHalfFace(FaceType * pFace);
			/*! access the right half face of a face */
			static HalfFaceType * FaceRightHalfFace(FaceType * pFace);

			//Tetrahedron

			/*! access the j-th half edge of a tet */
			static HalfFaceType* TetHalfFace(TetType * pT, int j);
			/*! access the j-th tvertex of a tet */
			static TVertexType * TetTVertex(TetType * pT, int j);
			/*! access the j-th vertex of a tet */
			static VertexType * TetVertex(TetType * pT, int j);
		};

		/*------------------------------------------------------------------------------------------------
		Access Vertex data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::list<EdgeType*> * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexEdgeList(VertexType * pVertex)
		{
			return (std::list<EdgeType*>*) pVertex->edges();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::list<TEdgeType*> * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexTEdgeList(VertexType * pVertex)
		{
			return (std::list<TEdgeType*>*) pVertex->tedges();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::list<HalfFaceType*> * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexHalfFaceList(VertexType * pVertex)
		{
			return (std::list<HalfFaceType*>*) pVertex->HalfFaces();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::list<TVertexType*> * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexTVertexList(VertexType * pVertex)
		{
			return (std::list<TVertexType*>*) pVertex->tvertices();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexEdge(VertexType * v1, VertexType * v2)
		{
			std::list<CEdge*> * vEdgeList = VertexEdgeList(v1);

			for (std::list<CEdge*>::iterator titer = (*vEdgeList).begin(); titer != (*vEdgeList).end(); titer++)
			{
				CEdge * pE = *titer;

				CVertex * w1 = EdgeVertex1(pE);
				CVertex * w2 = EdgeVertex2(pE);

				if (w1 == v1 && w2 == v2)
				{
					return pE;
				}
				if (w1 == v2 && w2 == v1)
				{
					return pE;
				}
			}
			return NULL;
		}
		/*------------------------------------------------------------------------------------------------
		Access TVertex data members
		--------------------------------------------------------------------------------------------------*/
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexVertex(TVertexType * pTVertex)
		{
			return (VertexType*)pTVertex->vert();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexTet(TVertexType * pTVertex)
		{
			return (TetType*)pTVertex->tet();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexHalfEdge(TVertexType * pTVertex)
		{
			return (HalfEdgeType*)pTVertex->halfedge();
		}

		/*------------------------------------------------------------------------------------------------
		Access TEdge data members
		--------------------------------------------------------------------------------------------------*/
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeLeftHalfEdge(TEdgeType * pTEdge)
		{
			return (HalfEdgeType*)pTEdge->left();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeRightHalfEdge(TEdgeType * pTEdge)
		{
			return (HalfEdgeType*)pTEdge->right();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeEdge(TEdgeType * pTEdge)
		{
			return (EdgeType*)pTEdge->edge();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeDualTEdge(TEdgeType * pTEdge)
		{
			return (TEdgeType*)pTEdge->dual();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeTet(TEdgeType * pTEdge)
		{
			return (TetType*)pTEdge->tet();
		}

		/*------------------------------------------------------------------------------------------------
		Access HalfEdge data members
		--------------------------------------------------------------------------------------------------*/
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeSource(HalfEdgeType * pHalfEdge)
		{
			return (VertexType*)pHalfEdge->source();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTarget(HalfEdgeType * pHalfEdge)
		{
			return (VertexType*)pHalfEdge->target();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTSource(HalfEdgeType * pHalfEdge)
		{
			return (TVertexType*)pHalfEdge->tSource();
		}
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTTarget(HalfEdgeType * pHalfEdge)
		{
			return (TVertexType*)pHalfEdge->tTarget();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeDual(HalfEdgeType * pHalfEdge)
		{
			return (HalfEdgeType*)pHalfEdge->dual();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeNext(HalfEdgeType * pHalfEdge)
		{
			return (HalfEdgeType*)pHalfEdge->next();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgePrev(HalfEdgeType * pHalfEdge)
		{
			return (HalfEdgeType*)pHalfEdge->prev();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTEdge(HalfEdgeType * pHalfEdge)
		{
			return (TEdgeType*)pHalfEdge->tedge();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeHalfFace(HalfEdgeType * pHalfEdge)
		{
			return (HalfFaceType*)pHalfEdge->half_face();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::list<TEdgeType*> * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeTEdgeList(EdgeType * pEdge)
		{
			return (std::list<TEdgeType*>*) pEdge->edges();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeVertex1(EdgeType * pEdge)
		{
			return (VertexType*)pEdge->vertex1();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeVertex2(EdgeType * pEdge)
		{
			return (VertexType*)pEdge->vertex2();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline double TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeLength(EdgeType * pEdge)
		{
			VertexType * pV1 = EdgeVertex1(pEdge);
			VertexType * pV2 = EdgeVertex2(pEdge);

			return (pV1->position() - pV2->position()).norm();
		}
		/*------------------------------------------------------------------------------------------------
		Access HalfFace data members
		--------------------------------------------------------------------------------------------------*/
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceHalfEdge(HalfFaceType * pHalfFace)
		{
			return (HalfEdgeType*)pHalfFace->half_edge();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline FaceType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceFace(HalfFaceType * pHalfFace)
		{
			return (FaceType*)pHalfFace->face();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceTet(HalfFaceType * pHalfFace)
		{
			return (TetType*)pHalfFace->tet();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceDual(HalfFaceType * pHalfFace)
		{
			return (HalfFaceType*)pHalfFace->dual();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::FaceLeftHalfFace(FaceType * pFace)
		{
			return (HalfFaceType*)pFace->left();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::FaceRightHalfFace(FaceType * pFace)
		{
			return (HalfFaceType*)pFace->right();
		}

		/*------------------------------------------------------------------------------------------------
		Access Tetrahedron data members
		--------------------------------------------------------------------------------------------------*/
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetHalfFace(TetType * pT, int j)
		{
			return (HalfFaceType*)pT->half_face(j);
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetTVertex(TetType * pT, int j)
		{
			return (TVertexType*)pT->tvertex(j);
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType * TInterface<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetVertex(TetType * pT, int j)
		{
			return (VertexType*)pT->tvertex(j)->vert();
		}

	}
}