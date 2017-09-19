#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include <MeshLib/core/Mesh/boundary.h>

#include <MeshLib/toolbox/MeshLibGeneralDef.h>
#include <MeshLib/toolbox/FileIO.h>
int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Please give a input file." << endl;
		return 0;
	}
	char * inPath = argv[1];
	FileParts fp = fileparts(inPath);

	IfGeneral::MeshType mesh;
	mesh.read_m(inPath);
	double sqrt2 = sqrt(2.0);

	for (auto pV : ItGeneral::MVIterator(&mesh)) {
		pV->point() = pV->point() / sqrt2;
	}

	string outPath = fp.path + fp.name + "_precise.m";
	mesh.write_m_high_precision(outPath.c_str());

}
