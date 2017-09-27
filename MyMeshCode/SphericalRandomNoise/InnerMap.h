#pragma once
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/3rdParty/kdtree/src/tree.h>
#define SEARCH_COUNT 100
namespace MeshLib {
	namespace TMeshLib {
		struct _tetBary {
			CBaryCoordinates4D * pBaryCoord = NULL;
			CTet* pTImage;
			~_tetBary() {
				if (pBaryCoord != NULL) {
					delete pBaryCoord;
				}
			}
		};
		class CTetWithBaryCoords : public CTet, public _tetBary {};

		struct  _vertexWithImageVPtrAndNewPos{
			CPoint newPos;
			CVertex * pVImage;
			CTet* pTImage;
			CPoint4 bary;

		};
		class CVertexWithImageVPtrAndNewPos : public CVertex, public _vertexWithImageVPtrAndNewPos {};

		class CInnerMap{
		private:
			typedef TInterface<CTVertex, CVertexWithImageVPtrAndNewPos, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTetWithBaryCoords> TIf;
			typedef TIterators<TIf> TIt;
			struct KDTreeNode {
				CVertex * pV = NULL;

				double data[3];

				KDTreeNode() {}
				KDTreeNode(CVertex * newpV) {
					pV = newpV;
					data[0] = pV->position()[0];
					data[1] = pV->position()[1];
					data[2] = pV->position()[2];
				}
				KDTreeNode(double x, double y, double z) {
					data[0] = x;
					data[1] = y;
					data[2] = z;
				}

				double operator [] (int i) const {
					return data[i];
				}

				bool operator == (const KDTreeNode& p) const {
					return data[0] == p[0] && data[1] == p[1] && data[2] == p[2];
				}

				friend std::ostream& operator << (std::ostream& s, const KDTreeNode& p) {
					return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
				}
			};
			struct KDTreeNodeTet {
				CTet * pT = NULL;

				double data[3];

				KDTreeNodeTet() {}
				KDTreeNodeTet(CTet * newpT) {
					pT = newpT;
					CPoint c;
					for (int i = 0; i < 4; ++i) {
						c += pT->tvertex(i)->vert()->position();
					}
					c /= 4;
					pT = newpT;
					data[0] = c[0];
					data[1] = c[1];
					data[2] = c[2];
				}
				KDTreeNodeTet(double x, double y, double z) {
					data[0] = x;
					data[1] = y;
					data[2] = z;
				}

				double operator [] (int i) const {
					return data[i];
				}

				bool operator == (const KDTreeNodeTet& p) const {
					return data[0] == p[0] && data[1] == p[1] && data[2] == p[2];
				}

				friend std::ostream& operator << (std::ostream& s, const KDTreeNodeTet& p) {
					return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
				}
			};
		public:
			CInnerMap() : 
				kdtree(KDTreeNode(-1,-1,-1), KDTreeNode(1, 1, 1)), 
				kdtreeTet(KDTreeNodeTet(-1, -1, -1), KDTreeNodeTet(1, 1, 1))
			{};
			CInnerMap(TIf::TMeshPtr pSurfaceTMesh, TIf::TMeshPtr pSphericalSurfaceTMesh, TIf::TMeshPtr pOriginalTMesh) :
				kdtree(KDTreeNode(-1, -1, -1), KDTreeNode(1, 1, 1)),
				kdtreeTet(KDTreeNodeTet(-1, -1, -1), KDTreeNodeTet(1, 1, 1))
			{
				setSurfaceTMesh(pSurfaceTMesh);
				setSphericalSurfaceTMesh(pSphericalSurfaceTMesh);
				setOriginalTMesh(pOriginalTMesh);
			}
			CVertexWithImageVPtrAndNewPos* mapVertexFromOriginalTMeshToSurfaceTMesh(CVertex* pVOnOTMesh);
			CTetWithBaryCoords* mapVertexFromOriginalTMeshToSurfaceTMeshTet(CVertex* pVOnOTMesh);
			void buildSTMeshKDTree();
			void setSurfaceTMesh(TIf::TMeshPtr pSurfaceTMesh) {
				pSTMesh = pSurfaceTMesh;
				buildSTMeshKDTree();
				buidlSTMeshKDTetTree();
				makeBaryCoords(pSurfaceTMesh);
			};
			void setSphericalSurfaceTMesh(TIf::TMeshPtr pSphericalSurfaceTMesh) {
				pSSTMesh = pSphericalSurfaceTMesh;
				makeBaryCoords(pSphericalSurfaceTMesh);
			};
			void setOriginalTMesh(TIf::TMeshPtr pOriginalTMesh) {
				pOTMesh = pOriginalTMesh;
			};

			void mapOriginalTMeshToSurfaceTMesh();
			void mapSufaceeTMeshToSphericalTMesh();
			void changePointsOnOriginalTMesh();

		private:
			typedef KD::Core<3, KDTreeNode> CORE;
			typedef KD::Core<3, KDTreeNodeTet> CORETet;

			TIf::TMeshPtr pSTMesh, pSSTMesh, pOTMesh;
			KD::Tree<CORE> kdtree;
			KD::Tree<CORETet> kdtreeTet;

			void makeBaryCoords(TIf::TMeshPtr pTMesh);
			void buidlSTMeshKDTetTree();
		};

		CVertexWithImageVPtrAndNewPos* MeshLib::TMeshLib::CInnerMap::mapVertexFromOriginalTMeshToSurfaceTMesh(CVertex* pVOnOTMesh)
		{
			const CPoint & p = pVOnOTMesh->position();
			return (CVertexWithImageVPtrAndNewPos*)(kdtree.nearest(KDTreeNode(p[0], p[1], p[2]))).pV;
		}
		inline CTetWithBaryCoords * CInnerMap::mapVertexFromOriginalTMeshToSurfaceTMeshTet(CVertex * pVOnOTMesh)
		{
			CPoint p = pVOnOTMesh->position();
			int failureCount = 0;
			while (1) {
				std::vector<KDTreeNodeTet> Tets = kdtreeTet.nearest(KDTreeNodeTet(p[0], p[1], p[2]), SEARCH_COUNT*(1 + failureCount));
				for (int i = SEARCH_COUNT * failureCount; i < Tets.size(); ++i) {
					TIf::TPtr pTOnSTMesh = (CTetWithBaryCoords*)Tets[i].pT;
					if (pTOnSTMesh->pBaryCoord->withinTet(p)) {
						return pTOnSTMesh;
					}
				}
				++failureCount;
				std::cout << "Failure count: " << failureCount << std::endl;
			}
		}
		void CInnerMap::buildSTMeshKDTree()
		{
			for (auto pV : TIt::TM_VIterator(pSTMesh)) {
				kdtree.insert(KDTreeNode(pV));
			}
		}
		inline void CInnerMap::mapOriginalTMeshToSurfaceTMesh()
		{
			for (auto pV : TIt::TM_VIterator(pOTMesh)) {
				if (pV->boundary()) {
					TIf::VPtr pVImage = mapVertexFromOriginalTMeshToSurfaceTMesh(pV);
					pV->pVImage = pVImage;
				}
				else {
					TIf::TPtr pTImage = mapVertexFromOriginalTMeshToSurfaceTMeshTet(pV);
					pV->pTImage = pTImage;
					pV->bary = pTImage->pBaryCoord->descartes2Bary(pV->position());
				}
			}
		}
		inline void CInnerMap::mapSufaceeTMeshToSphericalTMesh()
		{
			for (auto pV : TIt::TM_VIterator(pSTMesh)) {
				TIf::VPtr pVImage = pSSTMesh->idVertex(pV->id());
				pV->pVImage = pVImage;
			}
			for (auto pT : TIt::TM_TIterator(pSTMesh)) {
				TIf::TPtr pTImage = pSSTMesh->idTet(pT->id());
				pT->pTImage = pTImage;
			}
		}
		inline void CInnerMap::changePointsOnOriginalTMesh()
		{
			mapOriginalTMeshToSurfaceTMesh();
			mapSufaceeTMeshToSphericalTMesh();
			for (auto pV : TIt::TM_VIterator(pOTMesh)){
				if (pV->boundary()) {
					TIf::VPtr pVOnSTMesh = (TIf::VPtr)pV->pVImage;
					pV->newPos = pVOnSTMesh->pVImage->position();
				}
				else {
					TIf::TPtr pTOnSTMesh = (TIf::TPtr)pV->pTImage;
					TIf::TPtr pTOnSSTMesh = (TIf::TPtr)pTOnSTMesh->pTImage;

					pV->newPos = pTOnSSTMesh->pBaryCoord->bary2Descartes(pV->bary);
				}
			}
			for (auto pV : TIt::TM_VIterator(pOTMesh)) {
				pV->position() = pV->newPos;
			}
		}
		inline void CInnerMap::makeBaryCoords(TIf::TMeshPtr pTMesh)
		{
			for (auto pT : TIt::TM_TIterator(pTMesh)) {
				pT->pBaryCoord = new CBaryCoordinates4D(pT);
			}
		}
		inline void CInnerMap::buidlSTMeshKDTetTree()
		{
			for (TIf::TPtr pT : TIt::TM_TIterator(pSTMesh)) {
				kdtreeTet.insert(KDTreeNodeTet(pT));
			}
		}
	}
}