#include "BaseMeshReadArray.h"
#include <MeshLib/core/Mesh/MeshHeaders.h>
#include <iostream>
#include "C2DMesh.h"
using std::cout;
using std::endl;
using namespace MeshLib;
int main() {
	VList vl;
	FList fl;

	vl.push_back(VertexElement(0, -1, 1, 0));
	vl.push_back(VertexElement(1, -1, -1, 0));
	vl.push_back(VertexElement(2, 0, 0, 0));
	vl.push_back(VertexElement(3, 1, 1, 0));
	vl.push_back(VertexElement(4, 1, -1, 0));
	vl.push_back(VertexElement(5, 0, -1, 0));
	vl.push_back(VertexElement(6, 0, -2, 0));
	vl.push_back(VertexElement(7, 2, -2, 0));

	fl.push_back(FaceElement(0, 0, 1, 2));
	fl.push_back(FaceElement(1, 3, 2, 4));
	fl.push_back(FaceElement(2, 1, 5, 2));
	//fl.push_back(FaceElement(3, 2, 5, 4));
	fl.push_back(FaceElement(4, 0, 2, 3));
	fl.push_back(FaceElement(5, 1, 6, 5));
	fl.push_back(FaceElement(6, 5, 6, 4));
	fl.push_back(FaceElement(7, 4, 6, 7));
	fl.push_back(FaceElement(8, 3, 4, 7));


	C2DMesh mesh;
	mesh.readArray(vl, fl);

	cout << "Manifold Determinition: " << mesh.manifoldDeterminition() << endl;
	cout << "D2 Determinition: " << mesh.isD2() << endl;
	getchar();
}

