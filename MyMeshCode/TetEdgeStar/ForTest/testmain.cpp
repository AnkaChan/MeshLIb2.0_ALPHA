#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/core/viewer/TMeshVisualTypes.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
#include <string>

#include "../DynamicTMesh.h"

using namespace MeshLib;
using namespace TMeshLib;
using std::cout;
using std::endl;
using std::string;
#define DEFAULT_FACE_COLOR 0.8,0.8,0.8

typedef TInterface<CTVertex, CVertexVisual, CHalfEdge, CTEdge, CEdgeVisual, CHalfFaceVisual, CFace, CTetVisual, CDynamicTMesh> TIf;
typedef TIterators<TIf> TIt;


int main(int argc, char **argv) {
	if (argc != 2) {
		cout << "Please give a input file." << endl;
		return 1;
	}

	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	string inFile = standardSlash(argv[1]);
	pTMesh->_load_t(inFile.c_str(), true);

	TIf::TPtr pT = pTMesh->tets().front();
	pTMesh->deleteTet(pT);

	int tet[4] = { 1, 2, 3, 4 };
	pTMesh->dynamic_construct_tet(tet);


	cout << "Done!" << endl;
}