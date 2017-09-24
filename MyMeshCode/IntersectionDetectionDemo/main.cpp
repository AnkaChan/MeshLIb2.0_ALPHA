#include <iostream>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/toolbox/TMeshLibGeneralDef.h>
#include "IntersectionDetection.h"
using std::cout;
using std::endl;
using namespace MeshLib;
TIfGeneral::TMeshPtr pTMesh;

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cout << "Please give a input directory." << std::endl;
		return 0;
	}

	pTMesh = new::TIfGeneral::TMeshType;
	pTMesh->_load_t(argv[1]);

	//Test if overflipped tet exists.
	for (auto pT : TItGeneral::TM_TIterator(pTMesh)) {
		double oV = TIfGeneral::TetOrientedVolume(pT);
		if (oV < 0) {
			cout << "TMesh has over flipped tet.\n";
			cout << "It's oriented volume: " << oV;
			return 0;
		}
	}

	//test edge face intersection
	CIntersectionDetection detecter;
	CPoint v[3];
	int fIds[3];
	int numIntersection = 0;
	int count = 0;
	double numE = pTMesh->edges().size();
	double percentage = 0;
	double lastPercentage = 0;

	for (auto pE : TItGeneral::TM_EIterator(pTMesh)) {
		if (!(count % 100)) {
			percentage = (double)count / numE;
			if (percentage - lastPercentage > 0.01) {
				cout << percentage * 100 << "% completed.\n";
				lastPercentage = percentage;
			}
		}

		detecter.setLineSegment(TIfGeneral::EdgeVertex1(pE)->position(), TIfGeneral::EdgeVertex2(pE)->position());
		for (auto pF : TItGeneral::TM_FIterator(pTMesh)) {
			int i = 0;
			for (auto pV : TItGeneral::F_VIterator(pF)) {
				v[i] = pV->position();
				fIds[i] = pV->id();
				++i;
			}
			detecter.setTriangle(v[0], v[1], v[2]);
			if (detecter.detectApproximately()) {
				++numIntersection;
				cout << "Intersection detected.\n"
					<< "Edge: " << TIfGeneral::EdgeVertex1(pE)->id() << "-" << TIfGeneral::EdgeVertex2(pE)->id()
					<< " intersects with face: "
					<< fIds[0] << "-" << fIds[1] << "-" << fIds[2] 
					<< "\nAt: " << detecter.intersectPoint() << endl;
 			}
		}
		++count;
	}

	cout << "Detection Done! " << numIntersection << " intersections " << "has been detected." << endl;

	getchar();
}