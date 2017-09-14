#pragma once
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>

namespace MeshLib {
	namespace TMeshLib {
		class CInnerMap{
		private:
			typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
			typedef TIterators<TIf> TIt;

		public:

		private:
			TIf::TMeshPtr pSurfaceTMesh, pSphericalSurfaceTMesh, pOriginalTMesh;
		};
	}
}