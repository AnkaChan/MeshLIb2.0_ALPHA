/*!
*      \file Boundary.h
*      \brief Trace boundary loops
*	   \author David Gu
*      \date 10/07/2010
*
*/


#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_
#define PIE 3.1415926535897932
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include <set>

#include "../Mesh/BaseMesh.h"
#include "../Mesh/iterators.h"

namespace MeshLib
{
/*!
	\brief CLoopSegment Boundary loop  segment class.	
	\tparam CVertex Vertex type
	\tparam EdgeType   Edge   type
	\tparam FaceType   Face   type
	\tparam HalfEdgeType HalfEdge type
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class CLoopSegment
{
public:
	/*!
		Constructor of the CLoopSegment
		\param pMesh  pointer to the current mesh
		\param pH halfedge on the boundary loop
	*/
	CLoopSegment(CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType>* pMesh, std::vector<HalfEdgeType*> & pHs )
	{
		m_pMesh = pMesh;

		for( size_t i = 0; i < pHs.size(); i ++ )
		{
			m_halfedges.push_back( pHs[i] );
		}
	}
	 /*!
		Destructor of CLoop.
	 */
	~CLoopSegment()
	{
		m_halfedges.clear();
	};
	
	/*!
	The vector of haledges on the current boundary loop segment.
	*/
	std::vector<HalfEdgeType*>  & halfedges() 
	{
		return m_halfedges;
	}
	/*!
		Starting vertex
	*/
	VertexType * start()
	{
		if( m_halfedges.size() == 0 ) return NULL;
		HalfEdgeType * he = m_halfedges[0];
		return m_pMesh->halfedgeSource( he );
	}
	/*!
		ending vertex
	*/
	VertexType * end()
	{
		if( m_halfedges.size() == 0 ) return NULL;
		size_t n = m_halfedges.size();
		HalfEdgeType * he = m_halfedges[n-1];
		return m_pMesh->halfedgeTarget( he );
	}
	
protected:
	/*!
		The mesh pointer
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType>		* m_pMesh;
	
	/*!
		The vector of consecutive halfedges along the boundary loop.
	*/
	std::vector<HalfEdgeType*>							  m_halfedges;

};

/*!
	\brief CLoop Boundary loop  class.	
	\tparam CVertex Vertex type
	\tparam EdgeType   Edge   type
	\tparam FaceType   Face   type
	\tparam HalfEdgeType HalfEdge type
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class CLoop
{
	typedef CLoopSegment<VertexType,EdgeType, FaceType, HalfEdgeType> TSegment;

public:
	/*!
		Constructor of the CLoop
		\param pMesh  pointer to the current mesh
		\param pH halfedge on the boundary loop
	*/
	 CLoop( CBaseMesh<VertexType, EdgeType,FaceType, HalfEdgeType> * pMesh, HalfEdgeType * pH );
	/*!
		Constructor of the CLoop
		\param pMesh  pointer to the current mesh
	*/
	 CLoop( CBaseMesh<VertexType, EdgeType,FaceType, HalfEdgeType> * pMesh ) { m_pMesh = pMesh; m_length = 0; m_pHalfedge = NULL; };
	 /*!
		Destructor of CLoop.
	 */
	~CLoop();
	
	/*!
	The list of haledges on the current boundary loop.
	*/
	std::list<HalfEdgeType*>  & halfedges()
	{
		return m_halfedges;
	}
	
	/*!
		The length of the current boundary loop.
	*/
	double length() 
	{ 
		return m_length;
	}
	/*!
	 *  Output to a file
	 */
	void write( const char * file );
	/*!
	 *  Input from a file
	 */
	void read( const char * file );
	/*!
		The vector of segments on this loop
	*/
	std::vector<TSegment*>  & segments() 
	{
		return m_segments;
	}
	/*!
		divide the loop to segments
		\param markers, the array of markers, the first marker is the starting one
	*/
	void divide( std::vector<VertexType*> & markers );

protected:
	/*!
		Pointer to the current mesh.
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType>		* m_pMesh;
	/*! The length of the current boundary loop.
	*/
	double											  m_length;
	/*!
		Starting halfedge of the current boundary loop.
	*/
	HalfEdgeType										* m_pHalfedge;
	/*!
		List of consecutive halfedges along the boundary loop.
	*/
	std::list<HalfEdgeType*>							  m_halfedges;
	/*!
		Vector of segments	 
	*/
	std::vector<TSegment*>							  m_segments;
};

/*!
	\brief CBoundary Boundary  class.	
	\tparam CVertex Vertex type
	\tparam EdgeType   Edge   type
	\tparam FaceType   Face   type
	\tparam HalfEdgeType HalfEdge type
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class CBoundary
{
	typedef CLoop<VertexType,EdgeType, FaceType, HalfEdgeType> TLoop;

public:
	/*!
	CBoundary constructor
	\param pMesh pointer to the current mesh
	*/
	CBoundary( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh );
	/*!
	CBoundary destructor
	*/
	~CBoundary();
	/*!
	The list of boundary loops.
	*/
	std::vector<TLoop*> & loops() 
	{ 
		return m_loops; 
	} 

protected:
	/*!
		Pointer to the current mesh.
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * m_pMesh;
	/*!
		List of boundary loops.
	*/
	typename std::vector<TLoop*> m_loops;
	/*!
		Bubble sort the loops
		\param loops the vector of loops
	*/
	void _bubble_sort( std::vector<CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>*> & loops);
};

/*!
	CLoop constructure, trace the boundary loop, starting from the halfedge pH.
	\param pMesh pointer to the current mesh
	\param pH  halfedge on the current boundary loop
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>::CLoop( CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> * pMesh, HalfEdgeType * pH )
{
	m_pMesh     = pMesh;
	m_pHalfedge = pH;

	m_length = 0;
	HalfEdgeType * he = pH;
	//trace the boundary loop
	do{
		VertexType * v = (VertexType*)he->target();
		he = m_pMesh->vertexMostClwOutHalfEdge( v );
		m_halfedges.push_back( he );
    m_length += m_pMesh->edgeLength( (EdgeType*)he->edge() );
	}while( he != m_pHalfedge );
}

/*!
CLoop destructor, clean up the list of halfedges in the loop
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>::~CLoop()
{
	m_halfedges.clear();

	for( size_t i = 0; i < m_segments.size(); i ++ )
	{
		delete m_segments[i];
	}
	m_segments.clear();
}


/*!
	_bubble_sort
	bubble sort  a vector of boundary loop objects, according to their lengths
	\param loops vector of loops
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CBoundary<VertexType, EdgeType, FaceType, HalfEdgeType>::_bubble_sort( std::vector<CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>*> & loops)
{
      int i, j, flag = 1;    // set flag to 1 to start first pass
      CLoop<VertexType,EdgeType,FaceType,HalfEdgeType> * temp;             // holding variable
      int numLength = (int)loops.size( ); 
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength -1); j++)
         {
               if (loops[j+1]->length() > loops[j]->length() )      // ascending order simply changes to <
              { 
                    temp = loops[j];								// swap elements
                    loops[j] = loops[j+1];
                    loops[j+1] = temp;
                    flag = 1;										// indicates that a swap occurred.
               }
          }
     }
}

/*!
	CBoundary constructor
	\param pMesh the current mesh
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
CBoundary<VertexType, EdgeType, FaceType, HalfEdgeType>::CBoundary( CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> * pMesh )
{
	m_pMesh = pMesh;
	//collect all boundary halfedges
	std::set<HalfEdgeType*> boundary_hes;
	for( MeshEdgeIterator<VertexType, EdgeType, FaceType, HalfEdgeType> eiter( m_pMesh); !eiter.end(); eiter ++ )
	{
		EdgeType * e = *eiter;
		if( !m_pMesh->isBoundary(e) ) continue;

		HalfEdgeType * he = m_pMesh->edgeHalfedge( e, 0);
		boundary_hes.insert( he );
	}
	//trace all the boundary loops
	while( !boundary_hes.empty() )
	{
		//get the first boundary halfedge
		std::set<HalfEdgeType*>::iterator siter = boundary_hes.begin();
		HalfEdgeType * he = *siter;
		//trace along this boundary halfedge
		CLoop<VertexType, EdgeType, FaceType, HalfEdgeType> * pL = new CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>( m_pMesh, he );
		assert(pL);
		m_loops.push_back( pL );
		//remove all the boundary halfedges, which are in the same boundary loop as the head, from the halfedge list
		for( std::list<HalfEdgeType*>::iterator hiter = pL->halfedges().begin(); 
			hiter != pL->halfedges().end(); hiter ++ )
		{
			HalfEdgeType * he = *hiter;
			siter = boundary_hes.find( he );
			if( siter == boundary_hes.end() ) continue;
			boundary_hes.erase( siter );
		}
	}
	
	//std::sort( vlps.begin(), vlps.end(), loop_compare<CVertex,FaceType,EdgeType,HalfEdgeType> );
	_bubble_sort( m_loops );
}

/*!	CBoundary destructor, delete all boundary loop objects.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
CBoundary<VertexType, EdgeType, FaceType, HalfEdgeType>::~CBoundary()
{
	for( int i = 0; i < (int)m_loops.size(); i ++ )
	{
		CLoop<VertexType, EdgeType, FaceType, HalfEdgeType> * pL = m_loops[i];
		delete pL;
	}
};

/*!
	Output the loop to a file
	\param file_name the name of the file
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>::write( const char * file_name )
{
	std::ofstream myfile;
	myfile.open (file_name);
	for( std::list<HalfEdgeType*>::iterator hiter = m_halfedges.begin(); hiter != m_halfedges.end(); hiter ++ )
	{
		HalfEdgeType * pH = *hiter;
		VertexType * pV = m_pMesh->halfedgeSource(pH);
		VertexType * pW = m_pMesh->halfedgeTarget(pH);
		myfile << pV->id() << " " << pW->id() << std::endl;
	}
	myfile.close();
};

/*!
	Output the loop to a file
	\param file_name the name of the file
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>::read( const char * file_name )
{
	std::ifstream myfile;
	myfile.open (file_name);

	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			int source, target;
			myfile >> source >> target;
			VertexType * pS = m_pMesh->idVertex( source );
			VertexType * pT = m_pMesh->idVertex( target );
			EdgeType   * pE = m_pMesh->vertexEdge( pS, pT );
			HalfEdgeType * pH = m_pMesh->edgeHalfedge(pE,0);
			m_halfedges.push_back( pH );
		}
		myfile.close();
	}

};


/*!
	Divide the loop to segments
	\param markers
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
void CLoop<VertexType, EdgeType, FaceType, HalfEdgeType>::divide( std::vector<VertexType*> & markers )
{
	std::deque<HalfEdgeType*> queue;
	for( std::list<HalfEdgeType*>::iterator hiter = m_halfedges.begin(); hiter != m_halfedges.end(); hiter ++ )
	{
		HalfEdgeType * ph = *hiter;
		queue.push_back( ph );
	}

	size_t n = 0;
	while( true )
	{
		HalfEdgeType * ph = queue.front();
		if( m_pMesh->halfedgeSource( ph ) == markers[0] ) break;
		queue.pop_front();
		queue.push_back( ph );
		n ++;
		if( n > queue.size() ) break;
	}

	if( n > queue.size() )
	{
		std::cerr << "CLoop::divide: can not find the starting vertex " << std::endl;
		return;
	}

	for( size_t i = 0; i < markers.size(); i ++ )
	{
		std::vector<HalfEdgeType*> hes;
		HalfEdgeType * ph = queue.front();
		queue.pop_front();
		assert( m_pMesh->halfedgeSource( ph ) == markers[i] );
		hes.push_back( ph );
		
		while( m_pMesh->halfedgeTarget(ph ) != markers[(i+1)%markers.size()] )
		{
			if( queue.empty())
			{	
				std::cerr << "CLoop::divide error" << std::endl;
				return;
			}
			ph = queue.front();
			queue.pop_front();
			hes.push_back( ph );
		}
		
		TSegment * pS = new TSegment( m_pMesh, hes );
		assert( pS != NULL );
		m_segments.push_back( pS );
	}
};


}
#endif

