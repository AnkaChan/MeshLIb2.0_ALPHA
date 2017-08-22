#pragma once
#include <MeshLib/core/Mesh/MeshHeaders.h>
#include <MeshLib/core/Mesh/boundary.h>
#include <MeshLib/core/Geometry/Point.h>
#include "BaseMeshReadArray.h"
#include "C2DMesh.h"
#include <assert.h>
#include <memory>
#include <list>

namespace MeshLib {
	typedef CBaseMeshReadArray<CVertex, CEdge, CFace, CHalfEdge> C2DMeshCore;

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
}