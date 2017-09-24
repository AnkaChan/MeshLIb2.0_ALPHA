#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib\core\Geometry\BaryCoordinates4D.h>
#include <MeshLib/3rdParty/kdtree/src/tree.h>
#include "VolumetricMapper.h"

#define SEARCH_COUNT 100

namespace MeshLib {
	namespace TMeshLib {
		struct _tetBary {
			CBaryCoordinates4D * pBaryCoord = NULL;
			~_tetBary() {
				if (pBaryCoord != NULL) {
					delete pBaryCoord;
				}
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

		typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTetWithBaryCoords> TIf;
		typedef TIterators<TIf> TIt;

		class CTetWithBaryCoords : public CTet, public _tetBary {};
		struct VolumetricMapperCore {
			VolumetricMapperCore() :
				kdtreeSource(KDTreeNodeTet(-1, -1, -1), KDTreeNodeTet(1, 1, 1)),
				kdtreeTarget(KDTreeNodeTet(-1, -1, -1), KDTreeNodeTet(1, 1, 1))
			{};

			TIf::TMeshPtr pSourceTM = NULL;
			TIf::TMeshPtr pTargetTM = NULL;

			typedef KD::Core<3, KDTreeNodeTet> CORETet;
			KD::Tree<CORETet> kdtreeSource;
			KD::Tree<CORETet> kdtreeTarget;


		};
	}
}

using namespace MeshLib;
using namespace MeshLib::TMeshLib;

MeshLib::CVolumetricMapper::CVolumetricMapper()
{
}

void MeshLib::CVolumetricMapper::readSourceTMesh(const char * dir)
{
	core.pSourceTM = new TIf::TMeshType;
	core.pSourceTM->_load_t(dir);
	
	for (TIf::TPtr pT : TIt::TM_TIterator(core.pSourceTM)) {
		core.kdtreeSource.insert(KDTreeNodeTet(pT));
	}

}

void MeshLib::CVolumetricMapper::readTargetTMesh(const char * dir)
{
	core.pTargetTM = new TIf::TMeshType;
	core.pTargetTM->_load_t(dir);

	for (TIf::TPtr pT : TIt::TM_TIterator(core.pTargetTM)) {
		core.kdtreeTarget.insert(KDTreeNodeTet(pT));
	}
}

CPoint MeshLib::CVolumetricMapper::map(CPoint p)
{
	if (core.pSourceTM == NULL) {
		std::cerr << "Source TMesh hasn't been read.\n";
		exit(0);
	}
	else if (core.pTargetTM == NULL) {
		std::cerr << "Target TMesh hasn't been read.\n";
		exit(0);
	}

	int failureCount = 0;
	CPoint4 bary;
	CPoint imageP;
	TIf::TPtr pTOnSource;
	while (1) {
		std::vector<KDTreeNodeTet> Tets = core.kdtreeSource.nearest(KDTreeNodeTet(p[0], p[1], p[2]), SEARCH_COUNT*(1 + failureCount));
		for (int i = SEARCH_COUNT * failureCount; i < Tets.size(); ++i) {
			TIf::TPtr pTOnSTMesh = (CTetWithBaryCoords*)Tets[i].pT;
			if (pTOnSTMesh->pBaryCoord->withinTet(p)) {
				pTOnSource = pTOnSTMesh;
				bary = pTOnSTMesh->pBaryCoord->descartes2Bary(p);
			}
		}
		++failureCount;
		std::cout << "Failure count: " << failureCount << std::endl;
	}

	TIf::TPtr pTOnTarget = core.pTargetTM->idTet(pTOnSource->id());
	imageP = pTOnTarget->pBaryCoord->bary2dDescartes(bary);
	return imageP;
}

CPoint MeshLib::CVolumetricMapper::reverseMap(CPoint p)
{
	return CPoint();
}
