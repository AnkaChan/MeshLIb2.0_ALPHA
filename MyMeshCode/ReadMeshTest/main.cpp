#include <iostream>
#include <Eigen\Dense>
#include <ctime>
#include <MeshLib\core\Mesh\MeshHeaders.h>

using namespace MeshLib;
int main() {
	clock_t  clockBegin, clockEnd;
	clockBegin = clock();
	CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> mesh;
	mesh.read_m("D:/Data/Mesh/Alex/Alex.m");
	clockEnd = clock();
	printf("Time: %dms\n", clockEnd - clockBegin);
	getchar();
}