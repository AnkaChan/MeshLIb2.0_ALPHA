#include <MeshLib/core/Mesh/MesCoreHeader.h>
#include <MeshLib/core/Mesh/boundary.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#define ARC_TO_COORD(arc) sin(arc),cos(arc), 0
using namespace MeshLib;
int main() {
	class CVertexHm : public CVertexVisual {public: CPoint op; };
	class CHalfEdgeHm : public CHalfEdge { public: double oLength; };
	typedef CInterface<CVertexHm, CEdgeVisual, CFaceVisual, CHalfEdgeHm> IF;
	typedef CIterators<IF> IT;
	IF::MeshPtr pMesh = new IF::MeshType; pMesh->read_m("C:/Code/Data/Mesh/outputs/face125.m");
	CSimpleMeshViewer simpleViewer(pMesh, true, true); simpleViewer.show();
	CBoundary<IF::VType, IF::EType, IF::FType, IF::HEType> boundary(pMesh);
	auto loop = boundary.loops().front(); double l = loop->length(), lt = 0.0f;
	for (auto pHE : loop->halfedges()) { pHE->oLength = IF::edgeLength(IF::halfedgeEdge(pHE)); }
	for (auto pHE : loop->halfedges()) { auto pV = IF::halfedgeTarget(pHE); pV->op = pV->point();
		pV->point() = CPoint(ARC_TO_COORD(2 * PIE*(lt / l))); lt += pHE->oLength;} simpleViewer.show();
	for (auto pV : IT::MVIterator(pMesh)) { if (! IF::isBoundary(pV)) pV->point() = CPoint(0, 0, 0); };
	simpleViewer.show();


}
