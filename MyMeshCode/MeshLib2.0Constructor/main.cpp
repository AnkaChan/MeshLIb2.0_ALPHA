#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/iterators_new.h>

using namespace MeshLib;
class myVertex : public CVertex {
public:
	int num = 0;
};
class myHalfedge : public CHalfEdge {
public:
	int num = 10;
};
using std::cout;
using std::endl;
int main() {
	typedef CInterface<myVertex, CEdge, CFace, myHalfedge> Interface;
	typedef CIteratorsI<Interface> Iterators;
	typedef Interface::MeshType CMesh;

	std::vector<int> v(10);
	for (int& i : v) {
		i = 0;
	}

	CMesh mesh;
	mesh.read_m("D:/Data/Mesh/outputs/face125.m");
	Iterators::MEIterator meIter(&mesh);
	//cout << "Iterating Edges." << endl;
	//for (auto pE : meIter) {
	//	Interface::VPtr pV1, pV2;
	//	pV1 = Interface::edgeVertex1(pE);
	//	pV2 = Interface::edgeVertex2(pE);

	//	cout << "The edge's length:" << Interface::edgeLength(pE)
	//		<< "\nThe two vertice are: " << Interface::vertexId(pV1) << ", " << Interface::vertexId(pV2) << endl;

	//	cout << "Vertex 1: " << pV1->point() << endl;
	//	cout << "Vertex 2: " << pV2->point() << endl;

	//}

	Iterators::MVIterator mvIter(&mesh);
	cout << "Iterating Edges." << endl;
	for (auto pV : mvIter) {
		cout << "Vertex id: " << Interface::vertexId(pV) << " Position: " << pV->point() << endl;

		if (Interface::isBoundary(pV))
			cout << "Vertex on boundary." << endl;
		else
			cout << "Vertex in interior." << endl;

		Iterators::VVIterator vvIter(pV);
		cout << "Iterating vertices using halfedge list:" << endl;
		cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		for (auto pNeiV : vvIter) {
			cout << "Neighbor Vertex id: " << Interface::vertexId(pV) << " Position: " << pV->point() << endl;
		}

		cout << "-----------------------------" << endl;
		cout << "Iterating vertices using halfedge conection(CCW):" << endl;
		Iterators::VCcwVIterator vccwvIter(pV);
		for (auto pNeiV : vvIter){
			cout << "Neighbor Vertex id: " << Interface::vertexId(pV) << " Position: " << pV->point() << endl;
		}
		
		//Iterators::VOutHEIterator voutheIter(pV);
		//for (auto pHE : voutheIter) {
		//	cout << "The halfedge: " << pHE << endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << endl;
		//}
		//cout << "-----------------------------" << endl;
		//cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		//Iterators::VCcwOutHEIterator vccwheiter(pV);
		//for (Interface::HEPtr pHE : vccwheiter) {
		//	std::cout << "The halfedge: " << pHE << std::endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	Interface::VPtr pSourceV = Interface::halfedgeSource(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << " Source vertex id: " << Interface::vertexId(pSourceV) << endl;
		//}
		//cout << "-----------------------------" << endl;
		//cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		//Iterators::VClwInHEIterator vclwheiter(pV);
		//for (Interface::HEPtr pHE : vclwheiter) {
		//	std::cout << "The halfedge: " << pHE << std::endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	Interface::VPtr pSourceV = Interface::halfedgeSource(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << " Source vertex id: " << Interface::vertexId(pSourceV) << endl;
		//}
		
		getchar();

	}

	getchar();

	Interface::VPtr pV = mesh.idVertex(10);
	Iterators::VCcwOutHEIterator vheiter(pV);
	for (Interface::HEPtr pHE : vheiter) {
		std::cout << "This is the halfedge pointer:" << (int)pHE << std::endl;
	}
	  
	auto find_zero = [] (const int& a) {return a == 0; };
	auto zeros = std::find_if(v.begin(), v.end(), find_zero);

	/*Iterators::VCcwOutHEIterator vccwheiter(pV);
	for (Interface::HEPtr pHE : vccwheiter) {
		std::cout << "This is the halfedge pointer:" << (int)pHE << std::endl;
	}*/
	getchar();
}