#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/core/viewer/SimpleTetViewer.h>
#include <MeshLib/core/viewer/TMeshVisualTypes.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
#include <string>

#include "DynamicTMesh.h"

using namespace MeshLib;
using namespace TMeshLib;
using std::cout;
using std::endl;
using std::string;
#define DEFAULT_FACE_COLOR 0.8,0.8,0.8

typedef TInterface<CTVertex, CVertexVisual, CHalfEdge, CTEdge, CEdgeVisual, CHalfFaceVisual, CFace, CTetVisual, CDynamicTMesh> TIf;
typedef TIterators<TIf> TIt;


int main(int argc, char **argv){
	if (argc != 2) {
		cout << "Please give a input file." << endl;
		return 1;
	}

	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	string inFile = standardSlash(argv[1]);

	pTMesh->_load_t(inFile.c_str(), true);

	int maxVId = pTMesh->maxVertexId();
	TIf::VPtr pV = new TIf::VType;
	pV->id() = maxVId + 1;
	pV->position() = CPoint(0.5, 0.5, 0);
	TIf::TPtr pT = pTMesh->tets().front();
	TIf::TEPtr pSplitpTE;
	for (auto pTE : TIt::T_TEIterator(pT)) {
		TIf::VPtr pV1, pV2;
		TIf::EPtr pE = TIf::TEdgeEdge(pTE);
		pV1 = TIf::EdgeVertex1(pE);
		pV2 = TIf::EdgeVertex2(pE);

		if (pV1->id() == 2 && pV2->id() == 3) {
			pSplitpTE = pTE;
			break;
		}
		else if (pV1->id() == 3 && pV2->id() == 2) {
			pSplitpTE = pTE;
			break;
		}
	}

	TIf::TPtr newTets[2];
	pTMesh->tetEdgeSplit(pSplitpTE, pV, newTets);

	CSimpleTetViewer TViewer;
	TViewer.setMeshPointer(pTMesh);
	TViewer.show();
}