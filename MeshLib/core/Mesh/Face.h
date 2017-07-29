/*!
*      \file Face.h
*      \brief Base class of face
*	   \author Iron
*      \date 15/07/2017
*
*/

#ifndef _MESHLIB_FACE_H_
#define _MESHLIB_FACE_H_

#include <assert.h>
#include <string>
#include "../Geometry/Point.h"
#include "HalfEdge.h"
namespace MeshLib{
//class HalfEdgeType;

/*!
	\brief FaceType base class of all kinds of face classes
*/

class CFace
{
public:
	/*!	
	FaceType constructor
	*/
	CFace() { m_halfedge = NULL; };
	/*!
	FaceType destructor
	*/
	~CFace(){};
	/*!
		One of the halfedges attaching to the current face.
	*/
	CHalfEdge    *		& halfedge() { return m_halfedge; };
	/*!
		The reference to the current face id
	*/
	int		            & id() { return m_id; };
	/*!
		The value of the current face id.
	*/
	//const int             id() const { return m_id; };
	/*!
		The string of the current face.
	*/
	std::string			& string() { return m_string; };
	/*!
		Convert face traits to the string.
	*/
	void                  _to_string()   {};
	/*!
		read face traits from the string.
	*/
	void                  _from_string() {};
protected:
	/*!
		id of the current face
	*/
	int			       m_id;
	/*!
		One halfedge  attaching to the current face.
	*/
	CHalfEdge        * m_halfedge;
	/*!
		String of the current face.
	*/
    std::string        m_string;
};


}//name space MeshLib

#endif //_MESHLIB_FACE_H_ defined