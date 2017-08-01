#include <MeshLib/core/Mesh/MesCoreHeader.h>
#include <MeshLib/core/Mesh/boundary.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#define ARC_TO_COORD(arc) sin(arc),cos(arc), 0 
#define HSTR_EN(phe) (phe == NULL ? 0 : -((IF::halfedgeNext(phe)->vec())*(IF::halfedgePrev(phe)->vec())) \
/ ((IF::halfedgeNext(phe)->vec())^(IF::halfedgePrev(phe)->vec())).norm())
#define STRING_EN(pE) (pE->k*pow((IF::edgeVertex1(pE)->point()-IF::edgeVertex2(pE)->point()).norm(),2))
using namespace MeshLib;

double step = 0.00001, threshold = 0.00000001, o_en = 0.0f;
class CHEHm : public CHalfEdge { public: double oLength; CPoint vec() { return target()->point() - source()->point(); } };
class CEdgeHm : public CEdge { public: double k = 0.0f; };
typedef CInterface<CVertexVisual, CEdgeHm, CFaceVisual, CHEHm> IF;
typedef CIterators<IF> IT;
IF::MeshPtr pMesh = new IF::MeshType; 

void iterate() {
	auto totalEn = [&]() {double e = 0.0; for (auto pE : IT::MEIterator(pMesh)) { e += STRING_EN(pE); } return e; };
	if (abs(totalEn() - o_en) < threshold)
		return ;
	std::cout << "Harmonic Energy: " << (o_en = totalEn()) << std::endl;
	for (auto pV : IT::MVIterator(pMesh)) {
		CPoint nP; double tE = 0.0; if (IF::isBoundary(pV)) continue;
		for (auto pNV : IT::VVIterator(pV)) { auto pE = IF::vertexEdge(pV, pNV); nP += pNV->point()*pE->k; tE += pE->k; }
		pV->point() = nP / tE;
	}
	_sleep(200);
}

int main() {
	pMesh->read_m("C:/Code/Data/Mesh/outputs/face125.m");
	CSimpleMeshViewer simpleViewer(pMesh, true, true);// simpleViewer.show();
	CBoundary<IF::VType, IF::EType, IF::FType, IF::HEType> boundary(pMesh);
	auto loop = boundary.loops().front(); double l = loop->length(), lt = 0.0f;
	for (auto pE : IT::MEIterator(pMesh)) { pE->k = HSTR_EN(IF::edgeHalfedge(pE, 0)) + HSTR_EN(IF::edgeHalfedge(pE, 1)); };
	for (auto pHE : loop->halfedges()) { pHE->oLength = IF::edgeLength(IF::halfedgeEdge(pHE)); }
	for (auto pHE : loop->halfedges()) { auto pV = IF::halfedgeTarget(pHE);
		pV->point() = CPoint(ARC_TO_COORD(2 * PIE*(lt / l))); lt += pHE->oLength;} //simpleViewer.show();
	for (auto pV : IT::MVIterator(pMesh)) { if (! IF::isBoundary(pV)) pV->point() = CPoint(0, 0, 0); };
	simpleViewer.show();
	simpleViewer.setUserIdleFunc(iterate);
	simpleViewer.show();
	IF::MeshPtr pOMesh = new IF::MeshType; pOMesh->read_m("C:/Code/Data/Mesh/outputs/face125.m");
	for (auto pV : IT::MVIterator(pOMesh)) {
		pV->uv()[0] = (pMesh->idVertex(pV->id())->point())[0];
		pV->uv()[1] = (pMesh->idVertex(pV->id())->point())[1];
		pV->uv() = -(pV->uv() / 2 + CPoint2(0.5, 0.5));
	}
	simpleViewer.setMeshPointer(pOMesh, true, true);
	simpleViewer.setTexture("D:/Code/Data/texture/Xi.bmp");
	simpleViewer.setting().showUV = true;
	simpleViewer.show();
}