#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/iterators2.h>
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
	typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> Interface;
	typedef CIterators<Interface> Iterators;
	typedef Interface::MeshType CMesh;

	myVertex mV;
	mV.normal()[1] = 2;

	CMesh mesh;
	mesh.read_m("D:/Code/Data/Mesh/outputs/face125.m");
	for (auto pF : Iterators::MFIterator(&mesh)) {
		pF->r = 1.0;
		pF->g = 1.0;
		pF->b = 0.0;
	}
	for (auto pE : Iterators::MEIterator(&mesh)) {
		if (Interface::edgeFace1(pE)->id() > 100) {
			pE->r = 0.0;
			pE->g = 0.0;
			pE->b = 0.0;
		}
		else {
			pE->r = 0.0;
			pE->g = 0.0;
			pE->b = 1.0;

		}
	}
	CSimpleMeshViewer simpleViewer(&mesh);
	simpleViewer.show();
	//simpleViewer.setting().edgeSize = 20.0;
	simpleViewer.setting().faceColorMode = GLSetting::userDefined;
	simpleViewer.setting().vertexColorMode = GLSetting::defaultColor;
	simpleViewer.setting().vertexSize = 15.0;
	simpleViewer.setUserKeyFunc(myKeyFunc);
	simpleViewer.show();
	//GLViewer::IFGL::MeshPtr m_pmesh = GLViewer::pMesh;
	//simpleViewer.setting().edgeSize = 20.0;
	//simpleViewer.setting().edgeColorMode = GLSetting::defaultColor;
	//simpleViewer.show();
	//cout << "Iterating Edges." << endl;
	//for (auto pE : meIter) {
	//	Interface::VPtr pV1, pV2;
	//	pV1 = Interface::edgeVertex1(pE);
	//	pV2 = Interface::edgeVertex2(pE);

	//	cout << "The edge's length:" << Interface::edgeLength(pE)
	//		<< "\nThe two vertice are: " << Interface::vertexId(pV1) << ", " << Interface::vertexId(pV2) << endl;

	//	cout << "Vertex 1: " << pV1->point() << endl;
	//	cout << "Vertex 2: " << pV2->point() << endl;

	//}

	//Iterators::MFIterator mfIter(&mesh);
	//cout << "Iterating face." << endl;
	//for (auto pF : mfIter) {
	//	cout << "Face : " << pF << endl;
	//	cout << "FaceId : " << Interface::faceId(pF) << endl;
	//}
	//getchar();
	/*
	cout << "edgehalfedge test" << endl;
	for (auto pe : meIter)
	{
		Interface::HEPtr ph = Interface::edgeHalfedge(pe, 0);
		Interface::VPtr pv0 = Interface::halfedgeSource(ph),
			pv1 = Interface::halfedgeTarget(ph);
		std::cout << "the first halfedge: id0 = " << pv0->id() << " id1 = " << pv1->id() << std::endl;
		ph = Interface::edgeHalfedge(pe, 1);
		pv0 = Interface::halfedgeSource(ph);
		pv1 = Interface::halfedgeTarget(ph);
		std::cout << "the second halfedge: id0 = " << pv0->id() << " id1 = " << pv1->id() << std::endl;
	}
	*/

	
		///*
		//* Testing Face iterators.
		//*/
		//Iterators::VFIterator vfIter(pV);
		//cout << "Iterating Faces using halfedge list:" << endl;
		//cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		//for (auto pNeiF : vfIter) {
		//	cout << "Neighbor face ID: " << Interface::faceId(pNeiF) << "\n";
		//	cout << "It's vertices:";
		//	Iterators::FVIterator fvIter(pNeiF);
		//	for (auto pNeiV : fvIter) {
		//		cout << Interface::vertexId(pNeiV) <<  " ";
		//	}
		//	cout << "\nIt's edges:";
		//	Iterators::FEIterator feIter(pNeiF);
		//	for (auto pNeiE : feIter) {
		//		cout << pNeiE << " ";
		//	}
		//	cout << "\nIt's halfedges:";
		//	Iterators::FHEIterator fheIter(pNeiF);
		//	for (auto pNeiE : fheIter) {
		//		cout << pNeiE << " ";
		//	}
		//	cout << "\n---------------------------\n";
		//}
		//cout << "\n-----------------------------------------------------------\n" << endl;

		///*
		//* Testing edge iterators.
		//*/
		//Iterators::VEIterator veIter(pV);
		//cout << "Iterating vertices using halfedge list:" << endl;
		//cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		//for (auto pNeiE : veIter) {
		//	cout << "Neighbor edge length: " << Interface::edgeLength(pNeiE);
		//	cout << " Edge's vertices id: " << Interface::vertexId(Interface::edgeVertex1(pNeiE)) << " , " << Interface::vertexId(Interface::edgeVertex2(pNeiE)) << endl;
		//}

		//cout << "-----------------------------" << endl;
		//cout << "Iterating vertices using halfedge conection(CCW):" << endl;
		//Iterators::VCcwEIterator vccweIter(pV);
		//for (auto pNeiE : vccweIter) {
		//	cout << "Neighbor edge length: " << Interface::edgeLength(pNeiE);
		//	cout << " Edge's vertices id: " << Interface::vertexId(Interface::edgeVertex1(pNeiE)) << " , " << Interface::vertexId(Interface::edgeVertex2(pNeiE)) << endl;
		//}

		//cout << "-----------------------------" << endl;
		//cout << "Iterating vertices using halfedge conection(CLW):" << endl;
		//Iterators::VClwEIterator vclweIter(pV);
		//for (auto pNeiE : vclweIter) {
		//	cout << "Neighbor edge length: " << Interface::edgeLength(pNeiE);
		//	cout << " Edge's vertices id: " << Interface::vertexId(Interface::edgeVertex1(pNeiE)) << " , " << Interface::vertexId(Interface::edgeVertex2(pNeiE)) << endl;
		//}

		///*
		//* Testing Vertex iterators.
		//*/
		//Iterators::VVIterator vvIter(pV);
		//cout << "Iterating vertices using halfedge list:" << endl;
		//cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		//for (auto pNeiV : vvIter) {
		//	cout << "Neighbor Vertex id: " << Interface::vertexId(pNeiV) << " Position: " << pNeiV->point() << endl;
		//}

		//cout << "-----------------------------" << endl;
		//cout << "Iterating vertices using halfedge conection(CCW):" << endl;
		//Iterators::VCcwVIterator vccwvIter(pV);
		//for (auto pNeiV : vccwvIter){
		//	cout << "Neighbor Vertex id: " << Interface::vertexId(pNeiV) << " Position: " << pNeiV->point() << endl;
		//}

		//cout << "-----------------------------" << endl;
		//cout << "Iterating vertices using halfedge conection(CLW):" << endl;
		//Iterators::VClwVIterator vclwvIter(pV);
		//for (auto pNeiV : vclwvIter) {
		//	cout << "Neighbor Vertex id: " << Interface::vertexId(pNeiV) << " Position: " << pNeiV->point() << endl;
		//}

		//Iterators::VOutHEIterator voutheIter(pV);
		//for (auto pHE : voutheIter) {
		//	cout << "The halfedge: " << pHE << endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << endl;
		//}
		//cout << "-----------------------------" << endl;
		//cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		//Iterators::VCcwOutHEIterator vccwheiter(pV);
		//for (Interface::HEPtr pHE : vccwheiter) {
		//	std::cout << "The halfedge: " << pHE << std::endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	Interface::VPtr pSourceV = Interface::halfedgeSource(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << " Source vertex id: " << Interface::vertexId(pSourceV) << endl;
		//}
		//cout << "-----------------------------" << endl;
		//cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		//Iterators::VClwInHEIterator vclwheiter(pV);
		//for (Interface::HEPtr pHE : vclwheiter) {
		//	std::cout << "The halfedge: " << pHE << std::endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	Interface::VPtr pSourceV = Interface::halfedgeSource(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << " Source vertex id: " << Interface::vertexId(pSourceV) << endl;
		//}
		getchar();

	    //}
}