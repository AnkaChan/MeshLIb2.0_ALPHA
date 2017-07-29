/*!
*      \file Point.h
*      \brief Three dimensional point class
*	   \author David Gu
*      \date 10/07/2010
*
*/

#ifndef _MESHLIB_POINT_H_
#define _MESHLIB_POINT_H_

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>
#include <Eigen/Dense>

namespace MeshLib
{

	/*!
	*	\brief CPoint class, three dimensional point
	*   can also be regarded as a Eigen::vector3d
	*	Three dimensional point or vector \f$(x,y,z)\f$, inner product, cross product, scale multiplication etc.
	*/
	class CPoint : public Eigen::Vector3d {

	public:
		CPoint(const CPoint & v) : Eigen::Vector3d(v[0], v[1], v[2]) {};
		CPoint() : Eigen::Vector3d(0.0, 0.0, 0.0) {};
		CPoint(double v0, double v1, double v2) : Eigen::Vector3d(v0, v1, v2) {};
		CPoint(Eigen::Vector3d v) : Eigen::Vector3d(v) {};

		/*!
		*	the pointer of the coordinater
		*   for the compatibility with the 1.0 version
		*   \return the pointer double *
		*/
		double * coord() { return data(); };

		/*!
		* dot product
		* overrided the * of Vector3d
		* \param p another point
		* \return dot product of current point with p.
		*/
		double   operator*(const CPoint & p) const
		{
			return this->dot(p);
		};

		/*!
		* Cross product
		* \param p2 another point
		* \return cross product of the current point with p2.
		* \f[
		* \left|
		\begin{array}{ccc}
		x_1& y_1 & z_1\\
		x_2 &y_2 & z_2\\
		\mathbf{e}_1 & \mathbf{e}_2 & \mathbf{e}_3\\
		\end{array}
		* \right|
		* \f]
		*/
		CPoint operator^(const CPoint & p2) const
		{
			return this->cross(p2);
		};
		friend std::ostream & operator << (std::ostream & os, const CPoint & p)
		{
			os << p[0] << " " << p[1] << " " << p[2];
			return os;
		};
	};
	inline void operator>>(const std::string & str, CPoint &p)
	{
		std::string t = str;
		t.erase(0, t.find_first_not_of("()"));
		t.erase(t.find_last_not_of("()") + 1);
		std::istringstream iss(t);

		iss >> p[0] >> p[1] >> p[2];
	}
}//name space TMeshLib
#endif //_TMESHLIB_POINT_H_ defined