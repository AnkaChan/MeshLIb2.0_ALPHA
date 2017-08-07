#pragma once
#include <MeshLib/3rdParty/Eigen/Dense>
#include <cmath>

namespace MeshLib {

	/*
	* Get the great circle arc between A amd B, centered at O
	* so distance of AO and BO should be equal.
	*/
	class Arcs {
	public:
		Arcs(Eigen::Vector3d A, Eigen::Vector3d B, Eigen::Vector3d O);
		double getArcRadian() { return m_theta; };
		Eigen::Vector3d getPointFromArc(double theta);
	private:
		Eigen::Vector3d m_A, m_B, m_O, nA, nB, u, v, w;
		Eigen::Matrix3d G, F, FInverse;
		double m_theta, r;

		Eigen::Matrix3d getG(double theta);
	};

	Arcs::Arcs(Eigen::Vector3d A, Eigen::Vector3d B, Eigen::Vector3d O) : m_A(A), m_B(B), m_O(O)
	{
		nA = (m_A - m_O);
		r = nA.norm();
		nA = nA / r;

		nB = (m_B - m_O) / r;

		m_theta = acos(nA.transpose()*nB);
		
		u = nA;
		v = nB - (nA.transpose()*nB)*nA;
		v = v / v.norm();
		w = nB.cross(nA);

		FInverse << u, v, w;
		F = FInverse.inverse();
	}
	inline Eigen::Vector3d Arcs::getPointFromArc(double theta)
	{
		Eigen::Vector3d p, nP;
		Eigen::Matrix3d U = FInverse * getG(theta) * F;

		nP = U * nA;
		p = nP * r + m_O;
		return p;
	}
	inline Eigen::Matrix3d Arcs::getG(double theta)
	{
		Eigen::Matrix3d g;
		g << cos(theta), -sin(theta), 0,
			 sin(theta),  cos(theta), 0,
			 0, 0, 1;
		return g;
	}
}