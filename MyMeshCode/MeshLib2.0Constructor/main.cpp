#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/iterators_new.h>

using std::cout;
using std::endl;
using namespace MeshLib;
int main() {
	typedef CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> CMesh;
	typedef CInterface<CVertex, CEdge, CFace, CHalfEdge> Interface;
	typedef CIterators<CVertex, CEdge, CFace, CHalfEdge> Iterators;

	std::vector<int> v(10);
	for (int& i : v) {
		i = 0;
	}

	CMesh mesh;
	mesh.read_m("D:/Data/Mesh/outputs/face125.m");
	Iterators::MEIterator meIter(&mesh);
	cout << "Iterating Edges." << endl;
	for (auto pE : meIter) {
		Interface::VertexPtr pV1, pV2;
		cout << "The edge's length:" << Interface::edgeLength(pE);
		getchar();

	}

	Iterators::MEIterator meIter(&mesh);
	cout << "Iterating Vertices." << endl;
	for (auto pE : meIter) {
		Interface::VertexPtr pV1, pV2;
		cout << "The edge's length:" << Interface::edgeLength(pE);
		getchar();

	}

	Interface::VertexPtr pV = mesh.idVertex(10);
	Iterators::VOutHEIterator vheiter(pV);
	for (Interface::HalfEdgePtr pHE : vheiter) {
		std::cout << "This is the halfedge pointer:" << (int)pHE << std::endl;
	}
	  
	auto find_zero = [] (const int& a) {return a == 0; };
	auto zeros = std::find_if(v.begin(), v.end(), find_zero);

	/*Iterators::VCcwOutHEIterator vccwheiter(pV);
	for (Interface::HalfEdgePtr pHE : vccwheiter) {
		std::cout << "This is the halfedge pointer:" << (int)pHE << std::endl;
	}*/
	getchar();
}