#include <iostream>
#include <ctime>
#include <MeshLib\core\Mesh\MeshHeaders.h>
#include <MeshLib/toolbox/FileIO.h>

using namespace MeshLib;
int main(int argc, char** argv) {
	clock_t  clockBegin, clockEnd;
	clockBegin = clock();
	CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> mesh;
	if (argc < 2) {
		std::cout << "Please give input path." << std::endl;
		return 0;
	}

	char * inPath = argv[1];
	FileParts fp = fileparts(inPath);
	if (fp.ext != ".m") {
		std::cout << "Unsupported file format: " << fp.ext << ", please give .m file." <<std::endl;
		return 0;
	}
	mesh.read_m(inPath);
	clockEnd = clock();
	std::string outPath = fp.path + fp.name + ".obj";
	mesh.write_obj(outPath.c_str());
	std::cout << "Save to: " << outPath << std::endl;
}