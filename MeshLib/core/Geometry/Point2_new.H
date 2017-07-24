/*!
*      \file Point2.h
*      \brief Two dimensional point class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#include <math.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <Eigen/Dense>
#ifndef _MESHLIB_POINT2_H_
#define _MESHLIB_POINT2_H_



namespace MeshLib {

	/*!
	*	\brief CPoint2 class, two dimensional point
	*
	*	Two dimensional point \f$(x,y)\f$
	*/
	class CPoint2 : public Eigen::Vector2d 
	{
	public:
		/*!
		*	CPoint2 default constructor, it is initialized to be (0,0)
		*/
		CPoint2() : Eigen::Vector2d(0,0, 0.0) {};
		/*!
		*	CPoint2 class copy operator
		*/
		CPoint2(const CPoint2 & uv) : Eigen::Vector2d(uv[0], uv[1]) {};
		/*!
		 *	CPoint2 class destructor
		 */
		~CPoint2() {};

		/*!
		 *	CPoint2 constructor with \f$(x,y)\f$
		 */
		CPoint2(double  x, double y) : Eigen::Vector2d(x, y) {};

		/*! The square norm of CPoint2 \f$x^2+y^2\f$
		 *
		 */
		double norm2() { double n2 = this->norm(); return  n2*n2; };

		CPoint2 * & next() { return m_next; };
		CPoint2 * & prev() { return m_prev; };
	private:
		CPoint2 *m_prev, *m_next;
	};

	/*!
	 *	square of the norm of the CPoint2 \f$x^2+y^2\f$
	 *  \param uv input two dimensional point
	 *  \return square of the norm of the point
	 */
	inline double mag2(CPoint2 & uv)
	{
		return uv[0] * uv[0] + uv[1] * uv[1];
	};

	/*!
	 *	norm of the CPoint2 \f$ \sqrt{x^2+y^2}\f$
	 *  \param uv input two dimensional point
	 *  \return norm of the point
	 */
	inline double mag(CPoint2 & uv)
	{
		return sqrt(uv[0] * uv[0] + uv[1] * uv[1]);
	};

	/*!
	 *	cross product of two CPoint2
	 *  \param uv1 the first  two diemenional point
	 *  \param uv2 the second two diemenional point
	 *  \return the cross product of the two points
	 *  \f[
	 *  \left|
	 *	\begin{array}{cc}
	 *  x_1 & y_1\\
	 *  x_2 & y_2 \\
	 *  \end{array}
	 *	\right|
	 *  \f]
	 */

	inline double cross(CPoint2 uv1, CPoint2 uv2)
	{
		return (uv1[0] * uv2[1] - uv1[1] * uv2[0]);
	};


	/*!
	 *	inner product of two CPoint2
	 *  \param a the first  two diemenional point
	 *  \param b the second two diemenional point
	 *  \return the inner product of the two points
	 */
	inline double operator*(CPoint2 a, CPoint2 b)
	{
		return a[0] * b[0] + a[1] * b[1];
	}

	/*!
	 *	cross product of two CPoint2
	 *  \param uv1 the first  two diemenional point
	 *  \param uv2 the second two diemenional point
	 *  \return the cross product of the two points
	 */

	inline double operator^(CPoint2 uv1, CPoint2 uv2)
	{
		return (uv1[0] * uv2[1] - uv1[1] * uv2[0]);
	};

	/*!
	 *	read in a CPoint2 from a string
	 *  \param str input string
	 *  \param c   the reference to a point
	 *  \return the value of (x,y) read out from the string is assigned bo c
	 */

	inline void operator >> (const std::string & str, CPoint2& c)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of("()"));
		t.erase(t.find_last_not_of("()") + 1);
		std::istringstream iss(t);

		iss >> c[0] >> c[1];
	}

}; //namespace

#endif
