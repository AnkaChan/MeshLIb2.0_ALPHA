#include <iostream>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/toolbox/FileIO.h>
#include "VolumetricHarmonic.h"
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace TMeshLib;

typedef TInterface<CTVertex, CVertexVHarmonic, CHalfEdge, CTEdge, CEdgeVHarmonic, CHalfFace, CFace, CTet> TIf;
typedef TIterators<TIf> TIt;
typedef VolumetricHarmonic<TIf> CVolumetricHarmonicMap;

int main(int argc, char ** argv) {
	if (argc < 3) {
		cout << "Please give input path and a inital map path." << endl;
		return 0;
	}

	char * inPath = argv[1];
	char * initialMapPath = argv[2];
	FileParts fp = fileparts(inPath);
	std::string outPath = fp.path + fp.name + "_sHarmonic.t";
	CVolumetricHarmonicMap VHMapper;
	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	pTMesh->_load_t(inPath);
	TIf::TMeshPtr pInitialMapTMesh = new TIf::TMeshType;
	pInitialMapTMesh->_load_t(initialMapPath);
	VHMapper.setpTMesh(pTMesh);
	VHMapper.calculateEdgeWeights();
	VHMapper.setEpison(0.00000001);
	VHMapper.setStep(0.0001);
	VHMapper.setInitialMapOnBoundary(pInitialMapTMesh);
	VHMapper.dynamicStep = true;

	//VHMapper.setInitialMap(pInitialMapTMesh);
	VHMapper.adjustVerticesBoundaryHarmonic();
	pTMesh->_write_t(outPath.c_str(), true);
}