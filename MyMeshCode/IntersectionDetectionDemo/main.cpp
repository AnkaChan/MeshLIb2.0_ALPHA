#include <iostream>
#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include "IntersectionDetection.h"
using std::cout;
using std::endl;
using namespace MeshLib;

int main() {
	CPoint l1(0, 1, -1), l2(0, 1, 0.1);
	CPoint A(0, 1, 0.1), B(-1, -1, 0), C(1, -1, -0.45);
	CIntersectionDetection IntersectionDetector;
	IntersectionDetector.setLineSegment(l1, l2);
	IntersectionDetector.setTriangle(A, B, C);
	if (IntersectionDetector.detect()) {
		cout << "Intersect. " << endl;
		cout << "Intersect at : " << IntersectionDetector.intersectPoint() << endl;
	}
	else {
		cout << "Not Intersect. " << endl;
	}
	getchar();
}