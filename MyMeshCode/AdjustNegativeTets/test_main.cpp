#include "BaseMeshReadArray.h"
#include <MeshLib/core/Mesh/MeshHeaders.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#include <iostream>
using std::cout;
using std::endl;
using namespace MeshLib;
int main() {
	VList vl;
	FList fl;

	vl.push_back(VertexElement(0, -1,  1, 0));
	vl.push_back(VertexElement(1, -1, -1, 0));
	vl.push_back(VertexElement(2,  0,  0, 0));
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

	typedef CInterface<CVertex, CEdge, CFace, CHalfEdge, CBaseMeshReadArray> If;
	If::MeshType mesh;
	mesh.readArray(vl, fl);
	CSimpleMeshViewer viewer(&mesh);
	viewer.setting().edgeColorMode = GLSetting::defaultColor;

	cout << "Manifold  determine: " << mesh.manifoldDeterminition() << '\n';
	cout << "IsD2: " << mesh.isD2() << '\n';

	cout << "Number of boundaries: " << mesh.numBoundaries() << "\nThe 1st boundary edges:\n";
	std::shared_ptr<EList> pEl = mesh.getBoudaryLoop();
	for (EdgeElement e : *pEl) {
		cout << "Edge: " << e.vertices[0];
		cout << ", ";
		cout << e.vertices[1] << '\n';
	}
	cout << "\nThe 2nd boundary edges:\n";
	pEl = mesh.getBoudaryLoop(1);
	for (EdgeElement e : *pEl) {
		cout << "Edge: " << e.vertices[0];
		cout << ", ";
		cout << e.vertices[1] << '\n';
	}
	viewer.show();
}