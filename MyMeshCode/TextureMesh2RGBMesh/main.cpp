#include <MeshLib/core/Mesh/MeshCoreHeader.h>
//#define RGBIMAGE_DONT_USE_OPENGL
#include <MeshLib/core/bmp/RgbImage.h>
#include <MeshLib/toolbox/FileIO.h>

using namespace MeshLib;

typedef CInterface<CVertex, CEdge, CFace, CHalfEdge> If;
typedef CIterators<If> It;
If::MeshPtr pMesh = new If::MeshType; 


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Please give a input file. " << std::cout;
	}
	std::string inFile(argv[1]);

	RgbImage rgbImage;
	rgbImage.LoadBmpFile();
}