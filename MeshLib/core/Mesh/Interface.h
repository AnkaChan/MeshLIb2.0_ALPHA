 #pragma once

namespace MeshLib 
{
	/*!
	* \brief Interface, interface to acess data from geometric classes:
	* \tparam VertexType   vertex   class, derived from MeshLib::CVertex   class
	* \tparam EdgeType     edge     class, derived from MeshLib::EdgeType     class
	* \tparam FaceType     face     class, derived from MeshLib::FaceType     class
	* \tparam HalfEdgeType halfedge class, derived from MeshLib::HalfEdgeType class
	*
	*  This class is a static class, which requires no instantiation before using.
	*
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	class CInterface
	{
	public:
		// pointer to Vertices, Halfedges, Edges, Face and Solid
		typedef VertexType   * VertexPtr;
		typedef HalfEdgeType * HalfEdgePtr;
		typedef EdgeType     * EdgePtr;
		typedef FaceType     * FacePtr;
		CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>* MeshPtr;

		//is boundary
		/*! whether a vertex is on the boundary
		\param v the pointer to the vertex
		*/
		static bool    isBoundary(VertexPtr  v);
		/*! whether an edge is on the boundary
		\param e the pointer to the edge
		*/
		static bool    isBoundary(EdgePtr    e);
		/*! whether a halfedge is on the boundary
		\param he the pointer to the halfedge
		*/
		static bool    isBoundary(HalfEdgePtr  he);
		/*!
		The vertex id
		\param v the input vertex
		\return the vertex id.
		*/
		static int     vertexId(VertexPtr  v);
		/*!
		The face id
		\param f the input face
		\return the face id.
		*/
		static int     faceId(FacePtr  f);
		//access edge - edge key, vertex
		/*!
		Access an edge by its two end vertices
		\param v0 one vertex of the edge
		\param v1 the other vertex of the edge
		\return the edge connecting both v0 and v1, NULL if no such edge exists.
		*/
		static EdgePtr   vertexEdge(VertexPtr v0, VertexPtr v1);

		//access halfedge - halfedge key, vertex
		/*!
		Access a halfedge by its two end vertices
		\param v0 one vertex of the halfedge
		\param v1 the other vertex of the halfedge
		\return the halfedge connecting both v0 and v1, NULL if no such edge exists.
		*/

		static HalfEdgePtr   vertexHalfedge(VertexPtr v0, VertexPtr v1);
		/*!
		Access a halfedge by its target vertex, and attaching face.
		\param v target vertex
		\param f attaching face
		\return halfedge, whose target is v, attaching face is f. NULL if no such an halfedge exists.
		*/
		static HalfEdgePtr   corner(VertexPtr v, FacePtr f);

		//halfedge->face
		/*!
		The face a halfedge attaching to.
		\param he the input halfedge
		\return the face he attaches
		*/
		static FacePtr   halfedgeFace(HalfEdgePtr he);
		//halfedge->vertex
		/*!
		The target vertex of a halfedge.
		\param he the input halfedge.
		\return the target vertex of he.
		*/
		static VertexPtr halfedgeVertex(HalfEdgePtr he);
		//halfedge->vertex
		/*!
		The target vertex of a halfedge.
		\param he the input halfedge.
		\return the target vertex of he.
		*/
		static VertexPtr halfedgeTarget(HalfEdgePtr he);
		//halfedge->vertex
		/*!
		The source vertex of a halfedge.
		\param he the input halfedge.
		\return the source vertex of he.
		*/
		static VertexPtr halfedgeSource(HalfEdgePtr he);

		//halfedge->next
		/*!
		The next halfedge of a halfedge.
		\param he the input halfedge.
		\return the next halfedge of he.
		*/

		static HalfEdgePtr   halfedgeNext(HalfEdgePtr he);
		//halfedge->prev
		/*!
		The previous halfedge of a halfedge.
		\param he the input halfedge.
		\return the next halfedge of he.
		*/
		static HalfEdgePtr   halfedgePrev(HalfEdgePtr he);
		//halfedge->sym
		/*!
		The dual halfedge of a halfedge.
		\param he the input halfedge.
		\return the dual halfedge of he.
		*/
		static HalfEdgePtr   halfedgeSym(HalfEdgePtr he);
		//halfedge->edge
		/*!
		The edge of a halfedge.
		\param he the input halfedge.
		\return the edge of he.
		*/

		static EdgePtr       halfedgeEdge(HalfEdgePtr he);
		//v->halfedge
		/*!
		The halfedge targeting at a vertex.
		\param v the input vertex.
		\return the halfedge targeting at v, which is the most ccw in halfedge of v.
		*/
		static HalfEdgePtr   vertexHalfedge(VertexPtr v);
		//v->edges
		/*!
		The edge list attaching to the vertex v, such that v is the first vertex of the edge
		\param v the input vertex.
		\return the reference to the edge list
		*/
		static std::list<EdgePtr>    vertexEdges(VertexPtr v);

		//edge->vertex
		/*!
		The first vertex of an edge.
		\param e the input edge.
		\return the first vertex of e.
		*/
		static VertexPtr edgeVertex1(EdgePtr  e);
		/*!
		The second vertex of an edge.
		\param e the input edge.
		\return the second vertex of e.
		*/
		static VertexPtr edgeVertex2(EdgePtr  e);

		//edge->face
		/*!
		The first face attaching to an edge.
		\param e the input edge.
		\return the first face attaching to e.
		*/
		static FacePtr edgeFace1(EdgePtr  e);
		/*!
		The second face attaching to an edge.
		\param e the input edge.
		\return the second face attaching to e.
		*/
		static FacePtr edgeFace2(EdgePtr  e);

		//edge->halfedge
		/*!
		The halfedge attaching to an edge.
		\param e the input edge.
		\param id the index of the halfedge, either 0 or 1
		\return the halfedge[i] attaching to edge e.
		*/

		static HalfEdgePtr edgeHalfedge(EdgePtr  e);

		//face->halfedge
		/*!
		The first halfedge attaching to a face f.
		\param f the input face.
		\return the first halfedge attaching to f.
		*/

		static HalfEdgePtr faceHalfedge(FacePtr f);

		//Euler operations
		/*!
		The most Clw Out HalfEdge of a vertex
		\param v the input vertex.
		\return the most Clw Out HalfEdge of v.
		*/
		static HalfEdgePtr vertexMostClwOutHalfEdge(VertexPtr  v);
		/*!
		The next Ccw Out HalfEdge
		\param he the input halfedge .
		\return the next Ccw Out HalfEdge, sharing the same source of he.
		*/

		static HalfEdgePtr vertexNextCcwOutHalfEdge(HalfEdgePtr  he);

		/*!
		The most Ccw Out HalfEdge of a vertex
		\param v the input vertex.
		\return the most Ccw Out HalfEdge of v.
		*/
		static HalfEdgePtr vertexMostCcwOutHalfEdge(VertexPtr  v);
		/*!
		The next Clw Out HalfEdge
		\param he the input halfedge .
		\return the next Clw Out HalfEdge, sharing the same source of he.
		*/
		static HalfEdgePtr vertexNextClwOutHalfEdge(HalfEdgePtr  he);

		/*!
		The most Clw In HalfEdge of a vertex
		\param v the input vertex.
		\return the most Clw In HalfEdge of v.
		*/
		static HalfEdgePtr vertexMostClwInHalfEdge(VertexPtr  v);
		/*!
		The next Ccw In HalfEdge
		\param he the input halfedge .
		\return the next Ccw In HalfEdge, sharing the same target of he.
		*/
		static HalfEdgePtr vertexNextCcwInHalfEdge(HalfEdgePtr  he);

		/*!
		The most Ccw In HalfEdge of a vertex
		\param v the input vertex.
		\return the most Ccw In HalfEdge of v.
		*/
		static HalfEdgePtr vertexMostCcwInHalfEdge(VertexPtr  v);
		/*!
		The next Clw In HalfEdge
		\param he the input halfedge .
		\return the next Clw In HalfEdge, sharing the same target of he.
		*/
		static HalfEdgePtr vertexNextClwInHalfEdge(HalfEdgePtr  he);

		/*!
		The most Clw HalfEdge of a face
		\param face the input face.
		\return the most Clw HalfEdge of f.
		*/

		static HalfEdgePtr faceMostClwHalfEdge(FacePtr  face);
		/*!
		The most Ccw HalfEdge of a face
		\param face the input face.
		\return the most Ccw HalfEdge of f.
		*/
		static HalfEdgePtr faceMostCcwHalfEdge(FacePtr  face);
		/*!
		The next Ccw HalfEdge of a halfedge in a face
		\param he the input halfedge.
		\return the next Ccw HalfEdge of he in a face.
		*/
		static HalfEdgePtr faceNextCcwHalfEdge(HalfEdgePtr  he);
		/*!
		The next Clw HalfEdge of a halfedge in a face
		\param he the input halfedge.
		\return the next Clw HalfEdge of he in a face.
		*/
		static HalfEdgePtr faceNextClwHalfEdge(HalfEdgePtr  he);


		/*!
		Edge length
		\param e the input edge
		\return the length of the edge e
		*/
		static double edgeLength(EdgePtr e);

	};
	/*! whether a vertex is on the boundary
	\param v the pointer to the vertex
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::isBoundary(VertexPtr v)
	{
		return v->boundary();
	}
	/*! whether a edge is on the boundary
	\param e the pointer to the edge
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::isBoundary(EdgePtr e)
	{
		return (e->halfedge()->sym() == NULL);
	}
	/*! whether a halfedge is on the boundary
	\param he the pointer to the halfedge
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::isBoundary(HalfEdgePtr he)
	{
		return (he->he_sym() == NULL);
	}
	//access v->id
	/*!
	The vertex id
	\param v the input vertex
	\return the vertex id.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline int CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexId(VertexPtr v)
	{
		return v->id();
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline int CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::faceId(FacePtr f)
	{
		return f->id();
	}

	//access vertex->edge
	/*!
	Access an edge by its two end vertices
	\param v0 one vertex of the edge
	\param v1 the other vertex of the edge
	\return the edge connecting both v0 and v1, NULL if no such edge exists.
	*/
	//use the edge list associated with each vertex to locate the edge
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline EdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexEdge(VertexPtr v0, VertexPtr v1)
	{
		//VertexType * pV = (v0->id() < v1->id() )? v0: v1;
		std::list<HalfEdgePtr> & lhes0 = (std::list<HalfEdgePtr>&)v0->arhe();//vertexEdges(v0);
		std::list<HalfEdgePtr> & lhes1 = (std::list<HalfEdgePtr>&)v1->arhe();//vertexEdges(v1);

		for (HalfEdgePtr pH : lhes0)
		{
			if (pH->source() == v0 && pH->target() == v1) return (EdgePtr)pH->edge();
		}
		for (HalfEdgePtr pH : lhes1)
		{
			if (pH->source() == v1 && pH->target() == v0) return  (EdgePtr)pH->edge();
		}

		return NULL;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexHalfedge(VertexPtr v0, VertexPtr v1)
	{
		std::list<EdgePtr> & ledges0 = vertexEdges(v0);
		for (EdgePtr pH : ledges0)
		{
			if (pH->source() == v0 && pH->target() == v1) return pH;
		}
		return NULL;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::corner(VertexPtr v, FacePtr f)
	{
		HalfEdgePtr he = faceMostCcwHalfEdge(f);
		do {
			if (he->vertex() == v)
				return (HalfEdgePtr)he;
			he = faceNextCcwHalfEdge(he);
		} while (he != faceMostCcwHalfEdge(f));
		return NULL;
	}

	//access he->f
	/*!
	The face a halfedge attaching to.
	\param he the input halfedge
	\return the face he attaches
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline FaceType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeFace(HalfEdgePtr he)
	{
		return (FacePtr)he->face();
	}

	//access he->v
	/*!
	The target vertex of a halfedge.
	\param he the input halfedge.
	\return the target vertex of he.
	*/

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeVertex(HalfEdgePtr he)
	{
		return (VertexPtr)he->vertex();
	}

	//access he->v
	/*!
	The target vertex of a halfedge.
	\param he the input halfedge.
	\return the target vertex of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeTarget(HalfEdgePtr he)
	{
		return (VertexPtr)he->vertex();
	}

	//access he->v
	/*!
	The source vertex of a halfedge.
	\param he the input halfedge.
	\return the source vertex of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeSource(HalfEdgePtr he)
	{
		return (VertexPtr)he->he_prev()->vertex();
	}

	//access he->next
	/*!
	The next halfedge of a halfedge.
	\param he the input halfedge.
	\return the next halfedge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeNext(HalfEdgePtr he)
	{
		return (HalfEdgePtr)he->he_next();
	}

	//access he->prev
	/*!
	The previous halfedge of a halfedge.
	\param he the input halfedge.
	\return the next halfedge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgePrev(HalfEdgePtr he)
	{
		return (HalfEdgePtr)he->he_prev();
	}

	//access he->sym
	/*!
	The dual halfedge of a halfedge.
	\param he the input halfedge.
	\return the dual halfedge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeSym(HalfEdgePtr he)
	{
		return (HalfEdgePtr)he->he_sym();
	}

	//access he->edge
	/*!
	The edge of a halfedge.
	\param he the input halfedge.
	\return the edge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline EdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::halfedgeEdge(HalfEdgePtr  he)
	{
		return (EdgePtr)he->edge();
	}

	//access vertex->halfedge
	/*!
	The halfedge targeting at a vertex.
	\param v the input vertex.
	\return the halfedge targeting at v, which is the most ccw in halfedge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexHalfedge(VertexPtr v)
	{
		return (HalfEdgePtr)v->halfedge();
	}
	
	/*!
	Access the edge list of a vertex, {e} such that e->vertex1() == v
	\param v vertex
	\return the list of adjacent edges
	*/
	//return edges around vertex
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline std::list<EdgeType *> CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexEdges(VertexPtr v)
	{
		VertexType::CHalfEdgePtrList& lhe0 = v0->arhe();
		std::list<EdgePtr>  ledges;

		for (HalfEdgePtr pH : lhe0)
		{
			ledges.push_back((EdgePtr)pH->edge());
		}
		return ledges;
	}

	//access vertex->halfedge
	/*!
	The halfedge targeting at a vertex.
	\param v the input vertex.
	\return the halfedge targeting at v, which is the most ccw in halfedge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline VertexType* CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeVertex1(EdgePtr e)
	{
		assert(e->halfedge() != NULL);
		return (VertexType*)e->halfedge()->source();
	}

	/*!
	The second vertex of an edge.
	\param e the input edge.
	\return the first vertex of e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeVertex2(EdgePtr e)
	{
		assert(e->halfedge() != NULL);
		return (VertexType*)e->halfedge()->target();
	}

	/*!
	The first face attaching to an edge.
	\param e the input edge.
	\return the first face attaching to e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline FaceType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeFace1(EdgePtr e)
	{
		assert(e->halfedge() != NULL);
		return (FaceType*)e->halfedge()->face();
	}

	//access e->f
	/*!
	The second face attaching to an edge.
	\param e the input edge.
	\return the first face attaching to e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline FaceType* CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeFace2(EdgePtr e)
	{
		//assert( e->halfedge()->sym()!= NULL );
		if (e->halfedge()->sym() == NULL) return NULL
		else
		{
			return (FaceType*)e->halfedge()->sym()->face();
		}
	}

	//access he->f
	/*!
	The halfedge attaching to an edge.
	\param e the input edge.
	\return the halfedge attaching to edge e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeHalfedge(EdgePtr e)
	{
		return (HalfEdgeType*)e->halfedge();
	}

	//access f->he
	/*!
	The first halfedge attaching to a face f.
	\param f the input face.
	\return the first halfedge attaching to f.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::faceHalfedge(FacePtr f)
	{
		return (HalfEdgeType*)f->halfedge();
	}

	// above had been updated
	//Euler operation
	/*!
	The most Clw Out HalfEdge of a vertex
	\param v the input vertex.
	\return the most Clw Out HalfEdge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexMostClwOutHalfEdge(VertexPtr v)
	{
		return (HalfEdgeType*)v->most_clw_out_halfedge();
	}

	/*!
	The next Ccw Out HalfEdge
	\param v the input vertex.
	\return the next Ccw Out HalfEdge, sharing the same source of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexNextCcwOutHalfEdge(HalfEdgePtr he)
	{
		return (HalfEdgeType*)he->ccw_rotate_about_source();
	}

	/*!
	The next Ccw Out HalfEdge
	\param v the input vertex.
	\return the next Ccw Out HalfEdge, sharing the same source of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexMostCcwOutHalfEdge(VertexPtr v)
	{
		return (HalfEdgeType*)v->most_ccw_out_halfedge();
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexNextClwOutHalfEdge(HalfEdgePtr he)	
	{
		assert(he->he_sym() != NULL);
		return (HalfEdgeType*)he->clw_rotate_about_source();
	}

	/*!
	The most Clw In HalfEdge of a vertex
	\param v the input vertex.
	\return the most Clw In HalfEdge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexMostClwInHalfEdge(VertexPtr v)
	{
		return (HalfEdgeType*)v->most_clw_in_halfedge();
	}

	/*!
	The next Ccw In HalfEdge
	\param he the input halfedge .
	\return the next Ccw In HalfEdge, sharing the same target of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexNextCcwInHalfEdge(HalfEdgePtr he)
	{
		assert(he->he_sym() != NULL);
		return (HalfEdgeType*)he->ccw_rotate_about_target();
	}

	/*!
	The most Ccw In HalfEdge of a vertex
	\param v the input vertex.
	\return the most Clw In HalfEdge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexMostCcwInHalfEdge(VertexPtr   v)
	{
		return (HalfEdgeType*)v->most_ccw_in_halfedge();
	};

	/*!
	The next Clw In HalfEdge
	\param he the input halfedge .
	\return the next Clw In HalfEdge, sharing the same target of he.
	*/ 
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::vertexNextClwInHalfEdge(HalfEdgePtr he)
	{
		return (HalfEdgeType*)he->clw_rotate_about_target();
	}

	/*!
	The most Clw HalfEdge in a face
	\param face the input face.
	\return the most Clw HalfEdge in a face.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::faceMostClwHalfEdge(FacePtr face)
	{
		return (HalfEdgeType*)face->halfedge()->he_next();
	}

	/*!
	The most Ccw HalfEdge of a face
	\param face the input face.
	\return the most Ccw HalfEdge of f.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::faceMostCcwHalfEdge(FacePtr face)
	{
		return (HalfEdgeType*)face->halfedge();
	}

	/*!
	The next Ccw HalfEdge of a halfedge in a face
	\param he the input halfedge.
	\return the next Ccw HalfEdge of he in a face.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::faceNextCcwHalfEdge(HalfEdgePtr he)
	{
		return (HalfEdgeType*)he->he_next();
	}

	/*!
	The next Ccw HalfEdge of a halfedge in a face
	\param he the input halfedge.
	\return the next Ccw HalfEdge of he in a face.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::faceNextClwHalfEdge(HalfEdgePtr he)
	{
		return (HalfEdgeType*)he->he_next();
	}

	/*!
	Edge length
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline double CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>::edgeLength(EdgePtr e)
	{
		VertexType * v1 = edgeVertex1(e);
		VertexType * v2 = edgeVertex2(e);

		return (v1->point() - v2->point()).norm();
	}
}