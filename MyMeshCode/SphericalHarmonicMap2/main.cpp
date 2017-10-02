#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include "SphericalHarmonicMap.h"
#include <MeshLib/core/Mesh/boundary.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
#include <set>
using std::cout; 
using std::endl;
using namespace MeshLib;

class CVertexSH : public CVertex, public _vectexSphericalHarmonic {};
class CEdgeSH : public CEdge, public _edgeSphericalHarmonic {};
typedef CInterface<CVertexSH, CEdgeSH, CFace, CHalfEdge> If;
typedef CIterators<If> It;
typedef SphericalHarmonicMap<If>  SHMapper;
SHMapper * pMapper;
void shMapIdlefunc() {
	static bool succeeded = false;
	if (succeeded) {
		return;
	}
	if (succeeded = pMapper->adjustPointVisualOneStep()) {
		cout << "Algorithm Succeeded." << endl;
	}

}

void smoothMesh(If::MeshPtr pMesh) {
	std::set<If::VPtr, CPoint> newPointSet;
	for (auto pV : It::MVIterator(pMesh)) {
		CPoint newP;
		int numNV = 0;
		for (auto pNV : It::VVIterator(pV)) {
			newP += pNV->point();
		}
		newP /= numNV;
		//newPointSet.insert(pV, newP);

	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Please give input path." << endl;
		return 0;
	}

	char * inPath = argv[1];
	char * initalPath;
	FileParts fp = fileparts(inPath);

	If::MeshPtr pMesh = new If::MeshType;
	If::MeshPtr pInitalMapMesh = new If::MeshType;

	if (fp.ext == ".obj") {
		pMesh->read_obj(inPath);
	}
	else if (fp.ext == ".m") {
		pMesh->read_m(inPath);
	}
	else {
		cout << "Unsupported file format: " << fp.ext << endl;
	}
	SHMapper shMapper;
	shMapper.setInputMesh(pMesh);

	CSimpleMeshViewer viewer(pMesh, true);

	viewer.setting().edgeColorMode = GLSetting::defaultColor;
	viewer.show();

	if (argc = 3) {
		initalPath = argv[2];
		FileParts fp2 = fileparts(initalPath);

		if (fp2.ext == ".obj") {
			pInitalMapMesh->read_obj(initalPath);
		}
		else if (fp2.ext == ".m") {
			pInitalMapMesh->read_m(initalPath);
		}
		else {
			cout << "Unsupported file format: " << fp2.ext << endl;
		}
	}
	shMapper.setInitalMap(pInitalMapMesh);
	

	//shMapper.guassianMap();
	shMapper.setStep(0.01);
	shMapper.setStopEpsion(0.00000001);
	//shMapper.dynamicStep = true;
	//shMapper.centerVisualMap(CPoint(-0.560882, -0.630, 0));
	//shMapper.centerVisualMap(CPoint(0,0,0));
	viewer.show();
	pMapper = &shMapper;
	//viewer.setUserIdleFunc(shMapIdlefunc);
	//shMapper.iterativelyAdjustPoint();
	shMapper.iterativelyAdjustPoint();
	viewer.show();
	std::string outPath = fp.path + fp.name + "Spherical" + ".m";
	std::string outPathObj = fp.path + fp.name + "Spherical" + ".obj";
	cout << "Save to: " << outPath << endl;
	pMesh->write_m_high_precision(outPath.c_str());
	pMesh->write_obj(outPathObj.c_str());

}
