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
		 
#include <MeshLib/core/TetMesh/titerators.h>
#include <MeshLib/core/TetMesh/titerators2.h>
#include <MeshLib/algorithm/Shelling/TetSheller.h>
#include <MeshLib/core/Geometry/Circumsphere.h>
#include <MeshLib/core/Geometry/Point.h>

#include "D3Parameterization.h"
#include "GLTetViewReverseShelling.h"
#include "VertexDecomposable.h"

using namespace MeshLib;
using namespace MeshLib::TMeshLib;

typedef TInterface<CTVertex, CVertexDecomposition,CHalfEdge, CTEdge, CEdge, CHalfFace, CFaceD3Parameterization, CTetDecomposition> TIf;
typedef CTetSheller<TIf> CTSheller;
typedef D3Parameterization<TIf> D3Para;
typedef CVertexDecomposer<TIf> CVDecomposer;
typedef TIterators<TIf> TIt;
typedef TIf::TMeshType MyTMesh;

D3Para * pd3Para;
TIf::TMeshPtr pMesh(new TIf::TMeshType);
std::shared_ptr<std::list<CTetShelling *>> pShellingList;
TIf::TMeshType& mesh = *pMesh;
int argcG;
char ** argvG;
struct Sphere {
	CPoint center;
	double radius;
} sphere;

int main(int argc, char ** argv)
{
	argcG = argc;
	argvG = argv;

	if (argc < 2) {
		std::cout << "Please give a input directory." << std::endl;
		return 0;
	}
	mesh._load_t(argv[1]);
	std::cout << "Load done.\n";
	CVDecomposer decomposer;
	 
	decomposer.setInputTMesh(&mesh);
	TIf::TPtr pTStart = mesh.idTet(1);
	decomposer.vertexDecompositionGreedily(pTStart);

	//init_openGL();
	//mesh._write_t("D:\\Data\\tet\\FastOutDemo.t");
	//std::cout << "Save done.\n";
	getchar();
	return 0;
}

