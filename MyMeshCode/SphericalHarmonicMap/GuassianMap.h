#pragma once
#include <MeshLib/core/Mesh/MeshCoreHeader.h>
namespace MeshLib {
	class GuassianMap {
	private:
		typedef CInterface<CVertex,CEdge,CFace,CHalfEdge> TIf;
		typedef CIterators<TIf> TIt;
	public :
		GuassianMap() {};
		void map();
		void setInputMesh(void * newpMesh) {
			pMesh = (TIf::MeshPtr)pMesh;
		};
	private:
		TIf::MeshPtr pMesh;
		CPoint calculateFaceNormal();
	};
	void GuassianMap::map()
	{
		for (TIf::VPtr pV : TIt::MVIterator(pMesh)) {
			CPoint vNormal(0,0,0);
			for (TIf::FPtr pF : TIt::VFIterator(pV)) {
				TIf::HEPtr pHE = TIf::faceHalfedge(pF);
				TIf::HEPtr pHENext = TIf::halfedgeNext(pHE);
				CPoint v1 = TIf::halfedgeVec(pHE);
				CPoint v2 = TIf::halfedgeVec(pHENext);
				CPoint fNormal = v1^v2;
				vNormal += fNormal;
			}
			vNormal /= vNormal.norm();
			pV->point() = vNormal;
		}
	}
	inline CPoint GuassianMap::calculateFaceNormal()
	{
		return CPoint();
	}
}