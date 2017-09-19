#include <MeshLib\toolbox\TMeshLibGeneralDef.h>
#include <MeshLib\core\Geometry\BaryCoordinates4D.h>
#include <MeshLib\toolbox\FileIO.h>
#include "InnerMap.h"

typedef TInterface<CTVertex, CVertexWithImageVPtrAndNewPos, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTetWithBaryCoords> TIf;
typedef TIterators<TIf> TIt;

int main(int argc, char ** argv) {
	if (argc < 4) {
		cout << "Please give a path to surface TMesh, a path to spherical surface TMesh and a path to original TMesh (in such order). " << endl;
		return 0;
	}

	string surfaceTMeshPath = argv[1];
	string sphericalSurfaceTMeshPath = argv[2];
	string originalTMeshPath = argv[3];
	FileParts fp = fileparts(originalTMeshPath);

	TIf::TMeshPtr pSurfaceTMesh = new TIf::TMeshType;
	pSurfaceTMesh->_load_t(surfaceTMeshPath.c_str());
	TIf::TMeshPtr pSphericalSurfaceTMesh = new TIf::TMeshType;
	pSphericalSurfaceTMesh->_load_t(sphericalSurfaceTMeshPath.c_str());
	TIf::TMeshPtr pOriginalTMesh = new TIf::TMeshType;
	pOriginalTMesh->_load_t(originalTMeshPath.c_str());

	CInnerMap InnerMap(pSurfaceTMesh, pSphericalSurfaceTMesh, pOriginalTMesh);
	InnerMap.changePointsOnOriginalTMesh();

	string outPath = fp.path + fp.name + "_mapped.t";
	pOriginalTMesh->_write_t(outPath.c_str());
	getchar();
}