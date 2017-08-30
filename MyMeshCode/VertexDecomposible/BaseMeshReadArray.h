#pragma once
#include <MeshLib/core/Mesh/MeshHeaders.h>
#include <MeshLib/core/Mesh/boundary.h>
#include <MeshLib/core/Geometry/Point.h>
#include "C2DMesh.h"
#include <assert.h>
#include <memory>
#include <list>
#include <set>

namespace MeshLib {

	struct CFan {
		//The most CCW out halfedge of a fan.
		CHalfEdge* mostClwOutHE = NULL;
		CHalfEdge* mostCcwIntHE = NULL;
		bool isClosed = false;
		int numberTriangles = 0;

	};

	class CVertexManifold : public CVertex {
	public:
		int getNumFans() { return fans.size(); }
		std::list<CFan>& getFans() { return fans; };
	private:
		std::list<CFan> fans;
		
	};

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
		bool makeFansForVertices();
		bool isManifold() { return m_isManifold; };
	private:
		CFan makeFan(std::set<HalfEdgeType *> &);
		typedef CIterators<If> It;
		bool m_isManifold;
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
		m_isManifold = makeFansForVertices();
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::manifoldDeterminition()
	{
		

		for (auto pV : It::MVIterator(this)) {
			std::set<HalfEdgeType *> remainingOutHE;
			for (HalfEdgeType *pOutHE : pV->arhe()) {
				remainingOutHE.insert(pOutHE);
			}
			while (!remainingOutHE.empty()) {
				pV->getFans().push_back(makeFan(remainingOutHE));
			}
			if (pV->getNumFans() > 1) {
				return false;
			}
		}
		return true;
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::isD2()
	{
		if (!isManifold()) {
			return false;
		}
		labelBoundary();
		pBoundary = std::shared_ptr<BoundaryType>(new BoundaryType(this));
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
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	inline bool CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::makeFansForVertices()
	{
		bool isManifold = true;
		for (auto pV : It::MVIterator(this)) {
			std::set<HalfEdgeType *> remainingOutHE;
			for (HalfEdgeType *pOutHE : pV->arhe()) {
				remainingOutHE.insert(pOutHE);
			}
			while (!remainingOutHE.empty()) {
				pV->getFans().push_back(makeFan(remainingOutHE));
			}
			if (pV->getNumFans() > 1) {
				isManifold = false;
			}
		}
		return isManifold;
	}
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	CFan CBaseMeshReadArray<VertexType, EdgeType, FaceType, HalfEdgeType>::makeFan(std::set<HalfEdgeType*>& remainingHE)
	{
		assert(!remainingHE.empty());
		HalfEdgeType * pFirstHE = *remainingHE.begin();
		remainingHE.erase(pFirstHE);
		int numTriangles = 1;
		HalfEdgeType * mostCcwOutHE = If::vertexNextCcwOutHalfEdge(pFirstHE);
		HalfEdgeType * mostClwOutHE = If::vertexNextClwOutHalfEdge(pFirstHE);
		HalfEdgeType * pStartHE = NULL;
		HalfEdgeType * pEndHE = NULL;
		if (mostCcwOutHE == NULL) {
			pStartHE = pFirstHE;
		}
		else {
			pStartHE = mostCcwOutHE;
			remainingHE.erase(mostCcwOutHE);
			numTriangles++;
		}
		if (mostClwOutHE == NULL) {
			pEndHE = pFirstHE;
		}else {
			pEndHE = mostClwOutHE;
			numTriangles++;
			remainingHE.erase(mostClwOutHE);
		}

		while (mostCcwOutHE != mostClwOutHE) {
			if (mostCcwOutHE != NULL) {
				mostCcwOutHE = If::vertexNextCcwOutHalfEdge(mostCcwOutHE);
				if (mostCcwOutHE != NULL){
					pStartHE = mostCcwOutHE;
					remainingHE.erase(mostCcwOutHE);
					numTriangles++;
				}
			}

			if (mostClwOutHE != NULL) {
				mostClwOutHE = If::vertexNextClwOutHalfEdge(mostClwOutHE);
				if (mostClwOutHE != NULL) {
					pEndHE = mostClwOutHE;
					remainingHE.erase(mostClwOutHE);
					numTriangles++;
				}
			}
		}
		CFan fan;
		fan.mostClwOutHE = pEndHE;
		fan.mostCcwIntHE = If::halfedgePrev(pStartHE);
		fan.numberTriangles = numTriangles;
		if (mostCcwOutHE != NULL) {
			fan.isClosed = true;
		}
		else {
			fan.isClosed = false;
		}
		return fan;
	}
}