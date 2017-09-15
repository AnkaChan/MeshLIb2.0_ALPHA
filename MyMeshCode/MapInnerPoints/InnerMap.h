#pragma once
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/3rdParty/kdtree/src/tree.h>
namespace MeshLib {
	namespace TMeshLib {
		
		class CInnerMap{
		private:
			typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
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
			CVertex* mapOriginalTMeshToSurfaceTMesh(CVertex* pVOnOTMesh);
			void buildSTMeshKDTree();
		private:
			typedef KD::Core<3, KDTreeNode> CORE;
			TIf::TMeshPtr pSTMesh, pSSTMesh, pOTMesh;
			KD::Tree<CORE> kdtree;
			void setSurfaceTMesh(TIf::TMeshPtr pSurfaceTMesh) {
				pSTMesh = pSurfaceTMesh;
			};
			void setSphericalSurfaceTMesh(TIf::TMeshPtr pSphericalSurfaceTMesh) {
				pSSTMesh = pSphericalSurfaceTMesh;
			};
			void setOriginalTMesh(TIf::TMeshPtr pOriginalTMesh) {
				pOTMesh = pOriginalTMesh;
			};

		};

		CVertex* MeshLib::TMeshLib::CInnerMap::mapOriginalTMeshToSurfaceTMesh(CVertex* pVOnOTMesh)
		{
			const CPoint & p = pVOnOTMesh->position();
			return (kdtree.nearest(KDTreeNode(p[0], p[1], p[2]))).pV;
		}
		void CInnerMap::buildSTMeshKDTree()
		{
			for (auto pV : TIt::TM_VIterator(pSTMesh)) {
				kdtree.insert(KDTreeNode(pV));
			}
		}
	}
}