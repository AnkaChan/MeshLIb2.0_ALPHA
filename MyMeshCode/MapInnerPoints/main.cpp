#include <MeshLib\toolbox\TMeshLibGeneralDef.h>
#include <MeshLib\core\Geometry\BaryCoordinates4D.h>
#include "InnerMap.h"

typedef TInterface<CTVertex, CVertexWithImageVPtrAndNewPos, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTetWithBaryCoords> TIf;
typedef TIterators<TIf> TIt;

int main(int argc, char ** argv) {
	if (argc < 4) {
		cout << "Please give a path to surface TMesh, a path to spherical surface TMesh and a path to original TMesh (in such order). " << endl;
	}

	string surfaceTMeshPath = argv[1];
	string sphericalSurfaceTMeshPath = argv[2];
	string originalTMeshPath = argv[3];

	TIf::TMeshPtr pSurfaceTMesh = new TIf::TMeshType;
	pSurfaceTMesh->_load_t(surfaceTMeshPath.c_str());
	TIf::TMeshPtr pSphericalSurfaceTMesh = new TIf::TMeshType;
	pSphericalSurfaceTMesh->_load_t(surfaceTMeshPath.c_str());
	TIf::TMeshPtr pOriginalTMesh = new TIf::TMeshType;
	pOriginalTMesh->_load_t(surfaceTMeshPath.c_str());

	CInnerMap InnerMap(pSurfaceTMesh, pSphericalSurfaceTMesh, pOriginalTMesh);
	InnerMap.buildSTMeshKDTree();

	for (auto pV : TIt::TM_VIterator(pOriginalTMesh)) {
		cout << "Vertex: " << pV->id() << " in original TMesh.\n";
		auto pVS = InnerMap.mapVertexFromOriginalTMeshToSurfaceTMesh(pV);
		cout << "It's nearest point in surface TMesh: " << pVS->id() << endl;
	}

	TIf::TPtr pT = pOriginalTMesh->tets().front();
	CBaryCoordinates4D barys(pT);
	
	CPoint centroid;
	cout << "Vertices:\n";
	for (auto pV : TIt::T_VIterator(pT)) {
		centroid += pV->position();
		cout << pV->position() << "\n";
	}
	centroid /= 4;
	CPoint p = barys.bary2Descartes(CPoint4(0.25, 0.25, 0.25, 0.25));
	cout << "Bary to Descartes: " << p << "\n";
	CPoint4 b = barys.descartes2Bary(centroid);
	cout << "Centriod: " << centroid << "\n";
	cout << "Descartes to bary: " << b << "\n";

	cout << "(0, 0, 1)'s bary: " << barys.descartes2Bary(CPoint(0,0,2)) <<"\n";
	cout << "2*centriod's bary: " << barys.descartes2Bary(2*centroid) << "\n";

	getchar();
}