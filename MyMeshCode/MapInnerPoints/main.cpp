#include <MeshLib\toolbox\TMeshLibGeneralDef.h>

int main(int argc, char ** argv) {
	if (argc < 4) {
		cout << "Please give a path to surface TMesh, a path to spherical surface TMesh and a path to original TMesh (in such order). " << endl;
	}

	string surfaceTMeshPath = argv[1];
	string sphericalSurfaceTMeshPath = argv[2];
	string originalTMeshPath = argv[3];

	TIfGeneral::TMeshPtr pSurfaceTMesh = new TIfGeneral::TMeshType;
	pSurfaceTMesh->_load(surfaceTMeshPath.c_str());
	TIfGeneral::TMeshPtr pSphericalSurfaceTMesh = new TIfGeneral::TMeshType;
	pSphericalSurfaceTMesh->_load(surfaceTMeshPath.c_str());
	TIfGeneral::TMeshPtr pOriginalTMesh = new TIfGeneral::TMeshType;
	pOriginalTMesh->_load(surfaceTMeshPath.c_str());

	list<TIfGeneral::VPtr> original;
}