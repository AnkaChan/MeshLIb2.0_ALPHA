#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/core/viewer/SimpleTetViewer.h>
#include <MeshLib/core/viewer/TMeshVisualTypes.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
#include <string>

using namespace MeshLib;
using namespace TMeshLib;
using std::cout;
using std::endl;
using std::string;
#define DEFAULT_FACE_COLOR 0.8,0.8,0.8

typedef TInterface<CTVertex, CVertexVisual, CHalfEdge, CTEdge, CEdgeVisual, CHalfFaceVisual, CFace, CTetVisual> TIF;
typedef TIterators<TIF> TIT;


int main(int argc, char **argv){
	if (argc != 2) {
		cout << "Please give a input file." << endl;
		return 1;
	}

	TIF::TMeshPtr pTMesh = new TIF::TMeshType;
	string inFile = standardSlash(argv[1]);

	pTMesh->_load_t(inFile.c_str(), true);

	CSimpleTetViewer TViewer;
	TViewer.setMeshPointer(pTMesh);
	TViewer.show();
}