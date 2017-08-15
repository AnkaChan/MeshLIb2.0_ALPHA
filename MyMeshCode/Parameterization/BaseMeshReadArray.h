#pragma once
#include <MeshLib/core/Mesh/BaseMesh.h>

namespace MeshLib {
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>

	class CBaseMeshReadArray : public CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> {

	};
}