#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
using namespace MeshLib;
using namespace TMeshLib;
int main() {
	typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIF;
	TIF::TMeshPtr pTMesh = new TIF::TMeshType;
	pTMesh->_load_t("D:/Data/TetMesh/lion_h.t");
	getchar();
}