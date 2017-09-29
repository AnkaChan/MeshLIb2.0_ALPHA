#include <iostream>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/toolbox/FileIO.h>
#include <omp.h>
#include <ctime>
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
	clock_t time1 = 0, time2 = 0;
	clock_t startT, endT;

	pTMesh->prepare_for_mp();
	CPoint p;

	startT = clock();

#pragma omp parallel for
	for (int j = 0; j < pTMesh->vertices_vec.size(); ++j) {
		TIf::VPtr  pV = pTMesh->vertices_vec[j];
		int sum = 0;
		for (int k = 0; k < 1000000; ++k) {
			sum += k;
		}
		pV->position()[0] = sum;
	}

	endT = clock();
	time1 = endT - startT;
	cout << p << endl;

	p = CPoint();
	startT = clock();
	for (TIf::VPtr pV : TIt::TM_VIterator(pTMesh)) {
		TIf::VPtr  pV = pTMesh->vertices_vec[j];
		int sum = 0;
		for (int k = 0; k < 1000000; ++k) {
			sum += k;
		}
		pV->position()[0] = sum;
	}
	endT = clock();
	time2 = endT - startT;

	cout << p << endl;
	cout << "Time 1:" << time1 << endl;
	cout << "Time 2:" << time2 << endl;

	getchar();

	VHMapper.calculateEdgeWeights();
	VHMapper.setEpison(0.00000000000001);
	VHMapper.setStep(0.0001);
	VHMapper.setInitialMapOnBoundary(pInitialMapTMesh);
	VHMapper.dynamicStep = true;
	//VHMapper.setInitialMap(pInitialMapTMesh);
	//VHMapper.adjustVerticesBoundaryHarmonic();
	VHMapper.adjustVerticesBoundaryHarmonic();
	pTMesh->_write_t(outPath.c_str(), true);
	getchar();
}