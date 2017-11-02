#include <iostream>
#include <MeshLib/core/Mesh/MeshCoreHeader.h>
//#define RGBIMAGE_DONT_USE_OPENGL
#include <MeshLib/core/bmp/RgbImage.h>
#include <MeshLib/toolbox/FileIO.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>

#include "TextureMapper.h"
using namespace MeshLib;

typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> If;
typedef CIterators<If> It;


int main(int argc, char** argv) {
	if (argc != 4) {
		std::cout << "Please give a dir to input meshes(.m or .obj), a dir to texure images(.bmp) and a output dir. " << std::endl;
		return 1;
	}
	std::string inMeshDir = make2StandardPath(argv[1]);
	std::string inTextureDir = make2StandardPath(argv[2]);
	std::string outMeshDir = make2StandardPath(argv[3]);

	std::vector<std::string> meshFiles, textureFiles;
	getAllFiles(inMeshDir, meshFiles);
	getAllFiles(inTextureDir, textureFiles);

	if (meshFiles.size() != textureFiles.size()) {
		std::cout << "Number of mesh files does not match number texture files. " << std::endl;
		return 1;
	}

	for (int i = 0; i < meshFiles.size(); ++i) {
		If::MeshPtr pMesh = new If::MeshType;

		std::string meshFile = meshFiles[i];
		std::string textureFile = textureFiles[i];

		meshFile = standardSlash(meshFile);
		FileParts meshFileParts = fileparts(meshFile);

		if (meshFileParts.ext == ".m") {
			pMesh->read_m(meshFile.c_str());
			std::cout << "Load mesh: " << meshFile << std::endl;
		}
		else if (meshFileParts.ext == ".obj") {
			pMesh->read_obj(meshFile.c_str());

		}
		else {
			std::cerr << "Unsupported file format: " << meshFileParts.ext << std::endl;
		}

		RgbImage rgbImage;
		rgbImage.LoadBmpFile(textureFile.c_str());
		std::cout << "Load texture file: " << textureFile << std::endl;

		CTextureMapper<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> textureMapper(pMesh, &rgbImage);
		textureMapper.mapTextureToRGB();

		/*CSimpleMeshViewer viewer(pMesh, true, true);
		viewer.setting().faceColorMode = GLSetting::userDefined;
		viewer.show();*/

		std::string outMeshFile = outMeshDir + meshFileParts.name + "RGB.m";

		pMesh->write_m(outMeshFile.c_str());
		std::cout << "Save to: " << outMeshFile << std::endl;
		delete pMesh;
	}
}