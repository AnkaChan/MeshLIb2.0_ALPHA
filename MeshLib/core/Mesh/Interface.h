#pragma once

namespace MeshLib 
{
	template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
	class Interface
	{
		// pointer to Vertices, Halfedges, Edges, Face and Solid
		typedef CVertex   * tVertex;
		typedef CHalfEdge * tHalfEdge;
		typedef CEdge     * tEdge;
		typedef CFace     * tFace;

		//is boundary
		/*! whether a vertex is on the boundary
		\param v the pointer to the vertex
		*/
		static bool    isBoundary(tVertex  v);
		/*! whether an edge is on the boundary
		\param e the pointer to the edge
		*/
		static bool    isBoundary(tEdge    e);
		/*! whether a halfedge is on the boundary
		\param he the pointer to the halfedge
		*/
		static bool    isBoundary(tHalfEdge  he);

		//access edge - edge key, vertex
		/*!
		Access an edge by its two end vertices
		\param v0 one vertex of the edge
		\param v1 the other vertex of the edge
		\return the edge connecting both v0 and v1, NULL if no such edge exists.
		*/
		static tEdge   vertexEdge(tVertex v0, tVertex v1);

		//access halfedge - halfedge key, vertex
		/*!
		Access a halfedge by its two end vertices
		\param v0 one vertex of the halfedge
		\param v1 the other vertex of the halfedge
		\return the halfedge connecting both v0 and v1, NULL if no such edge exists.
		*/

		static tHalfEdge   vertexHalfedge(tVertex v0, tVertex v1);
		/*!
		Access a halfedge by its target vertex, and attaching face.
		\param v target vertex
		\param f attaching face
		\return halfedge, whose target is v, attaching face is f. NULL if no such an halfedge exists.
		*/
		tHalfEdge   corner(tVertex v, tFace f);

		//halfedge->face
		/*!
		The face a halfedge attaching to.
		\param he the input halfedge
		\return the face he attaches
		*/
		tFace   halfedgeFace(tHalfEdge he);
		//halfedge->vertex
		/*!
		The target vertex of a halfedge.
		\param he the input halfedge.
		\return the target vertex of he.
		*/
		tVertex halfedgeVertex(tHalfEdge he);
		//halfedge->vertex
		/*!
		The target vertex of a halfedge.
		\param he the input halfedge.
		\return the target vertex of he.
		*/
		tVertex halfedgeTarget(tHalfEdge he);
		//halfedge->vertex
		/*!
		The source vertex of a halfedge.
		\param he the input halfedge.
		\return the source vertex of he.
		*/
		tVertex halfedgeSource(tHalfEdge he);

		//halfedge->next
		/*!
		The next halfedge of a halfedge.
		\param he the input halfedge.
		\return the next halfedge of he.
		*/

		tHalfEdge   halfedgeNext(tHalfEdge he);
		//halfedge->prev
		/*!
		The previous halfedge of a halfedge.
		\param he the input halfedge.
		\return the next halfedge of he.
		*/
		tHalfEdge   halfedgePrev(tHalfEdge he);
		//halfedge->sym
		/*!
		The dual halfedge of a halfedge.
		\param he the input halfedge.
		\return the dual halfedge of he.
		*/
		tHalfEdge   halfedgeSym(tHalfEdge he);
		//halfedge->edge
		/*!
		The edge of a halfedge.
		\param he the input halfedge.
		\return the edge of he.
		*/

		tEdge       halfedgeEdge(tHalfEdge he);
		//v->halfedge
		/*!
		The halfedge targeting at a vertex.
		\param v the input vertex.
		\return the halfedge targeting at v, which is the most ccw in halfedge of v.
		*/
		tHalfEdge   vertexHalfedge(tVertex v);
		//v->edges
		/*!
		The edge list attaching to the vertex v, such that v is the first vertex of the edge
		\param v the input vertex.
		\return the reference to the edge list
		*/
		std::list<tEdge> &  vertexEdges(tVertex v);

		//edge->vertex
		/*!
		The first vertex of an edge.
		\param e the input edge.
		\return the first vertex of e.
		*/
		tVertex edgeVertex1(tEdge  e);
		/*!
		The second vertex of an edge.
		\param e the input edge.
		\return the second vertex of e.
		*/
		tVertex edgeVertex2(tEdge  e);

		//edge->face
		/*!
		The first face attaching to an edge.
		\param e the input edge.
		\return the first face attaching to e.
		*/
		tFace edgeFace1(tEdge  e);
		/*!
		The second face attaching to an edge.
		\param e the input edge.
		\return the second face attaching to e.
		*/
		tFace edgeFace2(tEdge  e);

		//edge->halfedge
		/*!
		The halfedge attaching to an edge.
		\param e the input edge.
		\param id the index of the halfedge, either 0 or 1
		\return the halfedge[i] attaching to edge e.
		*/

		tHalfEdge edgeHalfedge(tEdge  e, int id);

		//face->halfedge
		/*!
		The first halfedge attaching to a face f.
		\param f the input face.
		\return the first halfedge attaching to f.
		*/

		tHalfEdge faceHalfedge(tFace f);

		//Euler operations
		/*!
		The most Clw Out HalfEdge of a vertex
		\param v the input vertex.
		\return the most Clw Out HalfEdge of v.
		*/
		tHalfEdge vertexMostClwOutHalfEdge(tVertex  v);
		/*!
		The next Ccw Out HalfEdge
		\param he the input halfedge .
		\return the next Ccw Out HalfEdge, sharing the same source of he.
		*/

		tHalfEdge vertexNextCcwOutHalfEdge(tHalfEdge  he);

		/*!
		The most Ccw Out HalfEdge of a vertex
		\param v the input vertex.
		\return the most Ccw Out HalfEdge of v.
		*/
		tHalfEdge vertexMostCcwOutHalfEdge(tVertex  v);
		/*!
		The next Clw Out HalfEdge
		\param he the input halfedge .
		\return the next Clw Out HalfEdge, sharing the same source of he.
		*/
		tHalfEdge vertexNextClwOutHalfEdge(tHalfEdge  he);

		/*!
		The most Clw In HalfEdge of a vertex
		\param v the input vertex.
		\return the most Clw In HalfEdge of v.
		*/
		tHalfEdge vertexMostClwInHalfEdge(tVertex  v);
		/*!
		The next Ccw In HalfEdge
		\param he the input halfedge .
		\return the next Ccw In HalfEdge, sharing the same target of he.
		*/
		tHalfEdge vertexNextCcwInHalfEdge(tHalfEdge  he);

		/*!
		The most Ccw In HalfEdge of a vertex
		\param v the input vertex.
		\return the most Ccw In HalfEdge of v.
		*/
		tHalfEdge vertexMostCcwInHalfEdge(tVertex  v);
		/*!
		The next Clw In HalfEdge
		\param he the input halfedge .
		\return the next Clw In HalfEdge, sharing the same target of he.
		*/
		tHalfEdge vertexNextClwInHalfEdge(tHalfEdge  he);

		/*!
		The most Clw HalfEdge of a face
		\param face the input face.
		\return the most Clw HalfEdge of f.
		*/

		tHalfEdge faceMostClwHalfEdge(tFace  face);
		/*!
		The most Ccw HalfEdge of a face
		\param face the input face.
		\return the most Ccw HalfEdge of f.
		*/
		tHalfEdge faceMostCcwHalfEdge(tFace  face);
		/*!
		The next Ccw HalfEdge of a halfedge in a face
		\param he the input halfedge.
		\return the next Ccw HalfEdge of he in a face.
		*/
		tHalfEdge faceNextCcwHalfEdge(tHalfEdge  he);
		/*!
		The next Clw HalfEdge of a halfedge in a face
		\param he the input halfedge.
		\return the next Clw HalfEdge of he in a face.
		*/
		tHalfEdge faceNextClwHalfEdge(tHalfEdge  he);


		/*!
		Edge length
		\param e the input edge
		\return the length of the edge e
		*/
		double edgeLength(tEdge e);

	};
	template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
	inline bool Interface<CVertex, CEdge, CFace, CHalfEdge>::isBoundary(tVertex v)
	{
		return v->boundary();
	}
	template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
	inline bool Interface<CVertex, CEdge, CFace, CHalfEdge>::isBoundary(tEdge e)
	{
		return (e->halfedge()->sym() == NULL);
	}

	template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
	inline bool Interface<CVertex, CEdge, CFace, CHalfEdge>::isBoundary(tHalfEdge he)
	{
		return (he->he_sym() == NULL);
	}

	//access vertex->edge
	/*!
	Access an edge by its two end vertices
	\param v0 one vertex of the edge
	\param v1 the other vertex of the edge
	\return the edge connecting both v0 and v1, NULL if no such edge exists.
	*/
	//use the edge list associated with each vertex to locate the edge

	template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
	inline tEdge Interface<CVertex, CEdge, CFace, CHalfEdge>::vertexEdge(tVertex v0, tVertex v1)
	{
		CVertex * pV = (v0->id() < v1->id()) ? v0 : v1;
		std::list<CEdge*> & ledges = vertexEdges(pV);

		for (std::list<CEdge*>::iterator eiter = ledges.begin(); eiter != ledges.end(); eiter++)
		{
			CEdge * pE = *eiter;
			CHalfEdge * pH = edgeHalfedge(pE, 0);
			if (pH->source() == v0 && pH->target() == v1) return pE;
			if (pH->source() == v1 && pH->target() == v0) return pE;
		}
		return NULL;
	}

	template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
	inline tHalfEdge Interface<CVertex, CEdge, CFace, CHalfEdge>::vertexHalfedge(tVertex v0, tVertex v1)
	{
		CEdge * e = vertexEdge(v0, v1);
		assert(e != NULL);
		CHalfEdge * he = (CHalfEdge*)e->halfedge(0);
		if (he->vertex() == v1 && he->he_prev()->vertex() == v0) return he;
		he = (CHalfEdge*)e->halfedge(1);
		assert(he->vertex() == v1 && he->he_prev()->vertex() == v0);
		return he;
	}

}