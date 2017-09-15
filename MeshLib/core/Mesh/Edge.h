/*!
*      \file Edge.h
*      \brief Base class of edge
*	   \author Iron
*      \date 15/07/2017
*
*/


#ifndef _MESHLIB_EDGE_H_
#define _MESHLIB_EDGE_H_

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "HalfEdge.h"
namespace MeshLib {

	//class CHalfEdge;
	//class CVertex;

<<<<<<< HEAD
/*!
\brief CEdge class, which is the base class of all kinds of edge classes
*/
class CEdge
{
public:
	/*!
		CEdge constructor, set both halfedge pointers to be NULL.
	*/
	CEdge() { m_halfedge = NULL; };
	/*!
		CEdge destructor.
	*/
	~CEdge(){};
	/*!
		Edge ID
	 */
	int & id() { return m_id; };

	/*!
		The halfedge attached to the current edge
		
	*/
	CHalfEdge * & halfedge() { return m_halfedge; };
	/*!	
		whether the edge is on the boundary.
	*/
	bool		  boundary() { return (m_halfedge->he_sym() == NULL);};
	/*!
		The dual halfedge to the input halfedge
		\param he halfedge attached to the current edge
		\return the other halfedge attached to the current edge
	*/
	CHalfEdge * & other(CHalfEdge*  ph) { return ph->he_sym(); };
    /*!
		The string of the current edge.
	*/
	std::string & string() { return m_string; };
	/*!
		Read the traits from the string.
	*/
	void _from_string() {};
	/*!
		Save the traits to the string.
	*/
	void _to_string() {};

protected:
	/*!
		Pointers to the halfedge attached to the current edge
	*/
	CHalfEdge      * m_halfedge;
=======
>>>>>>> 8d426747a581b6594a44b7eb5b652884a339d6a1
	/*!
	\brief CEdge class, which is the base class of all kinds of edge classes
	*/
<<<<<<< HEAD
	//bool			 m_isboubdary;
    std::string      m_string;
	/*!
		Edge ID
	 */
	int				 m_id;

};

=======
	class CEdge
	{
	public:
		/*!
			CEdge constructor, set both halfedge pointers to be NULL.
		*/
		CEdge() { m_halfedge = NULL; };
		/*!
			CEdge destructor.
		*/
		~CEdge() {};
		/*!
			Edge ID
		 */
		int & id() { return m_id; };
>>>>>>> 8d426747a581b6594a44b7eb5b652884a339d6a1

		/*!
			The halfedge attached to the current edge

		*/
		CHalfEdge * & halfedge() { return m_halfedge; };
		/*!
			whether the edge is on the boundary.
		*/
		bool		  boundary() { return (m_halfedge->he_sym() == NULL); };
		/*!
			The dual halfedge to the input halfedge
			\param he halfedge attached to the current edge
			\return the other halfedge attached to the current edge
		*/
		CHalfEdge * & other(CHalfEdge*  ph) { return ph->he_sym(); };
		/*!
			The string of the current edge.
		*/
		std::string & string() { return m_string; };
		/*!
			Read the traits from the string.
		*/
		void _from_string() {};
		/*!
			Save the traits to the string.
		*/
		void _to_string() {};
	protected:
		/*!
			Pointers to the halfedge attached to the current edge
		*/
		CHalfEdge      * m_halfedge;
		/*!
			The string associated to the current edge.
		*/
		//bool			 m_isboubdary;
		std::string      m_string;
		/*!
			Edge ID
		 */
		int				 m_id;
	};
}//name space MeshLib

#endif //_MESHLIB_EDGE_H_ defined