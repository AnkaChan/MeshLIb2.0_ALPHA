#include <MeshLib/toolbox/MeshLibGeneralDef.h>
#include <MeshLib/toolbox/FileIO.h>
#include "VolumetricMapper.h"

int main(int argc, char ** argv) {
	if (argc < 4) {
		cout << "Please give a path to surface mesh(.m), a path to source TMesh(.t), a path to target TMesh(.t) (in such order), and a interger of layers(default 5) . " << endl;
		return 0;
	}
	int numLayer = 5;
	if (argc == 5) {
		//numLayer << ""
	}

	string surfaceMeshPath = argv[1];
	string sourceTMeshPath = argv[2];
	string targetTMeshPath = argv[3];

	FileParts fp = fileparts(surfaceMeshPath);

	IfGeneral::MeshType oMesh;
	IfGeneral::MeshType tMesh;
	oMesh.read_m(surfaceMeshPath.c_str());
	tMesh.read_m(surfaceMeshPath.c_str());

	CVolumetricMapper volumertricMapper;
	volumertricMapper.readSourceTMesh(sourceTMeshPath.c_str());
	volumertricMapper.readTargetTMesh(targetTMeshPath.c_str());

	for (int i = 0; i < numLayer; i++)
	{
		for (auto pV : ItGeneral::MVIterator(&mesh)) {

		}
	}

}