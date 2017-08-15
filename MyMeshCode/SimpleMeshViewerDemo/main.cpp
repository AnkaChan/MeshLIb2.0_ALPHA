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

//Path to the input file. You may need to change this.
#define INPUT_FILE "D:/Code/Data/Mesh/eight12.m"

//The call back function which will delivered to simpleViewer
//It captures the 'n' key and iteratively mark the faces of the mesh to black.
using namespace MeshLib;
void myKeyFunc(unsigned char key) {
	static GLViewer::ITGL::MFIterator mfIter(GLViewer::pMesh);
	GLViewer::IFGL::FPtr pF;
	switch (key)
	{
	case 'n':
		pF = *mfIter;
		pF->r = 0.0;
		pF->g = 0.0;
		pF->b = 0.0;
		++mfIter;
		break;
	default:
		break;
	}
}

//Classes derived from the visual classes.
class myVertex : public CVertexVisual {
public:
	int num = 0;
};
class myHalfedge : public CHalfEdge {
public:
	int num = 10;
};
class myEdge : public CEdgeVisual {
	int num = 222;
};
class myFace : public CFaceVisual {
	double i = 1223.2312;

};
using std::cout;
using std::endl;

int main() {
	//The simplest way to use viewer:
	CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> *pBasicMesh = new CBaseMesh<CVertex, CEdge, CFace, CHalfEdge>;
	pBasicMesh->read_m(INPUT_FILE);
	CSimpleMeshViewer simpleViewer0(pBasicMesh);
	//Any mesh can be visualized.
	simpleViewer0.show();

	//Next part is the higher level usage of SimpleMeshViewer
	//Firstly you define the Mesh whose parameters inheriting the CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge
	typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> IF;
	typedef CIterators<IF> IT;

	IF::MeshPtr pMeshVisual = new IF::MeshType;
	pMeshVisual->read_m(INPUT_FILE);
	CSimpleMeshViewer simpleViewer(pMeshVisual);

	//We change the face color to yellow, by writing the color of every face to yellow
	for (IF::FPtr pF : IT::MFIterator(pMeshVisual)) {
		pF->r = 0.6;
		pF->g = 0.6;
		pF->b = 0.0;
	}
	//Set the faceColorMode to GLSetting::userDefined
	simpleViewer.setting().faceColorMode = GLSetting::userDefined;
	//Set the vertexColorMode to GLSetting::defaultColor, to viusalize it
	simpleViewer.setting().vertexColorMode = GLSetting::defaultColor;
	//You can set the vertex size
	simpleViewer.setting().vertexSize = 5.0;
	//Set the edgeColorMode to GLSetting::defaultColor, to viusalize it
	simpleViewer.setting().edgeColorMode = GLSetting::defaultColor;
	//You can set the edge Size
	simpleViewer.setting().edgeSize = 5.0;
	simpleViewer.show();

	//At last , you will learn how to capture keys from opengl and do your own reaction.
	//try to press 'n' key and see what happened!
	simpleViewer.setUserKeyFunc(myKeyFunc);
	simpleViewer.show();
	
}