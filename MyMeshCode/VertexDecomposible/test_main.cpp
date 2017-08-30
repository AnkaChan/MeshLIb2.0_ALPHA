#include <MeshLib/core/Mesh/MeshHeaders.h>
#include "BaseMeshReadArray.h"

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
	vl.push_back(VertexElement(8, 1, -1, 1));
	vl.push_back(VertexElement(9, 2, -2, 1));
	vl.push_back(VertexElement(10, 0, -3, 1));
	vl.push_back(VertexElement(11, 1, -3, 1));

	fl.push_back(FaceElement(0, 0, 1, 2));
	fl.push_back(FaceElement(1, 3, 2, 4));
	fl.push_back(FaceElement(2, 1, 5, 2));
	fl.push_back(FaceElement(3, 2, 5, 4));
	fl.push_back(FaceElement(4, 0, 2, 3));
	fl.push_back(FaceElement(5, 1, 6, 5));
	fl.push_back(FaceElement(6, 5, 6, 4));
	fl.push_back(FaceElement(7, 4, 6, 7));
	fl.push_back(FaceElement(8, 3, 4, 7));
	fl.push_back(FaceElement(9, 6, 4, 9));
	//fl.push_back(FaceElement(10, 6, 10, 11));

	typedef CInterface<CVertexManifold, CEdge, CFace, CHalfEdge, CBaseMeshReadArray> If;
	typedef CIterators<If> It;
	If::MeshType mesh;
	//CBaseMeshReadArray<CVertexManifold, CEdge, CFace, CHalfEdge> mesh;
	mesh.readArray(vl, fl);
	//mesh.makeFansForVertices();
	for (If::VPtr pV : It::MVIterator(&mesh)) {
		std::list<CFan> fanList = pV->getFans();
		cout << "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
		cout << "Vertex id:" << pV->id() << "\n";
		cout << "Number of fans: " << pV->getNumFans();
		int fanId = 0;
		for (CFan fan : fanList) {
			cout << "\n-------------------------\n"
				<< "Fan : " << ++fanId << "\n"
				<< "Number of triangles: " << fan.numberTriangles << "\n"
				<< "Is closed: " << fan.isClosed << "\n"
				<< "most clw out halfedge: " << fan.mostClwOutHE->source()->id() << "->" << fan.mostClwOutHE->target()->id()
				<< "\nmost ccw in halfedge: " << fan.mostCcwIntHE->source()->id() << "->" << fan.mostCcwIntHE->target()->id();

		}
	}
	cout << "\nMesh is manifold: " << mesh.isManifold();
	CSimpleMeshViewer viewer(&mesh);
	viewer.setting().edgeColorMode = GLSetting::defaultColor;

	/*cout << "Manifold  determine: " << mesh.manifoldDeterminition() << '\n';
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
	}*/
	viewer.show();
}