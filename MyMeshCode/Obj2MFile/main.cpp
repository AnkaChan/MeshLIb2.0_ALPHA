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
	mesh.read_obj(inPath);
	clockEnd = clock();
	std::string outPath = fp.path + fp.name + ".m";
	mesh.write_m(outPath.c_str());
	std::cout << "Save to: " << outPath << std::endl;
}