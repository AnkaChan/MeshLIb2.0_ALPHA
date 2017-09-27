#include <MeshLib\toolbox\TMeshLibGeneralDef.h>
#include <MeshLib\core\Geometry\BaryCoordinates4D.h>
#include <MeshLib\toolbox\FileIO.h>
#include <ctime>
#include <random>

typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
typedef TIterators<TIf> TIt;

int main(int argc, char ** argv) {
	if (argc < 2) {
		cout << "Please give a path to TMesh(.t). " << endl;
		return 0;
	}

	string TMeshPath = argv[1];
	
	FileParts fp = fileparts(TMeshPath);

	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	pTMesh->_load_t(TMeshPath.c_str());
	std::default_random_engine generator(time(NULL));
	std::uniform_real_distribution<double> randReal(-1.0, 1.0);
	randReal(generator);

	for (auto pV : TIt::TM_VIterator(pTMesh)) {
		double shortestEdgeLength = 100000000000;
		for (auto pE : TIt::V_EIterator(pV)) {
			if (TIf::EdgeLength(pE) < shortestEdgeLength) {
				shortestEdgeLength = TIf::EdgeLength(pE);
			}
		}
		CPoint randD(randReal(generator), randReal(generator), randReal(generator));
		randD /= randD.norm();
		pV->position() += 0.1*shortestEdgeLength*randD;
		pV->position() /= pV->position().norm();
	}

	string outPath = fp.path + fp.name + "_noisy.t";
	pTMesh->_write_t(outPath.c_str(), true);
}