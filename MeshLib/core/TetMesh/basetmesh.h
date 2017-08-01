/*!
*      \file BaseTetMesh.h
*      \brief Base TetMesh Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for tetrahedral meshes
*	   \author David Gu, Jerome Jiang
*      \date 10/01/2011
*	   \modified 4/16/2015
*
*/

#ifndef _TMESHLIB_BASE_TET_MESH_H_
#define _TMESHLIB_BASE_TET_MESH_H_

#include <assert.h>
#include <list>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>

#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
#include "../Parser/StrUtil_fast.h"
#include "TInterface.h"

#ifndef MAX_LINE 
#define MAX_LINE 2048
#endif

namespace MeshLib
{
	namespace TMeshLib
	{

		/*!
		* \brief CBaseTMesh, base class for all types of tet-mesh classes
		*
		*  This is the fundamental class for tet-meshes. All the geometric objects are connected by pointers,
		*  vertex, edge, face, tet are connected by halffaces. The mesh class has file IO functionalities,
		*  supporting .tet file formats. It offers Euler operators, each geometric primative
		*  can access its neighbors freely.
		*
		* \tparam VertexType   vertex   class, derived from TMeshLib::VertexType     class
		* \tparam TVertexType  tetrahedron vertex   class, derived from TMeshLib::TVertexType   class
		* \tparam HalfEdgeType halfedge class, derived from TMeshLib::HalfEdgeType class
		* \tparam TEdgeType	tetrahedron edge class, derived from TMeshLib::TEdgeType class
		* \tparam EdgeType     edge     class, derived from MeshLib::EdgeType     class
		* \tparam FaceType     face     class, derived from TMeshLib::FaceType     class
		* \tparam HalfFaceType half face     class, derived from TMeshLib::HalfFaceType     class
		* \tparam TetType      tetrahedron class, derived from TMeshLib::TetType class
		*/

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		class CTMesh : public TInterface<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		{
		public:
			/*!
				CTMesh constructor
				*/
			CTMesh() { std::ios::sync_with_stdio(false); };
			/*!
				CTMesh desctructor
				*/
			~CTMesh() { _clear(); };
			/*!
				Load tet mesh from a ".tet" file
				*/
			void _load(const char * input, bool checkOrientation = false);
			/*!
				Load tet mesh from a ".t" file
				*/
			void _load_t(const char * input, bool checkOrientation = false);
			/*!
				Write tet mesh to a file
				*/
			void _write(const char *);

			/*!
			Write tet mesh to a .t file
			*/
			void _write_t(const char * filename);

			/*!
				access the list of half faces
				*/
			std::list<HalfFaceType*> & half_faces() { return m_pHalfFaces; };
			/*!
				access the list of edges
				*/
			std::list<EdgeType*>       & edges() { return m_pEdges; };
			/*!
				access list of faces
				*/
			std::list<FaceType*>        & faces() { return m_pFaces; };

			/*!
			access list of vertices
			*/
			std::list<VertexType*> & vertices() { return m_pVertices; };

			/*! number of tets */
			int numTets() { return m_nTets; };

			/*! number of vertices */
			int numVertices() { return m_nVertices; };

			/*! max vertex id*/
			int maxVertexId() { return m_maxVertexId; };

			/*! Access the array of tets */
			std::list<TetType*> & tets() { return m_pTets; };

			/*! access the vertex with ID */
			VertexType * idVertex(int id) { return m_map_Vertices[id]; };

			/*! access the tet with ID */
			TetType      * idTet(int id) { return m_map_Tets[id]; };

		protected:

			/*!
			construct tetrahedron
			\tparam v array of vertex ids
			\tparam pT retulting tetrahedron
			*/

			void  _construct_tet(TetType* pT, int tID, int * v);
			void  _construct_tet_orientation(TetType* pT, int tId, int  v[4]);
			/*! construct faces */
			void  _construct_faces();
			/*! construct edges */
			void  _construct_edges();
			/*!  construct half face
			\tparam array of vertex pointers
			*/
			HalfFaceType*   _construct_half_face(TVertexType **);

			/*! release all the memory allocations */
			void _clear();

		protected:
			/*!
			list of faces
			*/
			std::list<FaceType*>        m_pFaces;
			/*!
			list of half faces
			*/
			std::list<HalfFaceType*>	 m_pHalfFaces;
			/*!
			list of half edges
			*/
			std::list<HalfEdgeType*>	 m_pHalfEdges;
			/*!
			list of edges
			*/
			std::list<EdgeType*>        m_pEdges;
			/*!
			list of tetrahedra
			*/
			std::list<TEdgeType*>		 m_pTEdges;

			/*!
			 array of vertices
			 */
			std::list<VertexType*>		 m_pVertices;
			//VertexType *				 m_pVertices;

			/*!
			map of VertexType id and pointer
			*/
			std::map<int, VertexType *> m_map_Vertices;

			/*!
			array of tets
			*/
			std::list<TetType*>		 m_pTets;
			//TetType*                    m_pTets;

			std::map<int, TetType*>     m_map_Tets;

			/*! number of vertices */
			int m_nVertices;

			/*! number of tets */
			int m_nTets;

			/*! number of edges */
			int m_nEdges;

			/*! max vertex id */
			int m_maxVertexId;

		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_clear()
		{
			for (std::list<EdgeType*>::iterator eit = m_pEdges.begin(); eit != m_pEdges.end(); eit++)
			{
				EdgeType * pE = *eit;
				delete pE;
			}

			for (std::list<TEdgeType*>::iterator eit = m_pTEdges.begin(); eit != m_pTEdges.end(); eit++)
			{
				TEdgeType * pE = *eit;
				delete pE;
			}

			for (std::list<HalfEdgeType*>::iterator hit = m_pHalfEdges.begin(); hit != m_pHalfEdges.end(); hit++)
			{
				HalfEdgeType * pH = *hit;
				delete pH;
			}


			for (std::list<FaceType*>::iterator fit = m_pFaces.begin(); fit != m_pFaces.end(); fit++)
			{
				FaceType * pF = *fit;
				delete pF;
			}

			for (std::list<HalfFaceType*>::iterator fit = m_pHalfFaces.begin(); fit != m_pHalfFaces.end(); fit++)
			{
				HalfFaceType * pF = *fit;
				delete pF;
			}

			m_pVertices.clear();
			m_pTets.clear();
			//delete[] m_pTVertices;
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load(const char * input, bool checkOrientation = false_)
		{
			m_maxVertexId = 0;

			std::fstream is(input, std::fstream::in);

			if (is.fail())
			{
				fprintf(stderr, "Error in opening file %s\n", input);
				return;
			}

			char buffer[MAX_LINE];
			{
				//read in the first line
				is.getline(buffer, MAX_LINE);
				//read in the number of vertices: "number vertices"
				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");
				stokenizer.nextToken();
				std::string token = stokenizer.getToken();
				m_nVertices = strutil::parseString<int>(token);
				stokenizer.nextToken();
				token = stokenizer.getToken();
				if (token != "vertices")
				{
					fprintf(stderr, "Error in file format %s\n", input);
					return;
				}
			}

			//read in the second line
			is.getline(buffer, MAX_LINE);
			//read in the number of vertices: "number tets"
			std::string line(buffer);
			line = strutil::trim(line);
			strutil::Tokenizer stokenizer(line, " \r\n");
			stokenizer.nextToken();
			std::string token = stokenizer.getToken();
			m_nTets = strutil::parseString<int>(token);
			stokenizer.nextToken();
			token = stokenizer.getToken();
			if (token != "tets")
			{
				fprintf(stderr, "Error in file format %s\n", input);
				return;
			}

			//read in the vertices
			for (int i = 0; i < m_nVertices && is.getline(buffer, MAX_LINE); i++)
			{
				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				CPoint p;
				for (int k = 0; k < 3; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					p[k] = strutil::parseString<float>(token);
				}

				VertexType * v = new VertexType();
				//VertexType & v = m_pVertices[i];
				v->id() = i;
				v->position() = p;

				m_pVertices.push_back(v);
				m_map_Vertices.insert(std::pair<int, VertexType*>(i, v));

				if (!stokenizer.nextToken("\t\r\n")) continue;
				std::string token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					v->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}


			//read in tets 
			for (int id = 0; id < m_nTets && is.getline(buffer, MAX_LINE); id++)
			{
				int vId[4];

				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				//skip the first "4" in the line
				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				for (int k = 0; k < 4; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					vId[k] = strutil::parseString<int>(token);
				}

				TetType * pT = new TetType();
				m_pTets.push_back(pT);
				m_map_Tets.insert(std::pair<int, TetType *>(id, pT));

				if (checkOrientation) {
					_construct_tet_orientation(pT, tid, vId);
				}
				else {
					_construct_tet(pT, tid, vId);
				}
			}

			_construct_faces();
			_construct_edges();

			is.close();

			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (std::list<FaceType*>::iterator fiter = m_pFaces.begin(); fiter != m_pFaces.end(); ++fiter)
			{
				FaceType * pF = *fiter;

				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					HalfFaceType * pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);

					for (int i = 0; i < 3; ++i)
					{
						int vid = pH->key(i);
						VertexType * v = idVertex(vid);
						v->boundary() = true;
					}
				}
			}
		};


		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load_t(const char * input, bool checkOrientation = false)
		{
			m_maxVertexId = 0;

			std::fstream is(input, std::fstream::in);

			if (is.fail())
			{
				fprintf(stderr, "Error in opening file %s\n", input);
				return;
			}

			char buffer[MAX_LINE];

			m_nVertices = 0;
			m_nTets = 0;
			m_nEdges = 0;

			while (!is.eof())
			{
				is.getline(buffer, MAX_LINE);
				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token == "Vertex") m_nVertices++;
				if (token == "Tet") m_nTets++;
				if (token == "Edge") m_nEdges++;
			}

			is.clear();              // forget we hit the end of file
			is.seekg(0, std::ios::beg);   // move to the start of the file

			//read in the vertices
			for (int i = 0; i < m_nVertices && is.getline(buffer, MAX_LINE); i++)
			{
				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Vertex")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}


				stokenizer.nextToken();
				token = stokenizer.getToken();
				int vid = strutil::parseString<int>(token);

				CPoint p;
				for (int k = 0; k < 3; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					p[k] = strutil::parseString<float>(token);
				}

				VertexType * v = new VertexType();
				//VertexType & v = m_pVertices[i];
				v->id() = vid;
				v->position() = p;

				m_pVertices.push_back(v);

				m_map_Vertices.insert(std::pair<int, VertexType *>(vid, v));

				if (!stokenizer.nextToken("\t\r\n")) continue;
				token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					v->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}


			//read in tets 
			for (int id = 0; id < m_nTets && is.getline(buffer, MAX_LINE); id++)
			{
				int vId[4];

				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Tet")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}

				//skip the first "4" in the line
				stokenizer.nextToken();
				token = stokenizer.getToken();
				int tid = strutil::parseString<int>(token);


				for (int k = 0; k < 4; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					vId[k] = strutil::parseString<int>(token);
				}



				TetType * pT = new TetType();
				m_pTets.push_back(pT);
				m_map_Tets.insert(std::pair<int, TetType *>(tid, pT));

				if (checkOrientation) {
					_construct_tet_orientation(pT, tid, vId);
				}
				else {
					_construct_tet(pT, tid, vId);
				}
				// read in string
				if (!stokenizer.nextToken("\t\r\n")) continue;
				token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					pT->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}

			_construct_faces();
			_construct_edges();

			for (int id = 0; id < m_nEdges && is.getline(buffer, MAX_LINE); id++)
			{
				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Edge")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}

				stokenizer.nextToken();
				token = stokenizer.getToken();
				int id1 = strutil::parseString<int>(token);

				stokenizer.nextToken();
				token = stokenizer.getToken();
				int id2 = strutil::parseString<int>(token);

				VertexType * pV1 = idVertex(id1);
				VertexType * pV2 = idVertex(id2);

				EdgeType * pE = VertexEdge(pV1, pV2);

				if (!stokenizer.nextToken("\t\r\n"))
				{
					continue;
				}

				token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					pE->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}

			m_nEdges = (int)m_pEdges.size();

			is.close();

			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (std::list<FaceType*>::iterator fiter = m_pFaces.begin(); fiter != m_pFaces.end(); ++fiter)
			{
				FaceType * pF = *fiter;

				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					HalfFaceType * pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);

					for (int i = 0; i < 3; ++i)
					{
						int vid = pH->key(i);
						VertexType * v = idVertex(vid);
						v->boundary() = true;
					}
				}
			}

			// read in traits
			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				pV->_from_string();
			}

			for (std::list<TetType *>::iterator tIter = m_pTets.begin(); tIter != m_pTets.end(); tIter++)
			{
				TetType * pT = *tIter;
				pT->_from_string();
			}

			for (std::list<EdgeType*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				pE->_from_string();
			}

		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_half_face(TVertexType ** pTV)
		{
			HalfFaceType * pHF = new HalfFaceType;
			assert(pHF != NULL);
			m_pHalfFaces.push_back(pHF);

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
				m_pHalfEdges.push_back(pH[i]);

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

			VertexType * pv = m_map_Vertices[pHF->key(0)];

			pv->HalfFaces()->push_back(pHF);

			return pHF;
		};

		//construct faces
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_faces()
		{
			VertexType * pV = NULL;

			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				pV = *vIter;

				std::list<HalfFaceType*> * pL = VertexHalfFaceList(pV);

				while (!pL->empty())
				{
					HalfFaceType * pF = pL->front();
					pL->remove(pF);
					FaceType * f = new FaceType;
					assert(f != NULL);
					m_pFaces.push_back(f);
					f->SetLeft(pF);
					pF->SetFace(f);

					for (std::list<HalfFaceType*>::iterator it = pL->begin(); it != pL->end(); it++)
					{
						HalfFaceType * pH = *it;

						if (*pH == *pF)
						{
							pH->SetDual(pF);
							pF->SetDual(pH);
							f->SetRight(pH);
							pH->SetFace(f);
							break;
						}
					}

					if (pF->dual() != NULL)
					{
						pL->remove(HalfFaceDual(pF));
					}
				}
			}
		};

		//construct edges
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_edges()
		{
			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				std::list<TEdgeType*> * pL = VertexTEdgeList(pV);

				while (!pL->empty())
				{
					TEdgeType * pTE = pL->front();
					pL->remove(pTE);
					EdgeType * e = new EdgeType;
					assert(e != NULL);

					int id1 = pTE->key(0);
					VertexType * v1 = m_map_Vertices[id1];
					e->SetVertex1(v1);

					int id2 = pTE->key(1);
					VertexType * v2 = m_map_Vertices[id2];
					e->SetVertex2(v2);

					m_pEdges.push_back(e);

					e->edges()->push_back(pTE);
					pTE->SetEdge(e);

					std::list<TEdgeType*> tmp_edges;
					for (std::list<TEdgeType*>::iterator it = pL->begin(); it != pL->end(); it++)
					{
						TEdgeType * pH = *it;

						if (*pH == *pTE)
						{
							pH->SetEdge(e);
							tmp_edges.push_back(pH);
						}
					}

					for (std::list<TEdgeType*>::iterator it = tmp_edges.begin(); it != tmp_edges.end(); it++)
					{
						TEdgeType * pH = *it;
						pL->remove(pH);
						e->edges()->push_back(pH);
					}

				}

			}


			for (std::list<EdgeType*>::iterator it = m_pEdges.begin(); it != m_pEdges.end(); it++)
			{
				EdgeType * pE = *it;
				VertexType * v1 = EdgeVertex1(pE);
				VertexType * v2 = EdgeVertex2(pE);
				v1->edges()->push_back(pE);
				v2->edges()->push_back(pE);
			}
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_tet(TetType* pT, int tId, int * v)
		{
			//set the tet->id

			pT->id() = tId;

			//set TVertices of the Tet

			for (int k = 0; k < 4; k++)
			{
				TVertexType * pTV = new TVertexType();
				pT->setTVertex(pTV, k);
				pTV->id() = k;

				VertexType * pV = m_map_Vertices[v[k]];
				pTV->set_vert(pV);
				pV->tvertices()->push_back(pTV);

				pTV->set_tet(pT);
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
				pT->setHalfFace(_construct_half_face(pTV), i);
				pHF[i] = TetHalfFace(pT, i);
			}

			// connect the four half faces

			for (int i = 0; i < 4; i++)
			{
				pHF[i]->SetTet(pT);
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

				TEdgeType * pTE = new TEdgeType;
				assert(pTE != NULL);
				m_pTEdges.push_back(pTE);
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

				TEdgeType * pTE = new TEdgeType;
				assert(pTE != NULL);
				//save TEdges to the TEdge list
				m_pTEdges.push_back(pTE);
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

				VertexType * v = m_map_Vertices[pTE->key(0)];
				v->tedges()->push_back(pTE);

				pH0 = HalfEdgeNext(pH0);
			}
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_tet_orientation(TetType* pT, int tId, int  v[4])
		{
			//orient the tet 
			CPoint A = m_map_Vertices[v[0]]->position();
			CPoint B = m_map_Vertices[v[1]]->position();
			CPoint C = m_map_Vertices[v[2]]->position();
			CPoint D = m_map_Vertices[v[3]]->position();
			CPoint AB = B - A;
			CPoint AC = C - A;
			CPoint AD = D - A;

			double orientation_product = AB * (AC ^ AD);
			if (orientation_product < 0) {
				int temp = v[2];
				v[2] = v[3];
				v[3] = temp;
			}
			//set the tet->id

			pT->id() = tId;

			//set TVertices of the Tet

			for (int k = 0; k < 4; k++)
			{
				TVertexType * pTV = new TVertexType();
				pT->setTVertex(pTV, k);
				pTV->id() = k;

				VertexType * pV = m_map_Vertices[v[k]];
				pTV->set_vert(pV);
				pV->tvertices()->push_back(pTV);

				pTV->set_tet(pT);
			}

			//set half faces

			int order[4][3] = { { 1, 2, 3 }, { 2, 0, 3 }, { 0, 1, 3 }, { 1, 0, 2 } };

			TVertexType   * pTV[3];
			HalfFaceType * pHF[4];

			for (int i = 0; i < 4; i++)
			{
				for (int k = 0; k < 3; k++)
				{
					pTV[k] = TetTVertex(pT, order[i][k]);
				}
				pT->setHalfFace(_construct_half_face(pTV), i);
				pHF[i] = TetHalfFace(pT, i);
			}

			// connect the four half faces

			for (int i = 0; i < 4; i++)
			{
				pHF[i]->SetTet(pT);
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

				TEdgeType * pTE = new TEdgeType;
				assert(pTE != NULL);
				m_pTEdges.push_back(pTE);
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

				TEdgeType * pTE = new TEdgeType;
				assert(pTE != NULL);
				//save TEdges to the TEdge list
				m_pTEdges.push_back(pTE);
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

				VertexType * v = m_map_Vertices[pTE->key(0)];
				v->tedges()->push_back(pTE);

				pH0 = HalfEdgeNext(pH0);
			}
		};

		//write tet mesh to the file

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_write(const char * output)
		{

			std::fstream _os(output, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error is opening file %s\n", output);
				return;
			}
			_os << m_nVertices << " vertices" << std::endl;
			_os << m_nTets << " tets" << std::endl;

			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				CPoint  p = pV->position();
				for (int k = 0; k < 3; k++)
				{
					_os << " " << p[k];
				}
				if (pV->string().size() > 0)
				{
					_os << " " << "{" << pV->string() << "}";
				}
				_os << std::endl;
			}

			for (int i = 0; i < m_nTets; i++)
			{
				TetType * pt = m_map_Tets[i];
				_os << 4;
				for (int k = 0; k < 4; k++)
				{
					_os << " " << pt->tvertex(k)->vert()->id();
				}
				//if( pt->string().size() > 0 )
				//{
				//	_os << " " << "{"<< pt->string() << "}";
				//}
				_os << std::endl;
			}

			for (std::list<EdgeType*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				if (pE->string().size() > 0)
				{
					_os << "Edge " << EdgeVertex1(pE)->id() << " " << EdgeVertex2(pE)->id() << " ";
					_os << "{" << pE->string() << "}" << std::endl;
				}
			}

			_os.close();
		};

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_write_t(const char * output)
		{
			//write traits to string, add by Wei Chen, 11/23/2015
			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				pV->_to_string();
			}

			for (std::list<TetType *>::iterator tIter = m_pTets.begin(); tIter != m_pTets.end(); tIter++)
			{
				TetType * pT = *tIter;
				pT->_to_string();
			}

			for (std::list<EdgeType*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				pE->_to_string();
			}
			//write traits end

			std::fstream _os(output, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error while opening file %s\n", output);
				return;
			}

			for (std::list<VertexType*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				VertexType * pV = *vIter;
				CPoint p = pV->position();
				_os << "Vertex " << pV->id();
				for (int k = 0; k < 3; k++)
				{
					_os << " " << p[k];
				}
				if (pV->string().size() > 0)
				{
					_os << " " << "{" << pV->string() << "}";
				}
				_os << std::endl;
			}

			for (std::list<TetType *>::iterator tIter = m_pTets.begin(); tIter != m_pTets.end(); tIter++)
			{
				TetType * pT = *tIter;
				_os << "Tet " << pT->id();
				for (int k = 0; k < 4; k++)
				{
					_os << " " << pT->tvertex(k)->vert()->id();
				}
				if (pT->string().size() > 0)
				{
					_os << " " << "{" << pT->string() << "}";
				}
				_os << std::endl;
			}

			for (std::list<EdgeType*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				if (pE->string().size() > 0)
				{
					_os << "Edge " << EdgeVertex1(pE)->id() << " " << EdgeVertex2(pE)->id() << " ";
					_os << "{" << pE->string() << "}" << std::endl;
				}
			}

			_os.close();
		}

	};
};
#endif _MESHLIB_BASE_TET_MESH_H