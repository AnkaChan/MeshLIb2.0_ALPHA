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
			pMesh = (TIf::MeshPtr)newpMesh;
		};
	private:
		TIf::MeshPtr pMesh;
	};
	void GuassianMap::map()
	{
		for (TIf::VPtr pV : TIt::MVIterator(pMesh)) {
			CPoint vNormal(0,0,0);
			for (TIf::FPtr pF : TIt::VFIterator(pV)) {
				CPoint fNormal = TIf::faceOrientedArea(pF);
				vNormal += fNormal;
			}
			vNormal /= vNormal.norm();
			pV->normal() = vNormal;
		}

		for (TIf::VPtr pV : TIt::MVIterator(pMesh)) {
			pV->point() = pV->normal();
		}
	}

}