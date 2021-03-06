#include <iostream>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/toolbox/FileIO.h>
#include "NAVolumetricHarmonic.h"
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace TMeshLib;

typedef TInterface<CTVertex, CVertexVHarmonic, CHalfEdge, CTEdge, CEdgeVHarmonic, CHalfFace, CFace, CTet> TIf;
typedef TIterators<TIf> TIt;
typedef NAVolumetricHarmonic<TIf> CNAVolumetricHarmonicMap;

int main(int argc, char ** argv) {
	if (argc < 3) {
		cout << "Please give input path and a inital map path." << endl;
		return 0;
	}

	char * inPath = argv[1];
	char * initialMapPath = argv[2];
	FileParts fp = fileparts(inPath);
	std::string outPath = fp.path + fp.name + "_sphereHamonic.t";
	CNAVolumetricHarmonicMap VHMapper;
	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	pTMesh->_load_t(inPath);
	TIf::TMeshPtr pInitialMapTMesh = new TIf::TMeshType;
	pInitialMapTMesh->_load_t(initialMapPath);
	VHMapper.setpTMesh(pTMesh);
	VHMapper.calculateEdgeWeights();
	VHMapper.setEpison(0.00001);
	VHMapper.setStep(0.00001);
	VHMapper.setInitialMap(pInitialMapTMesh);
	VHMapper.adjustVertices();
	pTMesh->_write_t(outPath.c_str());
}