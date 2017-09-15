#include <MeshLib\toolbox\TMeshLibGeneralDef.h>
#include <MeshLib\core\Geometry\BaryCoordinates4D.h>
#include "InnerMap.h"

int main(int argc, char ** argv) {
	if (argc < 4) {
		cout << "Please give a path to surface TMesh, a path to spherical surface TMesh and a path to original TMesh (in such order). " << endl;
	}

	string surfaceTMeshPath = argv[1];
	string sphericalSurfaceTMeshPath = argv[2];
	string originalTMeshPath = argv[3];

	TIfGeneral::TMeshPtr pSurfaceTMesh = new TIfGeneral::TMeshType;
	pSurfaceTMesh->_load_t(surfaceTMeshPath.c_str());
	TIfGeneral::TMeshPtr pSphericalSurfaceTMesh = new TIfGeneral::TMeshType;
	pSphericalSurfaceTMesh->_load_t(surfaceTMeshPath.c_str());
	TIfGeneral::TMeshPtr pOriginalTMesh = new TIfGeneral::TMeshType;
	pOriginalTMesh->_load_t(surfaceTMeshPath.c_str());

	CInnerMap InnerMap(pSurfaceTMesh, pSphericalSurfaceTMesh, pOriginalTMesh);
	InnerMap.buildSTMeshKDTree();

	for (auto pV : TItGeneral::TM_VIterator(pOriginalTMesh)) {
		cout << "Vertex: " << pV->id() << " in original TMesh.\n";
		auto pVS = InnerMap.mapOriginalTMeshToSurfaceTMesh(pV);
		cout << "It's nearest point in surface TMesh: " << pVS->id() << endl;
	}

	TIfGeneral::TPtr pT = pOriginalTMesh->tets().front();
	CBaryCoordinates4D barys(pT);
	
	CPoint centroid;
	cout << "Vertices:\n";
	for (auto pV : TItGeneral::T_VIterator(pT)) {
		centroid += pV->position();
		cout << pV->position() << "\n";
	}
	centroid /= 4;
	CPoint p = barys.bary2Descartes(CPoint4(0.5, 0.5, 0.5, 0.5));
	cout << "Bary to Descartes: " << p << "\n";
	CPoint4 b = barys.descartes2Bary(centroid);

	getchar();
}