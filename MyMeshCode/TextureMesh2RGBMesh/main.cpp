#include <iostream>
#include <MeshLib/core/Mesh/MeshCoreHeader.h>
//#define RGBIMAGE_DONT_USE_OPENGL
#include <MeshLib/core/bmp/RgbImage.h>
#include <MeshLib/toolbox/FileIO.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>

#include "TextureMapper.h"
using namespace MeshLib;

typedef CInterface<CVertexVisual, CEdge, CFace, CHalfEdge> If;
typedef CIterators<If> It;
If::MeshPtr pMesh = new If::MeshType; 


int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Please give a input mesh(.m) and a texure Image(.bmp). " << std::endl;
	}
	std::string inMeshFile = standardSlash(argv[1]);
	std::string inTextureFile(argv[2]);

	FileParts meshFileParts = fileparts(inMeshFile);

	if (meshFileParts.ext == ".m") {
		pMesh->read_m(inMeshFile.c_str());
	}
	else if (meshFileParts.ext == ".obj") {
		pMesh->read_obj(inMeshFile.c_str());

	}
	else {
		std::cerr << "Unsupported file format: " << meshFileParts.ext << std::endl;
	}

	RgbImage rgbImage;
	rgbImage.LoadBmpFile(inTextureFile.c_str());

	CTextureMapper<CVertexVisual, CEdge, CFace, CHalfEdge> textureMapper(pMesh, &rgbImage);
	textureMapper.mapTextureToRGB();

	CSimpleMeshViewer viewer(pMesh);
	viewer.show();
}