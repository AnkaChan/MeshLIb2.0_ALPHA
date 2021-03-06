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


using namespace MeshLib;
using namespace MeshLib::TMeshLib;

class CTetParaShelling : public CTetShelling , public _tetParameterization {};

typedef TInterface<CTVertex, CVertexD3Parameterization , CHalfEdge, CTEdge, CEdge, CHalfFace, CFaceD3Parameterization, CTetParaShelling> TIf;
typedef CTetSheller<TIf> CTSheller;
typedef D3Parameterization<TIf> D3Para;
typedef TIterators<TIf> TIt;
typedef TIf::TMeshType MyTMesh;

D3Para * pd3Para;
TIf::TMeshPtr pMesh(new TIf::TMeshType);
std::shared_ptr<std::list<CTetParaShelling *>> pShellingList;
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
	CTSheller sheller(pMesh);
	//CTetShelling * p_startTet = pMesh->idTet(10000);
	std::list<TIf::TPtr> beginList;

	auto isBoundaryTet = [&sheller](TIf::TPtr pTet) {
		if (sheller.numFaceOnSurfaceInShelling(pTet) > 0) {
			return true;
		}
		return false;
	};

	auto pBoundryTetIter = std::find_if<>(pMesh->tets().begin(), pMesh->tets().end(), isBoundaryTet);

	beginList.push_back(*pBoundryTetIter);
	sheller.shellingBreadthFirstGreedy(beginList);

	pShellingList = sheller.getShellingOrder();
	
	D3Para d3Para(pMesh, pShellingList);
	pd3Para = &d3Para;
	sphere.center = CPoint(0,0,0);
	sphere.radius = 1.0;

	init_openGL();
	//mesh._write_t("D:\\Data\\tet\\FastOutDemo.t");
	//std::cout << "Save done.\n";
	getchar();
	return 0;
}

