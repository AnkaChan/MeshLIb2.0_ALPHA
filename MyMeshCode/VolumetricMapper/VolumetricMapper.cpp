#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/core/Geometry/BaryCoordinates4D.h>
#include <MeshLib/3rdParty/kdtree/src/tree.h>
#include "VolumetricMapper.h"

#define SEARCH_COUNT 100

namespace MeshLib {
	struct _tetBary {
		TMeshLib::CBaryCoordinates4D * pBaryCoord = NULL;
		~_tetBary() {
			if (pBaryCoord != NULL) {
				delete pBaryCoord;
			}
		}
	};


	class CTetWithBaryCoords : public TMeshLib::CTet, public _tetBary {};

	struct KDTreeNodeTet {
		TMeshLib::CTet * pT = NULL;

		double data[3];

		KDTreeNodeTet() {}

		KDTreeNodeTet(TMeshLib::CTet * newpT) {
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

	using namespace TMeshLib;
	typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTetWithBaryCoords> TIf;
	typedef TIterators<TIf> TIt;

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

using namespace MeshLib;
using namespace MeshLib::TMeshLib;

MeshLib::CVolumetricMapper::CVolumetricMapper() :
	pCore(new VolumetricMapperCore)
{}

void MeshLib::CVolumetricMapper::readSourceTMesh(const char * dir)
{
	pCore->pSourceTM = new TIf::TMeshType;
	pCore->pSourceTM->_load_t(dir);
	
	for (TIf::TPtr pT : TIt::TM_TIterator(pCore->pSourceTM)) {
		pT->pBaryCoord = new CBaryCoordinates4D(pT);
		pCore->kdtreeSource.insert(KDTreeNodeTet(pT));
	}

}

void MeshLib::CVolumetricMapper::readTargetTMesh(const char * dir)
{

	pCore->pTargetTM = new TIf::TMeshType;
	pCore->pTargetTM->_load_t(dir);

	for (TIf::TPtr pT : TIt::TM_TIterator(pCore->pTargetTM)) {
		pT->pBaryCoord = new CBaryCoordinates4D(pT);
		pCore->kdtreeTarget.insert(KDTreeNodeTet(pT));
	}
}

CPoint MeshLib::CVolumetricMapper::map(CPoint p)
{

	if (pCore->pSourceTM == NULL) {
		std::cerr << "Source TMesh hasn't been read.\n";
		exit(0);
	}
	else if (pCore->pTargetTM == NULL) {
		std::cerr << "Target TMesh hasn't been read.\n";
		exit(0);
	}

	int failureCount = 0;
	CPoint4 bary;
	CPoint imageP;

	TIf::TPtr pTOnSource = NULL;
	while (1) {
		std::vector<KDTreeNodeTet> Tets = pCore->kdtreeSource.nearest(KDTreeNodeTet(p[0], p[1], p[2]), SEARCH_COUNT*(1 + failureCount));
		for (int i = SEARCH_COUNT * failureCount; i < Tets.size(); ++i) {
			TIf::TPtr pTOnSTMesh = (CTetWithBaryCoords*)Tets[i].pT;
			if (pTOnSTMesh->pBaryCoord->withinTet(p)) {
				pTOnSource = pTOnSTMesh;
				bary = pTOnSTMesh->pBaryCoord->descartes2Bary(p);
			}
		}
		if (pTOnSource != NULL) {
			break;
		}
		++failureCount;
		std::cout << "Failure count: " << failureCount << std::endl;
	}

	TIf::TPtr pTOnTarget = pCore->pTargetTM->idTet(pTOnSource->id());
	imageP = pTOnTarget->pBaryCoord->bary2Descartes(bary);
	return imageP;
}

CPoint MeshLib::CVolumetricMapper::reverseMap(CPoint p)
{
	return CPoint();
}
