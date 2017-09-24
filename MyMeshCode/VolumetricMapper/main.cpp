#include <MeshLib/toolbox/MeshLibGeneralDef.h>
#include <MeshLib/toolbox/FileIO.h>
#include "VolumetricMapper.h"

using namespace MeshLib;
int main(int argc, char ** argv) {
	if (argc < 4) {
		cout << "Please give a path to surface mesh(.m), a path to source TMesh(.t), a path to target TMesh(.t) (in such order), and a interger of layers(default 5) . " << endl;
		return 0;
	}
	int numLayer = 5;
	if (argc == 5) {
		numLayer = atoi(argv[4]);
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
		//double depth = double(i + 1) / double(numLayer + 1);
		double depth = 0.99;
		for (auto pV : ItGeneral::MVIterator(&oMesh)) {
			CPoint oP = pV->point();
			CPoint & tP = tMesh.idVertex(pV->id())->point();

			tP = volumertricMapper.map( depth * oP);
		}

		char layerCount[10];
		string saveName = fp.path + fp.name + string("_layer") + itoa(i + 1, layerCount, 10) + ".m";

		cout << "Save to: " << saveName << endl;
		tMesh.write_m(saveName.c_str());
	}
	cout << "Done." << endl;
	return 0;
}