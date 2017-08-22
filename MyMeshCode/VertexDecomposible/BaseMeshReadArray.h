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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	class CBaseMeshReadArray : public CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> {
	public:
		CBaseMeshReadArray() :
			pBoundary(nullptr)
		{};
		typedef CInterface<VertexType, EdgeType, FaceType, HalfEdgeType> If;
		typedef CBoundary<VertexType, EdgeType, FaceType, HalfEdgeType> BoundaryType;

		void readArray(const VList & vlist, const FList & flist);
		bool manifoldDeterminition();
		bool isD2();
		std::shared_ptr<CBoundary<VertexType, EdgeType, FaceType, HalfEdgeType>> pBoundary;
		std::shared_ptr<EList> getBoudaryLoop(int i = 0);
		int numBoundaries();
	};
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline void CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::readArray(const VList & vList, const FList & fList)
	{
		for (VertexElement v : vList) {
			VertexPtr pV = createVertex(v.id);
			pV->point() = v.point;
		}

		for (FaceElement f : fList) {
			std::vector<VertexType*> v;
			for (int vid : f.vertices) {
				v.push_back(idVertex(vid));
			}
			createFace(v, f.id);
		}
		labelBoundary();
		pBoundary = std::shared_ptr<BoundaryType>(new BoundaryType(this));
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::manifoldDeterminition()
	{
		typedef CIterators<If> It;

		for (auto pV : It::MVIterator(this)) {
			int nOutBoundaryHF = 0;
			for (auto pHE : It::VOutHEIterator(pV)) {
				if (If::isBoundary(pHE)) {
					++nOutBoundaryHF;
				}
			}
			if (nOutBoundaryHF > 1) {
				return false;
			}
		}
		return true;
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::isD2()
	{
		if (!manifoldDeterminition()) {
			return false;
		}
		if (pBoundary->loops().size() == 1) {
			return true;
		}
		else {
			return false;
		}
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline std::shared_ptr<EList> CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::getBoudaryLoop(int i)
	{
		assert(i < numBoundaries());
		std::shared_ptr<EList> loop(new EList);
		BoundaryType::TLoop * loopVec = pBoundary->loops()[i];
		for (auto pHE : loopVec->halfedges()) {
			int idT = If::halfedgeTarget(pHE)->id();
			int idS = If::halfedgeSource(pHE)->id();
			loop->push_back(EdgeElement(idS, idT));
		}
		return loop;
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline int CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::numBoundaries()
	{
		return pBoundary->loops().size();
	}
}