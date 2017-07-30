/*!
*      \file BaseMesh.h
*      \brief Base Mesh Class for all types of Mesh Classes
*
*		This is the fundamental class for meshes
*	   \author Iron
*      \date 15/07/2017
*
*/

#ifndef _MESHLIB_BASE_MESH_H_
#define _MESHLIB_BASE_MESH_H_

#define MAX_LINE 2048

#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <array>

#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
#include "../Parser/StrUtil.h"
#include "Interface.h"

namespace MeshLib{
/*!
* \brief CBaseMesh, base class for all types of mesh classes
*
*  This is the fundamental class for meshes. It includes a list of vertices,
*  a list of edges, a list of faces. All the geometric objects are connected by pointers,
*  vertex, edge, face are connected by halfedges. The mesh class has file IO functionalities,
*  supporting .obj, .m and .off file formats. It offers Euler operators, each geometric primative
*  can access its neighbors freely. 
*  
* \tparam VertexType   vertex   class, derived from MeshLib::CVertex   class
* \tparam EdgeType     edge     class, derived from MeshLib::EdgeType     class
* \tparam FaceType     face     class, derived from MeshLib::FaceType     class
* \tparam HalfEdgeType halfedge class, derived from MeshLib::HalfEdgeType class
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class CBaseMesh : public CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>
{
public:
	// pointer to Vertices, Halfedges, Edges, Face and Solid
	typedef VertexType   * VertexPtr;
	typedef HalfEdgeType * HalfEdgePtr;
	typedef EdgeType     * EdgePtr;
	typedef FaceType     * FacePtr;

	//constructor and destructor
	/*!
	CBaseMesh constructor.
	*/
	CBaseMesh(){};
	/*!
	CBasemesh destructor
	*/
	~CBaseMesh();

	//copy operator
	/*!
	Copy operator
	*/
	void copy( CBaseMesh & mesh );//iron : just declare never realize

	//file io
	/*!
	Read an .obj file.
	\param filename the input .obj file name
	*/
	void read_obj(  const char * filename );
	/*!
	Write an .obj file.
	\param output the output .obj file name
	*/
	void write_obj( const char * output);

	/*!
	Read an .m file.
	\param input the input obj file name
	*/
	void read_m(  const char * input );
	/*!
	Write an .m file.
	\param output the output .m file name
	*/
	void write_m( const char * output);
	/*!
	Write an .g file.
	\param output the output .g file name
	*/
	void write_g( const char * output);
	
	/*!
	Read an .off file
	\param input the input .off filename
	*/
	void read_off( const char * input );
	/*!
	Write an .off file.
	\param output the output .off file name
	*/
	void write_off( const char * output);

	//number of vertices, faces, edges
	/*! number of vertices */
	int  numVertices();
	/*! number of edges */
	int  numEdges();
	/*! number of faces */
	int  numFaces();

	//acess vertex - id
	/*!
	Access a vertex by its id
	\param id the vertex id
	\return the vertex, whose ID equals to id. NULL, if there is no such a vertex.
	*/
	VertexPtr idVertex( int id );

	//access face - id
	/*!
	Access a face by its id
	\param id the face id
	\return the face, whose ID equals to id. NULL, if there is no such a face.
	*/
	FacePtr   idFace( int id );

	/*!
	List of the edges of the mesh.
	*/
	std::list<EdgePtr>   & edges()		{ return m_edges; };
	/*!
	List of the faces of the mesh.
	*/
	std::list<FacePtr>   & faces()		{ return m_faces; };
	/*!
	List of the vertices of the mesh.
	*/
	std::list<VertexPtr> & vertices()	{ return m_verts; };

/*
	bool with_uv() { return m_with_texture; };
	bool with_normal() { return m_with_normal; };
*/
protected:

  /*! list of edges */
  std::list<EdgePtr>                          m_edges;
  /*! list of vertices */
  std::list<VertexPtr>                        m_verts;
  /*! list of faces */
  std::list<FacePtr>						m_faces;


  //maps

  /*! map between vetex and its id*/
  std::map<int, VertexPtr>                    m_map_vert;
  /*! map between face and its id*/
  std::map<int, FacePtr>						m_map_face;  
public:
	/*! Create a vertex 
	\param id Vertex id
	\return pointer to the new vertex
	*/
	VertexPtr   createVertex(   int id = 0 );
	/*! Create an edge
	\param v1 end vertex of the edge
	\param v2 end vertex of the edge
	\return pointer to the new edge
	*/
	EdgePtr     createEdge(VertexPtr v1, VertexPtr v2 );
	/*
	return halfedge from v1 to v2
	*/
	HalfEdgePtr query_hedge(VertexPtr v1, VertexPtr v2);
	/*
	enter halfedge on vextex v1
	*/
	void	    enter_hedge(HalfEdgePtr he, VertexPtr v1);
	/*! Create a face
	\param v an array of vertices
	\param id face id
	\return pointer to the new face
	//create a triangle*/	
	FacePtr     createFace(   VertexPtr  v[], int id ); 
	/*! Create a face
	\param v an array of vertices
	\param id face id
	\return pointer to the new face
	*/
	FacePtr     createFace(   std::vector<VertexPtr> &  v, int id ); //create a triangle
	/*
	remove halfedge from vertex v1
	*/
	void	    remove_hedge(HalfEdgePtr he, VertexPtr v1);
	/*! delete one face
	\param pFace the face to be deleted
	*/
	void        deleteFace( FacePtr  f );
	/*! label boundary vertices, edges, faces */
	void	    labelBoundary(void);

	/*! whether the vertex is with texture coordinates */
	bool        m_with_texture;
	/*! whether the mesh is with normal */
	bool        m_with_normal;  
 public:
	 /*!
	  *   the input traits of the mesh, there are 64 bits in total
	  */
	 static unsigned long long m_input_traits;
	 /*!
	  *	 the output triats of the mesh, there are 64 bits in total
	  */
	 static unsigned long long m_output_traits;
};

/*!
CBaseMesh destructor
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::~CBaseMesh()
{
	//remove vertices

	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexType * pV = *viter;
		delete pV;
	}
	m_verts.clear();

	//remove faces
	//any problem? put by iron he thought it unwisely
	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FaceType * pF = *fiter;

		HalfEdgePtr he = faceHalfedge(pF);

		std::list<HalfEdgeType*> hes;
		do {
			he = halfedgeNext(he);
			hes.push_back(he);
		} while (he != pF->halfedge());

		for (std::list<HalfEdgeType*>::iterator hiter = hes.begin(); hiter != hes.end(); hiter++)
		{
			HalfEdgeType * pH = *hiter;
			delete pH;
		}
		hes.clear();

		delete pF;
	}
	m_faces.clear();

	//remove edges
	for (std::list<EdgeType*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); eiter++)
	{
		EdgeType * pE = *eiter;
		delete pE;
	}

	m_edges.clear();

	//clear all the maps
	m_map_vert.clear();
	m_map_face.clear();
	//m_map_edge.clear();
};

/*!
Read an .obj file.
\param filename the filename .obj file name
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::read_obj(const char * filename)
{

	std::fstream f(filename, std::fstream::in);
	if (f.fail()) return;

	char cmd[1024];

	int  vid = 1;
	int  fid = 1;

	bool with_uv = false;
	bool with_normal = false;

	std::vector<CPoint2> uvs;
	std::vector<CPoint> normals;


	while (f.getline(cmd, 1024))
	{
		std::string line(cmd);
		line = strutil::trim(line);

		strutil::Tokenizer stokenizer(line, " \t\r\n");

		stokenizer.nextToken();
		std::string token = stokenizer.getToken();

		if (token == "v")
		{
			CPoint p;
			for (int i = 0; i < 3; i++)
			{
				stokenizer.nextToken();
				token = stokenizer.getToken();
				p[i] = strutil::parseString<float>(token);
			}

			VertexType * v = createVertex(vid);
			v->point() = p;
			vid++;
			continue;
		}


		if (token == "vt")
		{
			with_uv = true;
			CPoint2 uv;
			for (int i = 0; i < 2; i++)
			{
				stokenizer.nextToken();
				token = stokenizer.getToken();
				uv[i] = strutil::parseString<float>(token);
			}
			uvs.push_back(uv);
			continue;
		}


		if (token == "vn")
		{
			with_normal = true;

			CPoint n;
			for (int i = 0; i < 3; i++)
			{
				stokenizer.nextToken();
				token = stokenizer.getToken();
				n[i] = strutil::parseString<float>(token);
			}
			normals.push_back(n);
			continue;
		}




		if (token == "f")
		{
			VertexType* v[3];
			for (int i = 0; i < 3; i++)
			{
				stokenizer.nextToken();
				token = stokenizer.getToken();


				strutil::Tokenizer tokenizer(token, " /\t\r\n");

				int ids[3];
				int k = 0;
				while (tokenizer.nextToken())
				{
					std::string token = tokenizer.getToken();
					ids[k] = strutil::parseString<int>(token);
					k++;
				}


				v[i] = m_map_vert[ids[0]];
				if (with_uv)
					v[i]->uv() = uvs[ids[1] - 1];
				if (with_normal)
					v[i]->normal() = normals[ids[2] - 1];
			}
			createFace(v, fid++);
		}
	}

	f.close();

	labelBoundary();
}

//assume the mesh is with uv coordinates and normal vector for each vertex
/*!
Write an .obj file.
\param output the output .obj file name
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::write_obj(const char * output)
{
	std::fstream _os(output, std::fstream::out);
	if (_os.fail())
	{
		fprintf(stderr, "Error is opening file %s\n", output);
		return;
	}

	int vid = 1;
	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;
		v->id() = vid++;
	}

	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;

		_os << "v";

		for (int i = 0; i < 3; i++)
		{
			_os << " " << v->point()[i];
		}
		_os << std::endl;
	}

	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;

		_os << "vt";

		for (int i = 0; i < 2; i++)
		{
			_os << " " << v->uv()[i];
		}
		_os << std::endl;
	}

	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;

		_os << "vn";

		for (int i = 0; i < 3; i++)
		{
			_os << " " << v->normal()[i];
		}
		_os << std::endl;
	}


	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FacePtr f = *fiter;

		_os << "f";

		HalfEdgePtr he = faceHalfedge(f);

		do {
			int vid = he->target()->id();
			_os << " " << vid << "/" << vid << "/" << vid;
			he = halfedgeNext(he);
		} while (he != f->halfedge());
		_os << std::endl;
	}

	_os.close();
};

/*!
Access the edge list of a vertex, {e} such that e->vertex1() == v
\param v vertex
\return the list of adjacent edges
*/

/*!
Read an .m file.
\param input the input obj file name
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::read_m(const char * input)
{
	std::fstream is(input, std::fstream::in);

	if (is.fail())
	{
		fprintf(stderr, "Error in opening file %s\n", input);
		return;
	}

	char buffer[MAX_LINE];
	int id;

	while (is.getline(buffer, MAX_LINE))
	{

		std::string line(buffer);
		line = strutil::trim(line);

		strutil::Tokenizer stokenizer(line, " \r\n");

		stokenizer.nextToken();
		std::string token = stokenizer.getToken();

		if (token == "Vertex")
		{
			stokenizer.nextToken();
			token = stokenizer.getToken();
			id = strutil::parseString<int>(token);

			CPoint p;
			for (int i = 0; i < 3; i++)
			{
				stokenizer.nextToken();
				token = stokenizer.getToken();
				p[i] = strutil::parseString<float>(token);
			}

			VertexPtr v = createVertex(id);
			v->point() = p;
			v->id() = id;

			if (!stokenizer.nextToken("\t\r\n")) continue;
			token = stokenizer.getToken();

			int sp = (int)token.find("{");
			int ep = (int)token.find("}");

			if (sp >= 0 && ep >= 0)
			{
				v->string() = token.substr(sp + 1, ep - sp - 1);
			}
			continue;
		}


		if (token == "Face")
		{

			stokenizer.nextToken();
			token = stokenizer.getToken();
			id = strutil::parseString<int>(token);

			std::vector<VertexType*> v;

			//assume each face is a triangle
			/*
			for( int i = 0;i < 3; i ++ )
			{
			stokenizer.nextToken();
			token = stokenizer.getToken();
			int vid = strutil::parseString<int>(token);
			v.push_back( idVertex( vid ) );
			}
			*/

			while (stokenizer.nextToken())
			{
				token = stokenizer.getToken();
				if (strutil::startsWith(token, "{")) break;
				int vid = strutil::parseString<int>(token);
				v.push_back(idVertex(vid));
			}

			FacePtr f = createFace(v, id);

			if (!stokenizer.nextToken("\t\r\n")) continue;
			token = stokenizer.getToken();

			//stokenizer.reset();
			token = line;
			int sp = (int)token.find("{");
			int ep = (int)token.find("}");

			if (sp >= 0 && ep >= 0)
			{
				f->string() = token.substr(sp + 1, ep - sp - 1);
			}
			/*
			if( strutil::startsWith( token, "{" ) )
			{

			f->string() = strutil::trim( token, "{}" );
			}
			*/
			continue;
		}

		//read in edge attributes
		if (token == "Edge")
		{
			stokenizer.nextToken();
			token = stokenizer.getToken();
			int id0 = strutil::parseString<int>(token);

			stokenizer.nextToken();
			token = stokenizer.getToken();
			int id1 = strutil::parseString<int>(token);


			VertexType * v0 = idVertex(id0);
			VertexType * v1 = idVertex(id1);

			EdgePtr edge = vertexEdge(v0, v1);

			if (!stokenizer.nextToken("\t\r\n")) continue;
			token = stokenizer.getToken();

			int sp = (int)token.find("{");
			int ep = (int)token.find("}");

			if (sp >= 0 && ep >= 0)
			{
				edge->string() = token.substr(sp + 1, ep - sp - 1);
			}
			continue;
		}

		//read in edge attributes
		if (token == "Corner")
		{
			stokenizer.nextToken();
			token = stokenizer.getToken();
			int vid = strutil::parseString<int>(token);

			stokenizer.nextToken();
			token = stokenizer.getToken();
			int fid = strutil::parseString<int>(token);


			VertexType * v = idVertex(vid);
			FaceType   * f = idFace(fid);
			HalfEdgePtr he = corner(v, f);


			if (!stokenizer.nextToken("\t\r\n")) continue;
			token = stokenizer.getToken();

			int sp = (int)token.find("{");
			int ep = (int)token.find("}");

			if (sp >= 0 && ep >= 0)
			{
				he->string() = token.substr(sp + 1, ep - sp - 1);
			}
			continue;
		}
	}

	labelBoundary();//为啥写了不用 by iron

					//read in the traits
	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); ++viter)
	{
		VertexType *     v = *viter;
		v->_from_string();
	}

	for (std::list<EdgeType*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); ++eiter)
	{
		EdgeType *     e = *eiter;
		e->_from_string();
	}

	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); ++fiter)
	{
		FaceType *     f = *fiter;
		f->_from_string();
	}

	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FaceType * pF = *fiter;

		HalfEdgeType * pH = faceMostCcwHalfEdge(pF);
		do {
			pH->_from_string();
			pH = faceNextCcwHalfEdge(pH);
		} while (pH != faceMostCcwHalfEdge(pF));
	}

};

/*!
Write an .m file.
\param output the output .m file name
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::write_m(const char * output)
{
	//write traits to string
	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexType * pV = *viter;
		pV->_to_string();
	}

	for (std::list<EdgeType*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); eiter++)
	{
		EdgeType * pE = *eiter;
		pE->_to_string();
	}

	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FaceType * pF = *fiter;
		pF->_to_string();
	}

	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FaceType * pF = *fiter;
		HalfEdgeType * pH = faceMostCcwHalfEdge(pF);
		do {
			pH->_to_string();
			pH = faceNextCcwHalfEdge(pH);
		} while (pH != faceMostCcwHalfEdge(pF));
	}


	std::fstream _os(output, std::fstream::out);
	if (_os.fail())
	{
		fprintf(stderr, "Error is opening file %s\n", output);
		return;
	}


	//remove vertices
	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;

		_os << "Vertex " << v->id();

		for (int i = 0; i < 3; i++)
		{
			_os << " " << v->point()[i];
		}
		if (v->string().size() > 0)
		{
			_os << " " << "{" << v->string() << "}";
		}
		_os << std::endl;
	}

	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FacePtr f = *fiter;

		_os << "Face " << f->id();
		HalfEdgePtr he = faceHalfedge(f);
		do {
			_os << " " << he->target()->id();
			he = halfedgeNext(he);
		} while (he != f->halfedge());

		if (f->string().size() > 0)
		{
			_os << " " << "{" << f->string() << "}";
		}
		_os << std::endl;
	}

	for (std::list<EdgeType*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); eiter++)
	{
		EdgePtr e = *eiter;
		if (e->string().size() > 0)
		{
			_os << "Edge " << edgeVertex1(e)->id() << " " << edgeVertex2(e)->id() << " ";
			_os << "{" << e->string() << "}" << std::endl;
		}
	}

	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FacePtr f = *fiter;

		HalfEdgePtr he = faceHalfedge(f);

		do {
			if (he->string().size() > 0)
			{
				_os << "Corner " << he->vertex()->id() << " " << f->id() << " ";
				_os << "{" << he->string() << "}" << std::endl;
			}
			he = halfedgeNext(he);
		} while (he != f->halfedge());

	}

	_os.close();
};

/*!
Write an .g file.
\param output the output .g file name
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::write_g(const char * output)
{

	std::fstream _os(output, std::fstream::out);
	if (_os.fail())
	{
		fprintf(stderr, "Error is opening file %s\n", output);
		return;
	}


	//remove vertices
	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;

		_os << "Vertex " << v->id();

		for (int i = 0; i < 3; i++)
		{
			_os << " " << v->point()[i];
		}
		if (v->string().size() > 0)
		{
			_os << " " << "{" << v->string() << "}";
		}
		_os << std::endl;
	}

	for (std::list<EdgeType*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); eiter++)
	{
		EdgePtr e = *eiter;

		_os << "Edge " << e->id() << " ";
		VertexPtr v1 = edgeVertex1(e);
		VertexPtr v2 = edgeVertex2(e);
		_os << v1->id() << " " << v2->id() << std::endl;
	}
	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FacePtr f = *fiter;

		_os << "Face " << f->id();
		HalfEdgePtr he = faceHalfedge(f);
		do {
			_os << " " << he->edge()->id();
			he = halfedgeNext(he);
		} while (he != f->halfedge());

		if (f->string().size() > 0)
		{
			_os << " " << "{" << f->string() << "}";
		}
		_os << std::endl;
	}

	_os.close();
};

/*!
Read an .off file
\param input the input .off filename
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::read_off(const char * input)
{
	std::fstream is(input, std::fstream::in);

	if (is.fail())
	{
		fprintf(stderr, "Error is opening file %s\n", input);
		return;
	}

	char buffer[MAX_LINE];

	//read in the first line "OFF"

	while (is.getline(buffer, MAX_LINE))
	{
		std::string line(buffer);

		strutil::Tokenizer stokenizer(line, " \r\n");

		stokenizer.nextToken();
		std::string token = stokenizer.getToken();
		if (token == "OFF" || token == "NOFF") break;
	}

	int nVertices, nFaces, nEdges;

	//read in Vertex Number, Face Number, Edge Number

	is.getline(buffer, MAX_LINE);
	std::string line(buffer);

	strutil::Tokenizer stokenizer(line, " \r\n");

	stokenizer.nextToken();
	std::string token = stokenizer.getToken();
	nVertices = strutil::parseString<int>(token);

	stokenizer.nextToken();
	token = stokenizer.getToken();
	nFaces = strutil::parseString<int>(token);

	stokenizer.nextToken();
	token = stokenizer.getToken();
	nEdges = strutil::parseString<int>(token);

	//printf("V %d F %d E %d\n" , nVertices, nFaces, nEdges);


	for (int id = 0; id < nVertices; id++)
	{
		is.getline(buffer, MAX_LINE);
		std::string line(buffer);

		strutil::Tokenizer stokenizer(line, " \r\n");
		CPoint p;
		for (int j = 0; j < 3; j++)
		{
			stokenizer.nextToken();
			std::string token = stokenizer.getToken();
			p[j] = strutil::parseString<float>(token);
		}

		VertexType * v = createVertex(id + 1);
		v->point() = p;
	}


	for (int id = 0; id < nFaces; id++)
	{

		is.getline(buffer, MAX_LINE);
		std::string line(buffer);

		strutil::Tokenizer stokenizer(line, " \r\n");
		stokenizer.nextToken();
		std::string token = stokenizer.getToken();

		int n = strutil::parseString<int>(token);
		assert(n == 3);

		VertexType * v[3];
		for (int j = 0; j < 3; j++)
		{
			stokenizer.nextToken();
			std::string token = stokenizer.getToken();
			int vid = strutil::parseString<int>(token);
			v[j] = idVertex(vid + 1);
		}

		createFace(v, id + 1);
	}

	is.close();

	labelBoundary();

};
/*!
Write an .off file.
\param output the output .off file name
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::write_off(const char * output)
{
	std::fstream _os(output, std::fstream::out);
	if (_os.fail())
	{
		fprintf(stderr, "Error is opening file %s\n", output);
		return;
	}

	_os << "OFF" << std::endl;
	_os << m_verts.size() << " " << m_faces.size() << " " << m_edges.size() << std::endl;


	int vid = 0;
	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;
		v->id() = vid++;
	}

	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter++)
	{
		VertexPtr v = *viter;
		_os << v->point()[0] << " " << v->point()[1] << " " << v->point()[2] << std::endl;
		//_os << v->normal()[0] << " " << v->normal()[1]<< " " << v->normal()[2]<< std::endl;
	}


	for (std::list<FaceType*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter++)
	{
		FacePtr f = *fiter;

		_os << "3";

		HalfEdgePtr he = faceHalfedge(f);

		do {
			int vid = he->target()->id();
			_os << " " << vid;
			he = halfedgeNext(he);
		} while (he != f->halfedge());
		_os << std::endl;
	}

	_os.close();
};

/*! Number of vertices of the mesh
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
inline int CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::numVertices()
{
	return (int)m_verts.size();
};

/*! Number of edges of the mesh
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
inline int CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::numEdges()
{
	return (int)m_edges.size();
};

/*! Number of faces of the mesh
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
inline int CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::numFaces()
{
	return (int)m_faces.size();
};

//access id->v
/*!
Access a vertex by its id
\param id the vertex id
\return the vertex, whose ID equals to id. NULL, if there is no such a vertex.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
VertexType * CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::idVertex(int id)
{
	return m_map_vert[id];
};

//access id->f
/*!
Access a face by its id
\param id the face id
\return the face, whose ID equals to id. NULL, if there is no such a face.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
FaceType * CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::idFace(int id)
{
	return m_map_face[id];
};

//create new gemetric simplexes
/*! Create a vertex
\param id Vertex id
\return pointer to the new vertex
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
VertexType * CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::createVertex(int id)
{
	VertexType * v = new VertexType();
	assert(v != NULL);
	v->id() = id;
	m_verts.push_back(v);
	m_map_vert.insert(std::pair<int, VertexType*>(id, v));
	return v;
};

/*! Create an edge
\param v1 end vertex of the edge
\param v2 end vertex of the edge
\return pointer to the new edge
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
EdgeType * CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::createEdge(VertexPtr  v1, VertexPtr  v2)
{

	std::cout << "we didn't need a createEdge" << std::endl;
	system("pause");
	//there is no need to have this function really
	//there is no need to introduce edgelist on every vertex

	//VertexPtr pV = ( v1->id()<v2->id())?v1:v2;
	//std::list<EdgeType*> & ledges = (std::list<EdgeType*> &) pV->edges();

	//
	//for( std::list<EdgeType*>::iterator te = ledges.begin(); te != ledges.end(); te ++ )
	//{
	//	EdgeType	  * pE = *te;
	//	HalfEdgeType * pH = (HalfEdgeType*) pE->halfedge(0);
	//
	//	if( pH->source() == v1 && pH->target() == v2 ) 
	//	{
	//		return pE;		
	//	}
	//	if( pH->source() == v2 && pH->target() == v1 )
	//	{
	//		return pE;
	//	}
	//}

	////new edge
	//EdgeType * e = new EdgeType;
	//assert( e != NULL );
	//m_edges.push_back( e );
	//e->id() = (int)m_edges.size();
	//ledges.push_back( e );


	//return e;

};

/*!
Find halfedge from v1 to v2
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
HalfEdgeType* CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::query_hedge(VertexPtr v1, VertexPtr v2)
{
	for (VertexType::CHalfEdgePtrList::iterator heiter = v1->arhe().begin(); heiter != v1->arhe().end(); ++heiter)
	{
		HalfEdgePtr he= (HalfEdgePtr)*heiter;
		if ((VertexPtr)he->vertex() == v2) return he;
	}
	//return nullptr;//
	return NULL;
};


template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::enter_hedge(HalfEdgePtr he, VertexPtr v1)
{
	VertexPtr v2 = (VertexPtr) he->vertex();
	/*if (sdebug >= 1) {
	valid(v1); valid(v2);
	assertx(!query_hedge(v1, v2) && !v1->_arhe.contains(he));
	}*/
	v1->arhe().push_back(he);
	he->he_sym() = query_hedge(v2, v1);
	//HalfEdgePtr hes = (HalfEdgePtr)he->he_sym() = query_hedge(v2, v1);
	HalfEdgePtr hes = (HalfEdgePtr)he->he_sym();
	if (hes) {
		//assertx(!hes->he_sym());
		hes->he_sym() = he;
		EdgePtr e =(EdgePtr) hes->edge();
		he->edge() = e;
		if (he->vertex()->id()>hes->vertex()->id()) e->halfedge() = he;
	}
	else {                                           //hes不存在 说明v1v2之间没有边 需new一个边
		EdgePtr e = new EdgeType();
		e->halfedge() = he;
		he->edge() = e;
		m_edges.push_back(e);
	}
}

/*! Create a face
\param v an array of vertices
\param id face id
\return pointer to the new face
*/
//i did find nowhere we use this createfece so i didnt update it .
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
FaceType * CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::createFace(VertexPtr  v[], int id)
{
	FacePtr f = new FaceType();
	assert(f != NULL);
	f->id() = id;
	m_faces.push_back(f);
	m_map_face.insert(std::pair<int, FacePtr>(id, f));
	HalfEdgePtr hep = NULL;
	//int nv = va.num();
	int nv = 3;
	for (int i = 0; i < nv; i++) {//i=0							//先new he123 和点连起来 并视情况new e123 最后调整he123的值
		VertexPtr v2 = v[i + 1 == nv ? 0 : i + 1];
		HalfEdgePtr he = new HalfEdgeType;
		he->he_prev() = hep;
		// he->next is set below
		he->vertex() = v2;
		he->vertex()->halfedge() = he;
		enter_hedge(he, v[i]);
		// he->sym and he->edge were set in enter_hedge
		he->face() = f;
		hep = he;
	}
	//assertx(hep);               // HH_ASSUME(hep);
	HalfEdgePtr helast = hep;  //此时 he123的next皆为空 he1的prev是null为终止条件 依次为21next1prev3next赋值
	for (;;) {
		HalfEdgePtr hepp = (HalfEdgePtr)hep->he_prev();
		if (!hepp) break;
		hepp->he_next() = hep;
		hep = hepp;
	}
	hep->he_prev() = helast;
	helast->he_next() = hep;
	f->halfedge() = helast;          // such that f->herep->vert==va[0]  //f->herep is he3 指向v0 
									 //_facenum = max(_facenum, id + 1);
									 //if (sdebug >= 3) ok();
	return f;//此时he123的sys被处理为null 或取已有值


};

/*! Create a face
\param v an array of vertices
\param id face id
\return pointer to the new face
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
FaceType * CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::createFace(std::vector<VertexPtr> &  v, int id)
{
	FacePtr f = new FaceType();
	f->id() = id;
	m_faces.push_back(f);
	m_map_face.insert(std::pair<int, FacePtr>(id, f));

	HalfEdgePtr hep = NULL;
	int nv = v.size();
	for (int i = 0; i < nv; i++) {
		VertexPtr v2 = v[i + 1 == nv ? 0 : i + 1];
		HalfEdgePtr he = new HalfEdgeType;
		he->he_prev() = (CHalfEdge*)hep;//子类的值可以赋给父类 反过来 ？ 不知道 忘了
		// he->next is set below
		he->vertex() = v2;
		//One incoming halfedge of the vertex , but always re assign each vertex many times
		//is assignment copy cost large time?
		he->vertex()->halfedge() = he;
		enter_hedge(he, v[i]);
		// he->sym and he->edge were set in enter_hedge
		he->face() = f;
		hep = he;
	}
	HalfEdgePtr helast = hep;
	for (;;) {
		HalfEdgePtr hepp = (HalfEdgePtr)hep->he_prev();
		if (!hepp) break;
		hepp->he_next() = hep;
		hep = hepp;
	}
	hep->he_prev() = helast;
	helast->he_next() = hep;
	f->halfedge() = helast;          // such that f->herep->vert==va[0]  //f->herep is he3 point to v0 
	return f;

};

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::remove_hedge(HalfEdgePtr he, VertexPtr v1)
{
	VertexPtr v2 = he->vertex();
	he->he_sym() == query_hedge(v2, v1);
	EdgePtr e = he->edge();
	HalfEdgePtr hes = he->he_sym();
	if (hes) {
		hes->sym() = nullptr;
		if (e->halfedge() == he) e->halfedge() = hes;
	}
	else {
		m_edges.remove(e);
		// e->herep = nullptr;     // optional
		delete e;
	}
	// he->edge = nullptr;         // optional
	v1->arhe().remove(he); // slow, shucks

};
//template pointer converting to base class pointer is OK (BasePointer) = (TemplatePointer)
//(TemplatePointer)=(BasePointer) is incorrect
/*! delete one face
\param pFace the face to be deleted
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::deleteFace(FacePtr  f)
{
	std::map<int, FacePtr>::iterator fiter = m_map_face.find(f->id());
	if (fiter != m_map_face.end())
	{
		m_map_face.erase(fiter);
	}
	m_faces.remove(f);

	HalfEdgePtr he = f->halfedge(), hef = he;
	VertexPtr v1 = he->prev()->vert();
	for (;;)
	{
		HalfEdgePtr hen = he->next();
		VertexPtr v1n = he->vertex();
		remove_hedge(he, v1);
		delete he; he = hen;
		v1 = v1n;
		if (he == hef) break;
	}

	delete f;
};

/*!
Label boundary edges, vertices
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>::labelBoundary()
{

	//Label boundary edges
	for (std::list<EdgeType*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); ++eiter)
	{
		EdgeType *     edge = *eiter;
		HalfEdgeType * he = (HalfEdgePtr)edge->halfedge();

		assert(he != NULL);
		if (he->he_sym() == NULL)
		{
			//edge->boundary() = true;
			he->target()->boundary() = true;
			he->source()->boundary() = true;

		}
	}

	//may be there is no need to do so      by iron.
	//Arrange the boundary half_edge of boundary vertices, to make its halfedge
	//to be the most ccw in half_edge

	for (std::list<VertexType*>::iterator viter = m_verts.begin(); viter != m_verts.end(); ++viter)
	{
		VertexType *     v = *viter;
		if (!v->boundary()) continue;

		HalfEdgeType * he = vertexMostCcwInHalfEdge(v);
		//it has been done in function vertexMostCcwInHalfEdge .
		/*while( he->he_sym() != NULL )
		{
		he =  vertexNextCcwInHalfEdge ( he );
		}*/
		v->halfedge() = he;
	}

};

}//name space MeshLib

#endif //_MESHLIB_BASE_MESH_H_ defined
