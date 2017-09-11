#include <iostream>
#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace TMeshLib;

int main(int argc, char** argv) {
	typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	//typedef TIteratorCore<myTV, myV, myHE, myTE, myE, myHF, myF, myTet> TIT;
	typedef TIterators<TIf> TIt;

	if (argc < 2) {
		cout << "Please give input path." << endl;
		return 0;
	}

	char * inPath = argv[1];
	FileParts fp = fileparts(inPath);
	If::MeshPtr pMesh = new If::MeshType;
	if (fp.ext == ".obj") {
		pMesh->read_obj(inPath);
	}
	else if (fp.ext == ".m") {
		pMesh->read_m(inPath);
	}
	else {
		cout << "Unsupported file format: " << fp.ext << endl;
	}
	getchar();
}