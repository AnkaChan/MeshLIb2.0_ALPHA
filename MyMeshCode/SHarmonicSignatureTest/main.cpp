#include <MeshLib/toolbox/MeshLibGeneralDef.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#include "IntersectionDetection.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
using namespace MeshLib;
using std::cout;
using std::endl;

typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> If;
typedef CIterators<If> It;

If::MeshPtr pOMesh = new If::MeshType;
If::MeshPtr pSMesh = new If::MeshType;

double orientedVolume(CPoint * VPs) {
	CPoint A = VPs[0];
	CPoint B = VPs[1];
	CPoint C = VPs[2];
	CPoint D = VPs[3];
	CPoint AB = B - A;
	CPoint AC = C - A;
	CPoint AD = D - A;

	double orientation_product = AB * (AC ^ AD);
	return orientation_product;
}

void myKeyFunc(unsigned char key) {
	static std::default_random_engine generator(time(NULL));
	int numV = pOMesh->vertices().size();
	std::uniform_int_distribution<int> randInt(1, numV);

	switch (key)
	{

	case 'n':
		CPoint VPs[4];
		vector<int> VIDs(4);
		
			CIntersectionDetection IntersectionDetect;
		while (true) {
			for (auto pV : It::MVIterator(pOMesh)) {
				pV->user_defined_visbility = false;
				pV->r = 0.0;
				pV->g = 0.0;
				pV->b = 0.0;
			}
			for (int i = 0; i < 4; ++i) {
				do {
					VIDs[i] = randInt(generator);
				} while (std::find(VIDs.begin(), VIDs.end(), VIDs[i]) != VIDs.begin() + (i));
			}

			for (int i = 0; i < 4; ++i) {
				If::VPtr pV = pOMesh->idVertex(VIDs[i]);
				pV->user_defined_visbility = true;
				VPs[i] = pV->point();
				pV->r = 1.0;
				pV->g = 0.3;
				pV->b = 0.2;
			}
			double oVO = orientedVolume(VPs);
			cout << "Original oriented volume: " << oVO << endl;

			CPoint SVPs[4];
			for (int i = 0; i < 4; ++i) {
				If::VPtr pV = pSMesh->idVertex(VIDs[i]);
				SVPs[i] = pV->point();
			}
			double oVS = orientedVolume(SVPs);
			cout << "Spherical oriented volume: " << oVS << endl;
			if (oVO * oVS > 0) {
				cout << "Sign didn't change." << endl;
			}
			else
			{
				cout << "Sign changed !!" << endl;
				break;
			}
		}
		size_t linesS[6] = { 0, 0, 0, 1, 1, 2 };
		size_t linesT[6] = { 1, 2, 3, 2, 3, 3 };
		CPoint FVs[3];
		for (auto pF : It::MFIterator(pOMesh)) {
			int i = 0;

			for (size_t i = 0; i < 6; i++){
				IntersectionDetect.setLineSegment(VPs[linesS[i]], VPs[linesT[i]]);
				i = 0;
				for (auto pV : It::FVIterator(pF)) {
					FVs[i] = pV->point();
					++i;
				}
				IntersectionDetect.setTriangle(FVs[0], FVs[1], FVs[2]);
				if (IntersectionDetect.detectApproximately()) {
					cout << "Edge Face intersection detected!" << endl;
					If::VPtr pV = pOMesh->idVertex(VIDs[linesS[i]]);
					pV->r = 0.0;
					pV->g = 0.3;
					pV->b = 1.0;
					pV = pOMesh->idVertex(VIDs[linesT[i]]);
					pV->r = 0.0;
					pV->g = 0.3;
					pV->b = 1.0;
					return;
				}
			}
		}
		break;
	}
}


int main(int argc, char **argv){
	if (argc < 3) {
		std::cout << "Please give a dir to original mesh and a dir to spherical mesh." << std::endl;
		return 0;
	}
	char * OMeshDir = argv[1];
	char * SMeshDir = argv[2];

	pOMesh->read_m(OMeshDir);
	pSMesh->read_m(SMeshDir);


	CSimpleMeshViewer viewer(pOMesh);
	viewer.setting().vertexColorMode = GLSetting::userDefined;
	viewer.setting().customVertexVisibility = true;
	viewer.setting().vertexSize = 10.0;
	viewer.setUserKeyFunc(myKeyFunc);
	viewer.show();
	//Mesh->_write_t("D:/GoogleDrive/Data/Tet/D3/3HalfWheel_O.t");
}