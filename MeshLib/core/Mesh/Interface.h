#pragma once

namespace MeshLib
{
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	class CBaseMesh;
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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	class CInterface
	{
	public:
		//AnkaChan: Don't look. Maybe confusing but necessary.
		typedef VertexType	  VType;
		typedef HalfEdgeType  HEType;
		typedef EdgeType	  EType;
		typedef FaceType	  FType;

		// pointer to Vertices, Halfedges, Edges, Face and Solid
		typedef VertexType   * VPtr  ;
		typedef HalfEdgeType * HEPtr;
		typedef EdgeType     * EPtr	  ;
		typedef FaceType     * FPtr	  ;
		typedef MeshTemplate<VertexType, EdgeType, FaceType, HalfEdgeType> MeshType;
		typedef MeshType* MeshPtr;//need include base mesh£¿but when i need myMesh?

		//is boundary
		/*! whether a vertex is on the boundary
		\param v the pointer to the vertex
		*/
		static bool    isBoundary(VPtr  v);
		/*! whether an edge is on the boundary
		\param e the pointer to the edge
		*/
		static bool    isBoundary(EPtr    e);
		/*! whether a halfedge is on the boundary
		\param he the pointer to the halfedge
		*/
		static bool    isBoundary(HEPtr  he);
		/*!
		The vertex id
		\param v the input vertex
		\return the vertex id.
		*/
		static int     vertexId(VPtr  v);
		/*!
		The face id
		\param f the input face
		\return the face id.
		*/
		static int     faceId(FPtr  f);
		//access edge - edge key, vertex
		/*!
		Access an edge by its two end vertices
		\param v0 one vertex of the edge
		\param v1 the other vertex of the edge
		\return the edge connecting both v0 and v1, NULL if no such edge exists.
		*/
		static EPtr   vertexEdge(VPtr v0, VPtr v1);

		//access halfedge - halfedge key, vertex
		/*!
		Access a halfedge by its two end vertices
		\param v0 one vertex of the halfedge
		\param v1 the other vertex of the halfedge
		\return the halfedge connecting both v0 and v1, NULL if no such edge exists.
		*/

		static HEPtr   vertexHalfedge(VPtr v0, VPtr v1);
		/*!
		Access a halfedge by its target vertex, and attaching face.
		\param v target vertex
		\param f attaching face
		\return halfedge, whose target is v, attaching face is f. NULL if no such an halfedge exists.
		*/
		static HEPtr   corner(VPtr v, FPtr f);

		//halfedge->face
		/*!
		The face a halfedge attaching to.
		\param he the input halfedge
		\return the face he attaches
		*/
		static FPtr   halfedgeFace(HEPtr he);
		//halfedge->vertex
		/*!
		The target vertex of a halfedge.
		\param he the input halfedge.
		\return the target vertex of he.
		*/
		static VPtr halfedgeVertex(HEPtr he);
		//halfedge->vertex
		/*!
		The target vertex of a halfedge.
		\param he the input halfedge.
		\return the target vertex of he.
		*/
		static VPtr halfedgeTarget(HEPtr he);
		//halfedge->vertex
		/*!
		The source vertex of a halfedge.
		\param he the input halfedge.
		\return the source vertex of he.
		*/
		static VPtr halfedgeSource(HEPtr he);

		//halfedge->next
		/*!
		The next halfedge of a halfedge.
		\param he the input halfedge.
		\return the next halfedge of he.
		*/

		static HEPtr   halfedgeNext(HEPtr he);
		//halfedge->prev
		/*!
		The previous halfedge of a halfedge.
		\param he the input halfedge.
		\return the next halfedge of he.
		*/
		static HEPtr   halfedgePrev(HEPtr he);
		//halfedge->sym
		/*!
		The dual halfedge of a halfedge.
		\param he the input halfedge.
		\return the dual halfedge of he.
		*/
		static HEPtr   halfedgeSym(HEPtr he);
		//halfedge->edge
		/*!
		The edge of a halfedge.
		\param he the input halfedge.
		\return the edge of he.
		*/

		static EPtr       halfedgeEdge(HEPtr he);
		//v->halfedge
		/*!
		The halfedge targeting at a vertex.
		\param v the input vertex.
		\return the halfedge targeting at v, which is the most ccw in halfedge of v.
		*/
		static HEPtr   vertexHalfedge(VPtr v);
		//v->edges
		/*!
		The edge list attaching to the vertex v, such that v is the first vertex of the edge
		\param v the input vertex.
		\return the reference to the edge list
		*/
		static std::list<EPtr>    vertexEdges(VPtr v);

		//edge->vertex
		/*!
		The first vertex of an edge.
		\param e the input edge.
		\return the first vertex of e.
		*/
		static VPtr edgeVertex1(EPtr  e);
		/*!
		The second vertex of an edge.
		\param e the input edge.
		\return the second vertex of e.
		*/
		static VPtr edgeVertex2(EPtr  e);

		//edge->face
		/*!
		The first face attaching to an edge.
		\param e the input edge.
		\return the first face attaching to e.
		*/
		static FPtr edgeFace1(EPtr  e);
		/*!
		The second face attaching to an edge.
		\param e the input edge.
		\return the second face attaching to e.
		*/
		static FPtr edgeFace2(EPtr  e);

		//edge->halfedge
		/*!
		The halfedge attaching to an edge.
		\param e the input edge.
		\param id the index of the halfedge, either 0 or 1
		\return the halfedge[i] attaching to edge e.
		*/
		static HEPtr edgeHalfedge(EPtr	e, int	id);

		//edge->halfedge
		/*!
		The halfedge attaching to an edge.
		\param e the input edge.
		\return the halfedge attaching to edge e.
		*/
		static HEPtr edgeHalfedge(EPtr  e);

		//face->halfedge
		/*!
		The first halfedge attaching to a face f.
		\param f the input face.
		\return the first halfedge attaching to f.
		*/

		static HEPtr faceHalfedge(FPtr f);

		//Euler operations
		/*!
		The most Clw Out HalfEdge of a vertex
		\param v the input vertex.
		\return the most Clw Out HalfEdge of v.
		*/
		static HEPtr vertexMostClwOutHalfEdge(VPtr  v);
		/*!
		The next Ccw Out HalfEdge
		\param he the input halfedge .
		\return the next Ccw Out HalfEdge, sharing the same source of he.
		*/

		static HEPtr vertexNextCcwOutHalfEdge(HEPtr  he);

		/*!
		The most Ccw Out HalfEdge of a vertex
		\param v the input vertex.
		\return the most Ccw Out HalfEdge of v.
		*/
		static HEPtr vertexMostCcwOutHalfEdge(VPtr  v);
		/*!
		The next Clw Out HalfEdge
		\param he the input halfedge .
		\return the next Clw Out HalfEdge, sharing the same source of he.
		*/
		static HEPtr vertexNextClwOutHalfEdge(HEPtr  he);

		/*!
		The most Clw In HalfEdge of a vertex
		\param v the input vertex.
		\return the most Clw In HalfEdge of v.
		*/
		static HEPtr vertexMostClwInHalfEdge(VPtr  v);
		/*!
		The next Ccw In HalfEdge
		\param he the input halfedge .
		\return the next Ccw In HalfEdge, sharing the same target of he.
		*/
		static HEPtr vertexNextCcwInHalfEdge(HEPtr  he);

		/*!
		The most Ccw In HalfEdge of a vertex
		\param v the input vertex.
		\return the most Ccw In HalfEdge of v.
		*/
		static HEPtr vertexMostCcwInHalfEdge(VPtr  v);
		/*!
		The next Clw In HalfEdge
		\param he the input halfedge .
		\return the next Clw In HalfEdge, sharing the same target of he.
		*/
		static HEPtr vertexNextClwInHalfEdge(HEPtr  he);

		/*!
		The most Clw HalfEdge of a face
		\param face the input face.
		\return the most Clw HalfEdge of f.
		*/

		static HEPtr faceMostClwHalfEdge(FPtr  face);
		/*!
		The most Ccw HalfEdge of a face
		\param face the input face.
		\return the most Ccw HalfEdge of f.
		*/
		static HEPtr faceMostCcwHalfEdge(FPtr  face);
		/*!
		The next Ccw HalfEdge of a halfedge in a face
		\param he the input halfedge.
		\return the next Ccw HalfEdge of he in a face.
		*/
		static HEPtr faceNextCcwHalfEdge(HEPtr  he);
		/*!
		The next Clw HalfEdge of a halfedge in a face
		\param he the input halfedge.
		\return the next Clw HalfEdge of he in a face.
		*/
		static HEPtr faceNextClwHalfEdge(HEPtr  he);


		/*!
		Edge length
		\param e the input edge
		\return the length of the edge e
		*/
		static double edgeLength(EPtr e);

	};
	/*! whether a vertex is on the boundary
	\param v the pointer to the vertex
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline bool CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::isBoundary(VPtr v)
	{
		return v->boundary();
	}
	/*! whether a edge is on the boundary
	\param e the pointer to the edge
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline bool CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::isBoundary(EPtr e)
	{
		return (e->halfedge()->sym() == NULL);
	}
	/*! whether a halfedge is on the boundary
	\param he the pointer to the halfedge
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline bool CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::isBoundary(HEPtr he)
	{
		return (he->he_sym() == NULL);
	}
	//access v->id
	/*!
	The vertex id
	\param v the input vertex
	\return the vertex id.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline int CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexId(VPtr v)
	{
		return v->id();
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline int CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::faceId(FPtr f)
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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline EdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexEdge(VPtr v0, VPtr v1)
	{
		//VertexType * pV = (v0->id() < v1->id() )? v0: v1;
		std::list<HEPtr> & lhes0 = (std::list<HEPtr>&)v0->arhe();//vertexEdges(v0);
		std::list<HEPtr> & lhes1 = (std::list<HEPtr>&)v1->arhe();//vertexEdges(v1);

		for (HEPtr pH : lhes0)
		{
			if (pH->source() == v0 && pH->target() == v1) return (EPtr)pH->edge();
		}
		for (HEPtr pH : lhes1)
		{
			if (pH->source() == v1 && pH->target() == v0) return  (EPtr)pH->edge();
		}

		return NULL;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexHalfedge(VPtr v0, VPtr v1)
	{
		std::list<EPtr> & ledges0 = vertexEdges(v0);
		for (EPtr pH : ledges0)
		{
			if (pH->source() == v0 && pH->target() == v1) return pH;
		}
		return NULL;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::corner(VPtr v, FPtr f)
	{
		HEPtr he = faceMostCcwHalfEdge(f);
		do {
			if (he->vertex() == v)
				return (HEPtr)he;
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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline FaceType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeFace(HEPtr he)
	{
		return (FPtr)he->face();
	}

	//access he->v
	/*!
	The target vertex of a halfedge.
	\param he the input halfedge.
	\return the target vertex of he.
	*/

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeVertex(HEPtr he)
	{
		return (VPtr)he->vertex();
	}

	//access he->v
	/*!
	The target vertex of a halfedge.
	\param he the input halfedge.
	\return the target vertex of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeTarget(HEPtr he)
	{
		return (VPtr)he->vertex();
	}

	//access he->v
	/*!
	The source vertex of a halfedge.
	\param he the input halfedge.
	\return the source vertex of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeSource(HEPtr he)
	{
		return (VPtr)he->he_prev()->vertex();
	}

	//access he->next
	/*!
	The next halfedge of a halfedge.
	\param he the input halfedge.
	\return the next halfedge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeNext(HEPtr he)
	{
		return (HEPtr)he->he_next();
	}

	//access he->prev
	/*!
	The previous halfedge of a halfedge.
	\param he the input halfedge.
	\return the next halfedge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgePrev(HEPtr he)
	{
		return (HEPtr)he->he_prev();
	}

	//access he->sym
	/*!
	The dual halfedge of a halfedge.
	\param he the input halfedge.
	\return the dual halfedge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeSym(HEPtr he)
	{
		return (HEPtr)he->he_sym();
	}

	//access he->edge
	/*!
	The edge of a halfedge.
	\param he the input halfedge.
	\return the edge of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline EdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::halfedgeEdge(HEPtr  he)
	{
		return (EPtr)he->edge();
	}

	//access vertex->halfedge
	/*!
	The halfedge targeting at a vertex.
	\param v the input vertex.
	\return the halfedge targeting at v, which is the most ccw in halfedge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexHalfedge(VPtr v)
	{
		return (HEPtr)v->halfedge();
	}
	
	/*!
	Access the edge list of a vertex, {e} such that e->vertex1() == v
	\param v vertex
	\return the list of adjacent edges
	*/
	//return edges around vertex
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline std::list<EdgeType *> CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexEdges(VPtr v)
	{
		VertexType::CHEPtrList& lhe0 = v0->arhe();
		std::list<EPtr>  ledges;

		for (HEPtr pH : lhe0)
		{
			ledges.push_back((EPtr)pH->edge());
		}
		return ledges;
	}

	//access vertex->halfedge
	/*!
	The halfedge targeting at a vertex.
	\param v the input vertex.
	\return the halfedge targeting at v, which is the most ccw in halfedge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline VertexType* CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeVertex1(EPtr e)
	{
		assert(e->halfedge() != NULL);
		return (VertexType*)e->halfedge()->source();
	}

	/*!
	The second vertex of an edge.
	\param e the input edge.
	\return the first vertex of e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline VertexType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeVertex2(EPtr e)
	{
		assert(e->halfedge() != NULL);
		return (VertexType*)e->halfedge()->target();
	}

	/*!
	The first face attaching to an edge.
	\param e the input edge.
	\return the first face attaching to e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline FaceType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeFace1(EPtr e)
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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline FaceType* CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeFace2(EPtr e)
	{
		//assert( e->halfedge()->sym()!= NULL );
		if (e->halfedge()->sym() == NULL) return NULL
		else
		{
			return (FaceType*)e->halfedge()->sym()->face();
		}
	}

	//edge->halfedge
	/*!
	The halfedge attaching to an edge.
	\param e the input edge.
	\param id the index of the halfedge, either 0 or 1
	\return the halfedge[i] attaching to edge e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeHalfedge(EPtr e, int id)
	{
		return (HalfEdgeType*)(id == 0 ? e->halfedge() : e->halfedge()->he_sym());
	}

	//access he->f
	/*!
	The halfedge attaching to an edge.
	\param e the input edge.
	\return the halfedge attaching to edge e.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeHalfedge(EPtr e)
	{
		return (HalfEdgeType*)e->halfedge();
	}

	//access f->he
	/*!
	The first halfedge attaching to a face f.
	\param f the input face.
	\return the first halfedge attaching to f.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::faceHalfedge(FPtr f)
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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexMostClwOutHalfEdge(VPtr v)
	{
		return (HalfEdgeType*)v->most_clw_out_halfedge();
	}

	/*!
	The next Ccw Out HalfEdge
	\param v the input vertex.
	\return the next Ccw Out HalfEdge, sharing the same source of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexNextCcwOutHalfEdge(HEPtr he)
	{
		return (HalfEdgeType*)he->ccw_rotate_about_source();
	}

	/*!
	The next Ccw Out HalfEdge
	\param v the input vertex.
	\return the next Ccw Out HalfEdge, sharing the same source of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexMostCcwOutHalfEdge(VPtr v)
	{
		return (HalfEdgeType*)v->most_ccw_out_halfedge();
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexNextClwOutHalfEdge(HEPtr he)	
	{
		assert(he->he_sym() != NULL);
		return (HalfEdgeType*)he->clw_rotate_about_source();
	}

	/*!
	The most Clw In HalfEdge of a vertex
	\param v the input vertex.
	\return the most Clw In HalfEdge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexMostClwInHalfEdge(VPtr v)
	{
		return (HalfEdgeType*)v->most_clw_in_halfedge();
	}

	/*!
	The next Ccw In HalfEdge
	\param he the input halfedge .
	\return the next Ccw In HalfEdge, sharing the same target of he.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexNextCcwInHalfEdge(HEPtr he)
	{
		assert(he->he_sym() != NULL);
		return (HalfEdgeType*)he->ccw_rotate_about_target();
	}

	/*!
	The most Ccw In HalfEdge of a vertex
	\param v the input vertex.
	\return the most Clw In HalfEdge of v.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexMostCcwInHalfEdge(VPtr   v)
	{
		return (HalfEdgeType*)v->most_ccw_in_halfedge();
	};

	/*!
	The next Clw In HalfEdge
	\param he the input halfedge .
	\return the next Clw In HalfEdge, sharing the same target of he.
	*/ 
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::vertexNextClwInHalfEdge(HEPtr he)
	{
		return (HalfEdgeType*)he->clw_rotate_about_target();
	}

	/*!
	The most Clw HalfEdge in a face
	\param face the input face.
	\return the most Clw HalfEdge in a face.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::faceMostClwHalfEdge(FPtr face)
	{
		return (HalfEdgeType*)face->halfedge()->he_next();
	}

	/*!
	The most Ccw HalfEdge of a face
	\param face the input face.
	\return the most Ccw HalfEdge of f.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::faceMostCcwHalfEdge(FPtr face)
	{
		return (HalfEdgeType*)face->halfedge();
	}

	/*!
	The next Ccw HalfEdge of a halfedge in a face
	\param he the input halfedge.
	\return the next Ccw HalfEdge of he in a face.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::faceNextCcwHalfEdge(HEPtr he)
	{
		return (HalfEdgeType*)he->he_next();
	}

	/*!
	The next Ccw HalfEdge of a halfedge in a face
	\param he the input halfedge.
	\return the next Ccw HalfEdge of he in a face.
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline HalfEdgeType * CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::faceNextClwHalfEdge(HEPtr he)
	{
		return (HalfEdgeType*)he->he_next();
	}

	/*!
	Edge length
	*/
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType, template<typename, typename, typename, typename> typename MeshTemplate = CBaseMesh>
	inline double CInterface<VertexType, EdgeType, FaceType, HalfEdgeType, MeshTemplate>::edgeLength(EPtr e)
	{
		VertexType * v1 = edgeVertex1(e);
		VertexType * v2 = edgeVertex2(e);

		return (v1->point() - v2->point()).norm();
	}
}