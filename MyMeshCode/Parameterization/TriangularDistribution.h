#pragma once
#include <random>
#include <ctime>
#include <cmath>
#include <MeshLib/core/Geometry/Point2.H>
#include <MeshLib/core/Geometry/Point.H>

#define PI 3.1415926535897932

namespace MeshLib {
	namespace TMeshLib {
		class CTriangularDistribution {
		public:
			CTriangularDistribution() : generator(time(NULL)), randReal(0.0, 1.0) {};


			CPoint getUnif() {
				CPoint p(randReal(generator), randReal(generator), randReal(generator));
				double c = (p[0] + p[1] + p[2]) / 3;
				p[0] -= c;
				p[1] -= c;
				p[2] -= c;

				return p;
			}
		private:
			std::default_random_engine generator;
			std::uniform_real_distribution<double> randReal;
		};

	}
}
