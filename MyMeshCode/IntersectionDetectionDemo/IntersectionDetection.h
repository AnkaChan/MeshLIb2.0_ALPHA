#pragma once
#include <MeshLib/core/Geometry/Point.h>
#include <cmath>

namespace MeshLib {
	class CIntersectionDetection {
	public:
		CIntersectionDetection() {};
		void setLineSegment(CPoint newL1, CPoint newL2) {
			L1 = newL1;
			L2 = newL2;
			O = L1;
			D = L2 - L1;
		};
		void setTriangle(CPoint newA, CPoint newB, CPoint newC) {
			A = newA;
			B = newB;
			C = newC;

			E1 = B - A;
			E2 = C - A;
		};
		bool detect() {
			T = O - A;
			P = D ^ E2;
			Q = T ^ E1;
			double k = (P*E1);

			t = Q * E2 / k;
			u = P * T / k;
			v = Q * D / k;

			if (0 < t && t < 1) {
				if (0 < u && u < 1 && 0 < v && u < 1) {
					if (u + v < 1) {
						return true;
					}
				}
			}
			return false;
		};
		bool detectApproximately() {
			T = O - A;
			P = D ^ E2;
			Q = T ^ E1;
			double k = (P*E1);

			t = Q * E2 / k;
			u = P * T / k;
			v = Q * D / k;

			if (0.01 < t && t < 0.99) {
				if (0.01 < u && u < 0.99 && 0.01 < v && u < 0.99) {
					if (u + v < 0.99) {
						return true;
					}
				}
			}
			return false;
		};

		CPoint intersectPoint() {
			return O + D * t;
		}

	private:
		CPoint L1, L2, O, D, A, B, C, E1, E2, T, P, Q;
		double t, u, v;
	};
}