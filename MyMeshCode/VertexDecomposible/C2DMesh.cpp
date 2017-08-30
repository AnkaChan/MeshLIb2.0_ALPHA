#pragma once
#include "C2DMesh.h"

#include <MeshLib/core/Mesh/MeshHeaders.h>
#include <MeshLib/core/Mesh/boundary.h>
#include <MeshLib/core/Geometry/Point.h>
#include "BaseMeshReadArray.h"
#include <assert.h>
#include <memory>
#include <list>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>

namespace MeshLib {
	class C2DMeshCore : public CBaseMeshReadArray<CVertexManifold, CEdge, CFace, CHalfEdge> 
	{};

	C2DMesh::C2DMesh() : m_p2DMesh(new C2DMeshCore)
	{}

	void C2DMesh::readArray(const VList & vlist, const FList & flist)
	{
		m_p2DMesh->readArray(vlist, flist);
	}

	bool C2DMesh::manifoldDeterminition()
	{
		return m_p2DMesh->manifoldDeterminition();
	}

	bool C2DMesh::isD2()
	{
		return m_p2DMesh->isD2();
	}
	C2DMeshCore * C2DMesh::getpMesh()
	{
		return m_p2DMesh.get();
	}
	void C2DMesh::show()
	{
		CSimpleMeshViewer viewer(getpMesh());
		viewer.show();
	}
}