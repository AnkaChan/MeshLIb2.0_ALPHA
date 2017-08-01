#include <iostream>
#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace TMeshLib;

class myTV : public CTVertex { public: int i = 100;; };

int main() {
	typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIF;
	TIF::TMeshPtr pTMesh = new TIF::TMeshType;
	typedef TIteratorCore<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet > TIT;
	pTMesh->_load_t("D:/Data/tet/lion_h.t");

	///* Testing the Face iterator*/
	//for (TIF::FPtr pF : TIT::TM_FIterator(pTMesh)) {
	//	cout << "Face ID: " << pF << endl;
	//}


	int i = 0;
	for (auto pE : TIT::TM_EIterator(pTMesh)) {
		++i;
		cout << "Edge: " << pE << "\n";
		cout << "Its vertices: V1: " << TIF::EdgeVertex1(pE)->id() << " , V2: " << TIF::EdgeVertex2(pE)->id() << '\n';
		cout << "TEdges number:" << TIF::EdgeTEdgeList(pE)->size() << endl;

		for (auto pF : TIT::E_FIterator(pE)) {
			cout << "It's adjacent faces: " << pF << endl;
		}

		for (auto pTE : TIT::E_TEIterator(pE)) {
			cout << "Its tEdges: " << pTE << endl;
		}
	}
	cout << "Edges number: " << pTMesh->edges().size() << '\n';
	cout << "Edges count: " << i << endl;

	///* Testing the Tet iterator*/
	//for (TIF::TPtr pT : TIT::TM_TIterator(pTMesh)) {
	//	int i = 0, j = 0;
	//	cout << "Tet's ID: " << pT->id() << endl;

	//	for (auto pE : TIT::T_EIterator(pT)) {
	//		++j;
	//		cout << "Edge: " << pE << '\n';
	//	}
	//	for (auto pHF : TIT::T_HFIterator(pT)) {
	//		++i;
	//		cout << "Halfface: " << pHF << "\n";
	//	}
	//	if (i != 4 || j != 6) {
	//		std::cerr << "Iterator Wrong!" << endl;
	//		getchar();
	//	}
	//}

	/* Testing the Veretex iterator*/
	//for (auto pV : TIT::TM_VIterator(pTMesh)) {
	//	cout << "Vertex ID: " << pV->id() << endl;
	//	cout << "Adjacent to " << TIF::VertexEdgeList(pV)->size() << " Edges.\n ";
	//	cout << "Iterating it's Neighbors" << endl;
	//	int i = 0;
	//	int j = 0;
	//	for (auto pE : TIT::V_EIterator(pV)) {
	//		++j;
	//		cout << "Edge: " << pE << '\n';
	//	}

	//	for (auto pNV : TIT::V_VIterator(pV)) {
	//		++i;
	//		cout << "Neighbor vetex id:" << pNV->id() << endl;
	//	}
	//	if (j != i)
	//		std::cerr << "Iterator Wrong!" << endl;
	//}
	getchar();
}