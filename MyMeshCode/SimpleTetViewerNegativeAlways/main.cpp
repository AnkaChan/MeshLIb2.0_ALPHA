#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/core/viewer/SimpleTetViewer.h>
#include <iostream>
using namespace MeshLib;
using namespace TMeshLib;
using std::cout;
using std::endl;
#define DEFAULT_FACE_COLOR 0.8,0.8,0.8

typedef TInterface<CTVertex, CVertexVisual, CHalfEdge, CTEdge, CEdgeVisual, CHalfFaceVisual, CFace, CTetVisual> TIF;
typedef TIterators<TIF> TIT;

TIF::TPtr pOverlapT;
double step = 0.05;
CSimpleTetViewer * pViewer;
void adjustVertice() {
	CPoint d[4];

	int k = 0;
	for (auto pTV : TIT::T_TVIterator(pOverlapT)) {
		TIF::HFPtr  pHF = TIF::TVertexOppositeHalfFace(pTV);
		CPoint & A = TIF::TVertexVertex(pTV)->position();
		CPoint V[3];
		cout << "Adjusting vertex: " << pTV->vert()->id() << "\n";
		cout << "Opposite Halfface: ";
		int i = 0;
		for (auto pV : TIT::HF_VIterator(pHF)) {
			V[i] = pV->position();
			++i;
			cout << pV->id() << ", ";
		}
		CPoint BA = A - V[0];
		CPoint BC = V[0] - V[1];
		CPoint BD = V[0] - V[2];

		cout << "\nOriented volume before: " << ( BA ^ BC ) * BD << "\n";

		d[k] = CPoint(BC[1] * BD[2] - BD[1] * BC[2], -(BC[0] * BD[2] - BD[0] * BC[2]), BC[0] * BD[1] - BD[0] * BC[1]);
		d[k] = - d[k] / d[k].norm();
		CPoint OA = A;
		d[k] = d[k] - (d[k] * OA)*OA;
		d[k] = d[k] / d[k].norm();
		//cout << "\nOriented volume after: " << AB * (AC ^ AD) << endl;
		cout << "Move direction: " << d[k] << endl;
		++k;

	}
	k = 0;
	for (auto pTV : TIT::T_TVIterator(pOverlapT)) {
		CPoint & A = TIF::TVertexVertex(pTV)->position();
		A += step * d[k];
		A = A / A.norm();
		++k;
	}
}

void myKeyFunc(unsigned char key) {
	switch (key)
	{
	/*case 'n':
		adjustVertice();
		break;*/
	case 'e':
		if (pViewer->setting().edgeColorMode == GLTetSetting::none) {
			pViewer->setting().edgeColorMode = GLTetSetting::userDefined;
		}
		else {
			pViewer->setting().edgeColorMode = GLTetSetting::none;
		}
		break;
	case 'f':
		if (pViewer->setting().halfFaceColorMode == GLTetSetting::none) {
			pViewer->setting().halfFaceColorMode = GLTetSetting::userDefined;
		}
		else {
			pViewer->setting().halfFaceColorMode = GLTetSetting::none;
		}
		break;
	default:
		break;
	}
}

double orientedVolume(TIF::TPtr pT) {
	CPoint A = pT->vertex(0)->position();
	CPoint B = pT->vertex(1)->position();
	CPoint C = pT->vertex(2)->position();
	CPoint D = pT->vertex(3)->position();
	CPoint AB = B - A;
	CPoint AC = C - A;
	CPoint AD = D - A;

	double orientation_product = AB * (AC ^ AD);
	return orientation_product;
}

int countBoundaryHF(TIF::TPtr pT) {
	int numBHF = 0;
	for (TIF::HFPtr pHF : TIT::T_HFIterator(pT)) {
		if (TIF::HalfFaceDual(pHF) == NULL) {
			++numBHF;
		}
	}
	return numBHF;
}
int main(int argc, char **argv){
	if (argc < 2) {
		std::cout << "Please give a input file." << std::endl;
		return 0;
	}
	char * inDir = argv[1];


	TIF::TMeshPtr pTMesh = new TIF::TMeshType;
	pTMesh->_load_t(inDir);
	CSimpleTetViewer tViewer;
	pViewer = &tViewer;
	tViewer.setting().drawAxis = true;
	//tViewer.setting().renderAllEdges = true;
	tViewer.setting().useCustomEdgeSize = true;
	tViewer.setting().edgeColorMode = GLTetSetting::userDefined;
	tViewer.setting().halfFaceColorMode = GLTetSetting::userDefined;
	//tViewer.setting().halfFaceColorMode = GLTetSetting::none;

	double s = 1.0;
	int i = 1;
	for (auto pE : TIT::TM_EIterator(pTMesh)) {
		
			pE->size = 1.0;
			pE->r = 1.0;
			pE->g = 0;
			pE->b = 0;

		++i;
	}
	int negativeCount = 0;
	int type0count = 0;
	int type1count = 0;
	int type2count = 0;
	for (auto pT : TIT::TM_TIterator(pTMesh)) {
		//std::cout << "The oriented volume for Tet " << pT->id() << ": " << orientedVolume(pT) << std::endl;
		double oT = orientedVolume(pT);
		if (oT < 0) {
			++negativeCount;
			int numBF = countBoundaryHF(pT);
			std::cout << "Negative oriented volume: " << oT << ". Type: " << numBF << "." << std::endl;
			for (TIF::HFPtr pHF : TIT::T_HFIterator(pT)) {
				switch (numBF)
				{
				case 0:
					pHF->color[0] = 0.9;
					pHF->color[1] = 0.3;
					pHF->color[2] = 0.3;
					++type0count;
					break;
				case 1:
					pHF->color[0] = 0.8;
					pHF->color[1] = 0.8;
					pHF->color[2] = 0.2;
					++type1count;
					break;
				case 2:
					pHF->color[0] = 0.2;
					pHF->color[1] = 0.8;
					pHF->color[2] = 0.2;
					++type2count;
					break;
				default:
					pHF->color[0] = 0.2;
					pHF->color[1] = 0.2;
					pHF->color[2] = 0.8;
					break;
				}
				
			}
			pT->alwaysVisible = true;
			//getchar();
		}
		else {
			for (TIF::HFPtr pHF : TIT::T_HFIterator(pT)) {
				pHF->color[0] = 0.8;
				pHF->color[1] = 0.8;
				pHF->color[2] = 0.8;
			}
		}
	}
	// Ugly but just let me use this.
	type0count /= 4;
	type1count /= 4;
	type2count /= 4;
	std::cout << "Totally " << negativeCount << " of negative oriented volume." << std::endl;
	std::cout << "There is: " << type0count << " of type 0 tets." << std::endl;
	std::cout << "There is: " << type1count << " of type 1 tets." << std::endl;
	std::cout << "There is: " << type2count << " of type 2 tets." << std::endl;
	pOverlapT = pTMesh->idTet(4);

	tViewer.setMeshPointer(pTMesh);
	tViewer.normalizeTMesh();
	tViewer.setUserKeyFunc(myKeyFunc);
	tViewer.show();
	//Mesh->_write_t("D:/GoogleDrive/Data/Tet/D3/3HalfWheel_O.t");
}