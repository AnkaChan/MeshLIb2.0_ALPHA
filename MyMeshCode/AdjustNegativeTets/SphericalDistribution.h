#pragma once
#include <random>
#include <ctime>
#include <cmath>
#include <MeshLib/core/Geometry/Point2.H>
#include <MeshLib/core/Geometry/Point.H>

#define PI 3.1415926535897932

namespace MeshLib {
	namespace TMeshLib {
		class CSphericalDistribution {
		public:
			CSphericalDistribution() : generator(time(NULL)), randReal(0.0, 1.0)  {};
			CPoint2 getUnifSpherical() {
				double u, v, theta, phi;
				u = randReal(generator);
				v = randReal(generator);

				theta = 2 * PI * u;
				phi = acos(2*v - 1);

				return CPoint2(theta, phi);
			};

			CPoint getUnifDecartes() {
				double u, v, theta, cosPhi, sinPhi;
				u = randReal(generator);
				v = randReal(generator);

				theta = theta = 2 * PI * u;
				cosPhi = 2 * v - 1;
				sinPhi = sqrt(1 - cosPhi*cosPhi);

				return CPoint(sinPhi*cos(theta), sinPhi*sin(theta), cosPhi);
			}
		private:
			std::default_random_engine generator;
			std::uniform_real_distribution<double> randReal;
		};

	}
}
