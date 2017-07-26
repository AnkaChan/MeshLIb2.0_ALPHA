#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/DynamicMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/iterators_new.h>
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
	mesh.read_obj("D:/Data/Mesh/spot/spot_triangulated.obj");
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