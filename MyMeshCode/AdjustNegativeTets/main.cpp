// TetOrientation.cpp : 定义控制台应用程序的入口点。
//
#include <algorithm>
#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/vertex.h>
#include <MeshLib/core/TetMesh/tvertex.h>
#include <MeshLib/core/TetMesh/edge.h>
#include <MeshLib/core/TetMesh/tedge.h>
#include <MeshLib/core/TetMesh/face.h>
#include <MeshLib/core/TetMesh/halfface.h>
#include <MeshLib/core/TetMesh/halfedge.h> 

#include <MeshLib/core/TetMesh/tet.h>
		 
#include <MeshLib/core/TetMesh/titerators2.h>
#include <MeshLib/core/Geometry/Point.h>

#include <MeshLib/toolbox/FileIO.h>
#include "AdjuestNegative.h"

using namespace MeshLib;
using namespace MeshLib::TMeshLib;

typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
typedef TIterators<TIf> TIt;
typedef NegativeAdjust<TIf> CNegativeAdjuster;
typedef TIf::TMeshType MyTMesh;

TIf::TMeshPtr pMesh(new TIf::TMeshType);
TIf::TMeshType& mesh = *pMesh;
int argcG;
char ** argvG;


int main(int argc, char ** argv)
{
	argcG = argc;
	argvG = argv;

	if (argc < 2) {
		std::cout << "Please give a input directory." << std::endl;
		return 0;
	}
	FileParts fp = fileparts(argv[1]);

	mesh._load_t(argv[1]);
	std::cout << "Load done.\n";
	CNegativeAdjuster NAjuster;
	NAjuster.setInputTMesh(pMesh);
	NAjuster.adjust();
	
	std::string outPath = fp.path + fp.name + "_non_negative.t";
	pMesh->_write_t(outPath.c_str(), true);

	getchar();
	return 0;
}

