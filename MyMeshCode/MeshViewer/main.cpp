#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/iterators2.h>
#include <MeshLib/core/Mesh/Types.h>
#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#include <MeshLib/toolbox/FileIO.h>

/*
* This is a demo for CSimpleMeshViewer, which is designed to visualize mesh in a much easier way.
* It is simple to use, just need a pointer to your mesh, which is derived from MeshLib::CBaseMesh.
* To use the based visualization function, any mesh pointer will do.
* 
* Howerver, if you want to use some higher level function, like changing the color or size of some element,
* the template parameters of MeshLib::CBaseMesh must be child class of: 
* CVertexVisual, CEdgeVisual, CFaceViusal, CHalfEdge
* give classes of CVertex, CEdge and CFace will cause error.
* Thank you!
* Author: Anka
* Data  : 30/07/2017
*/


//The call back function which will delivered to simpleViewer
//It captures the 'n' key and iteratively mark the faces of the mesh to black.
using namespace MeshLib;
CSimpleMeshViewer * psimpleViewer;

void myKeyFunc(unsigned char key) {

	switch (key)
	{
	case 'e':
		if (psimpleViewer->setting().edgeColorMode == GLSetting::defaultColor) {
			psimpleViewer->setting().edgeColorMode = GLSetting::none;
		}
		else {
			psimpleViewer->setting().edgeColorMode = GLSetting::defaultColor;
		}
		break;
	case 'v':
		if (psimpleViewer->setting().vertexColorMode == GLSetting::defaultColor) {
			psimpleViewer->setting().vertexColorMode = GLSetting::none;
		}
		else {
			psimpleViewer->setting().vertexColorMode = GLSetting::defaultColor;
		}
		break;
	default:
		break;
	}
}

//Next part is the higher level usage of SimpleMeshViewer
//Firstly you define the Mesh whose parameters inheriting the CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge
typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> IF;
typedef CIterators<IF> IT;
//Classes derived from the visual classes.

using std::cout;
using std::endl;


int main(int argc, char ** argv) {
	//The simplest way to use viewer:
	if (argc < 2) {
		cout << "Please give input path." << endl;
		return 0;
	}
	char * inPath = argv[1];
	FileParts fp = fileparts(inPath);
	IF::MeshPtr pMesh = new IF::MeshType;
	if (fp.ext == ".obj") {
		pMesh->read_obj(inPath);
	}
	else if (fp.ext == ".m") {
		pMesh->read_m(inPath);
	}
	else {
		cout << "Unsupported file format: " << fp.ext << endl;
	}

	CSimpleMeshViewer simpleViewer(pMesh, true, true);
	psimpleViewer = &simpleViewer;
	//Set the faceColorMode to GLSetting::userDefined
	simpleViewer.setting().faceColorMode = GLSetting::defaultColor;
	//Set the vertexColorMode to GLSetting::defaultColor, to viusalize it
	simpleViewer.setting().vertexColorMode = GLSetting::none;
	//Set the edgeColorMode to GLSetting::defaultColor, to viusalize it
	simpleViewer.setting().edgeColorMode = GLSetting::defaultColor;
	simpleViewer.setUserKeyFunc(myKeyFunc);
	//You can set the edge Size
	simpleViewer.show();
	
}