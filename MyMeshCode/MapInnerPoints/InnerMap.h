#pragma once
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/3rdParty/kdtree/src/tree.h>
#define SEARCH_COUNT 10
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
					CPoint c;
					for (int i = 0; i < 4; ++i) {
						c += pT->tvertex(i)->vert()->position();
					}
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

				friend std::ostream& operator << (std::ostream& s, const KDTreeNode& p) {
					return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
				}
			};
		public:
			CInnerMap() : kdtree(KDTreeNode(-1,-1,-1), KDTreeNode(1, 1, 1))
			{};
			CInnerMap(TIf::TMeshPtr pSurfaceTMesh, TIf::TMeshPtr pSphericalSurfaceTMesh, TIf::TMeshPtr pOriginalTMesh)
				: kdtree(KDTreeNode(-1, -1, -1), KDTreeNode(1, 1, 1))
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
			TIf::TMeshPtr pSTMesh, pSSTMesh, pOTMesh;
			KD::Tree<CORE> kdtree;
			void makeBaryCoords(TIf::TMeshPtr pTMesh);
		};

		CVertexWithImageVPtrAndNewPos* MeshLib::TMeshLib::CInnerMap::mapVertexFromOriginalTMeshToSurfaceTMesh(CVertex* pVOnOTMesh)
		{
			const CPoint & p = pVOnOTMesh->position();
			return (CVertexWithImageVPtrAndNewPos*)(kdtree.nearest(KDTreeNode(p[0], p[1], p[2]))).pV;
		}
		inline CTetWithBaryCoords * CInnerMap::mapVertexFromOriginalTMeshToSurfaceTMeshTet(CVertex * pVOnOTMesh)
		{
			std::vector<KDTreeNode> Tets;
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
					for (auto pV : TIt::TM_TIterator(pSTMesh)) {

					}
					TIf::TPtr pTImage = mapVertexFromOriginalTMeshToSurfaceTMeshTet(pV);
					pV->pTImage;
				}
			}
		}
		inline void CInnerMap::mapSufaceeTMeshToSphericalTMesh()
		{
			for (auto pT : TIt::TM_TIterator(pSTMesh)) {
				TIf::TPtr pTImage = pSSTMesh->idTet(pT->id());
				pT->pTImage = pTImage;
			}
		}
		inline void CInnerMap::changePointsOnOriginalTMesh()
		{
			//for (pV)
		}
		inline void CInnerMap::makeBaryCoords(TIf::TMeshPtr pTMesh)
		{
			for (auto pT : TIt::TM_TIterator(pTMesh)) {
				pT->pBaryCoord = new CBaryCoordinates4D(pT);
			}
		}
	}
}