#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/Iterators2.h>
#include <MeshLib/core/Mesh/Types.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>

using namespace MeshLib;
void myKeyFunc(unsigned char key) {
	static GLViewer::ITGL::MFIterator mfIter(GLViewer::pMesh);
	GLViewer::IFGL::FPtr pF;
	switch (key)
	{
	case 'n':
		pF = *mfIter;
		pF->r = 0.0;
		pF->g = 0.0;
		pF->b = 0.0;
		++mfIter;
		break;
	default:
		break;
	}


}
class myVertex : public CVertexVisual {
public:
	int num = 0;

};
class myHalfedge : public CHalfEdge {
public:
	int num = 10;
};
class myEdge : public CEdgeVisual {
	int num = 222;
};
class myFace : public CFaceVisual {
	double i = 1223.2312;

};
using std::cout;
using std::endl;

int main() {
	typedef CInterface<CVertex, CEdge, CFace, CHalfEdge> IF;
	typedef CIterators<IF> IT;
	myVertex mV;
	mV.normal()[1] = 2;

	IF::MeshPtr pMesh = new IF::MeshType;

	pMesh->read_m("D:/Code/Data/Mesh/outputs/face125.m");


	cout << "Iterating Edges." << endl;
	for (auto pE : IT::MEIterator(pMesh)) {
		IF::VPtr pV1, pV2;
		pV1 = IF::edgeVertex1(pE);
		pV2 = IF::edgeVertex2(pE);

		cout << "The edge's length:" << IF::edgeLength(pE)
			<< "\nThe two vertice are: " << IF::vertexId(pV1) << ", " << IF::vertexId(pV2) << endl;

		cout << "Vertex 1: " << pV1->point() << endl;
		cout << "Vertex 2: " << pV2->point() << endl;

	}

	cout << "Iterating face." << endl;
	for (auto pF : IT::MFIterator(pMesh)) {
		cout << "Face : " << pF << endl;
		cout << "FaceId : " << IF::faceId(pF) << endl;
	}
	getchar();
	/*
	cout << "edgehalfedge test" << endl;
	for (auto pe : meIter)
	{
		IF::HEPtr ph = IF::edgeHalfedge(pe, 0);
		IF::VPtr pv0 = IF::halfedgeSource(ph),
			pv1 = IF::halfedgeTarget(ph);
		std::cout << "the first halfedge: id0 = " << pv0->id() << " id1 = " << pv1->id() << std::endl;
		ph = IF::edgeHalfedge(pe, 1);
		pv0 = IF::halfedgeSource(ph);
		pv1 = IF::halfedgeTarget(ph);
		std::cout << "the second halfedge: id0 = " << pv0->id() << " id1 = " << pv1->id() << std::endl;
	}
	*/


	/*
	* Testing Face IT.
	*/
	/*cout << "Iterating Faces using halfedge list:" << endl;
	cout << "The number of halfedges: " << pV->arhe().size() << endl;;*/
	for (auto pV : IT::MVIterator(pMesh)) {
		for (auto pNeiF : IT::VFIterator(pV)) {
			cout << "Neighbor face ID: " << IF::faceId(pNeiF) << "\n";
			cout << "It's vertices:";
			IT::FVIterator fvIter(pNeiF);
			for (auto pNeiV : fvIter) {
				cout << IF::vertexId(pNeiV) << " ";
			}
			cout << "\nIt's edges:";
			IT::FEIterator feIter(pNeiF);
			for (auto pNeiE : feIter) {
				cout << pNeiE << " ";
			}
			cout << "\nIt's halfedges:";
			IT::FHEIterator fheIter(pNeiF);
			for (auto pNeiE : fheIter) {
				cout << pNeiE << " ";
			}
			cout << "\n---------------------------\n";
		}
		cout << "\n-----------------------------------------------------------\n" << endl;
		/*
		* Testing edge IT.
		*/
		cout << "Iterating vertices using halfedge list:" << endl;
		cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		for (auto pNeiE : IT::VEIterator(pV)) {
			cout << "Neighbor edge length: " << IF::edgeLength(pNeiE);
			cout << " Edge's vertices id: " << IF::vertexId(IF::edgeVertex1(pNeiE)) << " , " << IF::vertexId(IF::edgeVertex2(pNeiE)) << endl;
		}

		cout << "-----------------------------" << endl;
		cout << "Iterating vertices using halfedge conection(CCW):" << endl;
		for (auto pNeiE : IT::VCcwEIterator(pV)) {
			cout << "Neighbor edge length: " << IF::edgeLength(pNeiE);
			cout << " Edge's vertices id: " << IF::vertexId(IF::edgeVertex1(pNeiE)) << " , " << IF::vertexId(IF::edgeVertex2(pNeiE)) << endl;
		}

		cout << "-----------------------------" << endl;
		cout << "Iterating vertices using halfedge conection(CLW):" << endl;
		for (auto pNeiE : IT::VClwEIterator(pV)) {
			cout << "Neighbor edge length: " << IF::edgeLength(pNeiE);
			cout << " Edge's vertices id: " << IF::vertexId(IF::edgeVertex1(pNeiE)) << " , " << IF::vertexId(IF::edgeVertex2(pNeiE)) << endl;
		}

		/*
		* Testing Vertex IT.
		*/
		cout << "Iterating vertices using halfedge list:" << endl;
		cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		for (auto pNeiV : IT::VVIterator(pV)) {
			cout << "Neighbor Vertex id: " << IF::vertexId(pNeiV) << " Position: " << pNeiV->point() << endl;
		}

		cout << "-----------------------------" << endl;
		cout << "Iterating vertices using halfedge conection(CCW):" << endl;
		for (auto pNeiV : IT::VCcwVIterator(pV)) {
			cout << "Neighbor Vertex id: " << IF::vertexId(pNeiV) << " Position: " << pNeiV->point() << endl;
		}

		cout << "-----------------------------" << endl;
		cout << "Iterating vertices using halfedge conection(CLW):" << endl;
		for (auto pNeiV : IT::VClwVIterator(pV)) {
			cout << "Neighbor Vertex id: " << IF::vertexId(pNeiV) << " Position: " << pNeiV->point() << endl;
		}

		IT::VOutHEIterator voutheIter(pV);
		for (auto pHE : voutheIter) {
			cout << "The halfedge: " << pHE << endl;
			IF::VPtr pTargetV = IF::halfedgeTarget(pHE);
			cout << "Target vertex id: " << IF::vertexId(pTargetV) << endl;
		}
		cout << "-----------------------------" << endl;
		cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		for (IF::HEPtr pHE : IT::VCcwOutHEIterator(pV)) {
			std::cout << "The halfedge: " << pHE << std::endl;
			IF::VPtr pTargetV = IF::halfedgeTarget(pHE);
			IF::VPtr pSourceV = IF::halfedgeSource(pHE);
			cout << "Target vertex id: " << IF::vertexId(pTargetV) << " Source vertex id: " << IF::vertexId(pSourceV) << endl;
		}
		cout << "-----------------------------" << endl;
		cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		for (IF::HEPtr pHE : IT::VClwInHEIterator(pV)) {
			std::cout << "The halfedge: " << pHE << std::endl;
			IF::VPtr pTargetV = IF::halfedgeTarget(pHE);
			IF::VPtr pSourceV = IF::halfedgeSource(pHE);
			cout << "Target vertex id: " << IF::vertexId(pTargetV) << " Source vertex id: " << IF::vertexId(pSourceV) << endl;
		}
	}
	getchar();
}
	