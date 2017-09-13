 #include <iostream>
#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace TMeshLib;

int main(int argc, char** argv) {
	typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
	//typedef TIteratorCore<myTV, myV, myHE, myTE, myE, myHF, myF, myTet> TIT;
	typedef TIterators<TIf> TIt;

	if (argc < 2) {
		cout << "Please give input path." << endl;
		return 0;
	}

	char * inPath = argv[1];
	TIf::TMeshPtr pTMesh = new TIf::TMeshType;
	FileParts fp = fileparts(inPath);
	if (fp.ext == ".t") {
		pTMesh->_load_t(inPath);
	}
	else if (fp.ext == ".tet") {
		pTMesh->_load(inPath);
	}
	else {
		cout << "Unsupported file format: " << fp.ext << endl;
	}
	std::list<TIf::HFPtr> surfaceFList;
	auto pVless = [](TIf::VPtr pVa, TIf::VPtr pVb) {
		if (pVa->id() < pVb->id())
			return true; 
		else 
			return false; 
	};
	std::set<TIf::VPtr, decltype(pVless)> Vset(pVless);
	for (auto pHF : TIt::TM_HFIterator(pTMesh)) {
		if (TIf::HalfFaceDual(pHF) == NULL) {
			surfaceFList.push_back(pHF);
			for (auto pV : TIt::HF_VIterator(pHF)) {
				Vset.insert(pV);
			}
		}
	}
	std::string outPath = fp.path + fp.name + "_surface.m";
	std::ofstream outf;
	outf.open(outPath);
	for (TIf::VPtr pV : Vset) {
		outf << "Vertex " << pV->id() << " " << pV->position() << "\n";
	}
	int numF = 0;
	for (TIf::HFPtr pHF : surfaceFList) {
		++numF;
		outf << "Face " << numF;
		for (TIf::VPtr pV : TIt::HF_VIterator(pHF)) {
			outf << " " << pV->id();
		}
		outf << "\n";
	}
	outf.close();
	cout << "Save to: " << outPath << std::endl;

	std::string outPathObj = fp.path + fp.name + "_surface.obj";
	std::ofstream outfObj;
	outfObj.open(outPathObj);
	for (TIf::VPtr pV : Vset) {
		outfObj << "v "  << " " << pV->position() << "\n";
	}
	for (TIf::HFPtr pHF : surfaceFList) {
		++numF;
		outfObj << "f";
		for (TIf::VPtr pV : TIt::HF_VIterator(pHF)) {
			outfObj << " " << pV->id();
		}
		outfObj << "\n";
	}
	outfObj.close();
	cout << "Save to: " << outPathObj << std::endl;

	std::cout << "Done.\n";
}