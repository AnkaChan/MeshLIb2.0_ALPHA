#pragma once
#include <iostream>
#include <memory>
#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/vertex.h>
#include <MeshLib/core/TetMesh/tvertex.h>
#include <MeshLib/core/TetMesh/edge.h>
#include <MeshLib/core/TetMesh/tedge.h>	
#include <MeshLib/core/TetMesh/face.h>
#include <MeshLib/core/TetMesh/halfface.h>
#include <MeshLib/core/TetMesh/halfedge.h>

#include <MeshLib/core/TetMesh/tet.h>

#include <MeshLib/core/TetMesh/titerators.h>
#include <MeshLib/core/viewer/Arcball.h>                          /*  Arc Ball  Interface         */
#include <MeshLib/algorithm/Shelling/TetSheller.h>
#include <MeshLib/core/Geometry/Arcs.h>
#include <MeshLib/core/TetMesh/titerators2.h>
#include "D3Parameterization.h"

#include <GL\glut.h>
#include <GL\freeglut_ext.h>

#define FACE_COLOR 0.8,0.8,0.8
#define FACE_COLOR_INSERT 1.0,0.5,0.5
#define FACE_COLOR_INSERT_4D 1.0,0.5,0.5,0.5
#define FACE_COLOR_INSERT_INNER 0.8,0.8,0.8,1.0
#define Edge_COLOR 0.2,0.2,0.2
#define ZOOM_LEVEL 1.0 
#define SOLID_MODE 1
#define WIRE_MODE 2
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace MeshLib::TMeshLib;
class CTetParaShelling : public CTetShelling, public _tetParameterization {};

typedef CTVertex CTVertexGL;
typedef CVertex CVertexGL;
typedef CHalfEdge CHalfEdgeGL;
typedef CTEdge CTEdgeGL;
typedef CEdge CEdgeGL;
typedef CHalfFace CHalfFaceGL;
typedef CFaceD3Parameterization CFaceGL;
typedef CTetParaShelling CTetGL;

typedef TInterface<CTVertexGL, CVertexGL, CHalfEdgeGL, CTEdgeGL, CEdgeGL, CHalfFaceGL, CFaceGL, CTetGL> TIFGL;
typedef TIterators<TIFGL> TITGL;
typedef CTMesh<CTVertexGL, CVertexGL, CHalfEdgeGL, CTEdgeGL, CEdgeGL, CHalfFaceGL, CFaceGL, CTetGL> CTMeshGL;
typedef HalfFaceVertexIterator<CTVertexGL, CVertexGL, CHalfEdgeGL, CTEdgeGL, CEdgeGL, CHalfFaceGL, CFaceGL, CTetGL> HfVIterator;
typedef TetHalfFaceIterator<CTVertexGL, CVertexGL, CHalfEdgeGL, CTEdgeGL, CEdgeGL, CHalfFaceGL, CFaceGL, CTetGL> THfIterator;
typedef D3Parameterization<TIFGL> D3Para;
/* window width and height */
int win_width, win_height;
int gButton;
int startx, starty;
int shadeFlag = 0;

/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
CPoint      ObjTrans(0, 0, 0);
CPoint		TetCenter;
extern CTMeshGL* pMesh;
extern D3Para * pd3Para;

extern std::shared_ptr<std::list<CTetGL *>> pShellingList;
std::list<CTetGL *>::iterator shellingIter;

std::list < CTetGL *> renderList;
bool drawCircumSphere = true;
bool drawEdgeArcs = true;
bool drawSurfaceEdges = true;
int circumSphereMod = WIRE_MODE;
double arcWidth = 2.0;
double innerEdgeWidth = 4.0;
extern int argcG;
extern char** argvG;
struct Sphere {
	CPoint center;
	double radius;
};
extern struct Sphere sphere;

/* arcball object */
CArcball arcball;

void caculateCenter() {
	TIFGL::TPtr pFirstT = (TIFGL::TPtr)(pShellingList->front());
	for (auto pV : TITGL::T_VIterator(pFirstT)) {
		TetCenter += pV->position();
	}
	TetCenter = TetCenter / 4.0f;
}

CPoint relocate(CPoint input) {
	return (input - sphere.center) * ZOOM_LEVEL;
}

/*! setup the object, transform from the world to the object coordinate system */
void setupObject(void)
{
	double rot[16];

	glTranslated(ObjTrans[0], ObjTrans[1], ObjTrans[2]);
	ObjRot.convert(rot);
	glMultMatrixd((GLdouble *)rot);
}

/*! the eye is always fixed at world z = +5 */
void setupEye(void) {
	glLoadIdentity();
	gluLookAt(0, 0, 3.5, 0, 0, 0, 0, 1, 0);
}

/*! setup light */
void setupLight()
{
	GLfloat lightOnePosition[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! draw axis */
void draw_axis()
{
	glLineWidth(2.0);
	//x axis
	glColor3f(1.0, 0.0, 0.0);	//red
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glEnd();

	//y axis
	glColor3f(0.0, 1.0, 0);		//green
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glEnd();

	//z axis
	glColor3f(0.0, 0.0, 1.0);	//blue
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();

	glLineWidth(1.0);
}

void draw_edge(CEdge* pE, double width, double * color) {
	TIFGL::VPtr pV1, pV2;
	pV1 = TIFGL::EdgeVertex1(pE);
	pV2 = TIFGL::EdgeVertex2(pE);
	CPoint p1 = pV1->position();
	CPoint p2 = pV2->position();

	glLineWidth(width);
	glColor3f(color[0], color[1], color[2]);
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glVertex3d(p1[0], p1[1], p1[2]);
	glVertex3d(p2[0], p2[1], p2[2]);
	glEnd();
	glEnable(GL_LIGHTING);

}
void draw_half_face(CHalfFaceGL* pHF, double * colorRGB, double alpha = 1.0f, bool invertNormal = false) {
	
	CPoint n(0.0, 0.0, 0.0);

	CHalfEdgeGL * pHE = pHF->half_edge();
	CPoint  p[3];
	for (int k = 0; k < 3; k++)
	{
		p[k] = pHE->target()->position();
		pHE = pHE->next();
	}
	CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
	n = fn / fn.norm();
	if (invertNormal) {
		n = -n;
	}
	glColor4f(colorRGB[0], colorRGB[1], colorRGB[2], alpha);

	glBegin(GL_TRIANGLES);
	glNormal3d(n[0], n[1], n[2]);
	for (int k = 0; k < 3; k++)
	{
		glVertex3d(p[k][0], p[k][1], p[k][2]);
	}
	glEnd();
}

/*! draw half faces */
void draw_half_faces()
{
	std::list<CHalfFace *> &HalfFaces = pMesh->half_faces();

	//glBindTexture(GL_TEXTURE_2D, texName);
	

	CTetGL * pNextT = *shellingIter;

	for (auto renderTetIter = renderList.begin(); renderTetIter != renderList.end(); ++renderTetIter)
	{
		CTetGL *pT = *renderTetIter;
		bool hasInsertFace = false;
		CHalfFace *pInsertHF = NULL;
		for (THfIterator THfIter(pMesh, pT); !THfIter.end(); ++THfIter)
		{
			CHalfFace *pHF = *THfIter;
			if (TIFGL::HalfFaceDual(pHF) != NULL) {
				if (TIFGL::HalfFaceTet(TIFGL::HalfFaceDual(pHF))->id() == pNextT->id()) {
					hasInsertFace = true;
					pInsertHF = pHF;
				}
			}
		}
		
		double insertFaceColor[3] = { FACE_COLOR_INSERT };
		double normalFaceColor[3] = { FACE_COLOR };
		if (hasInsertFace) {
			draw_half_face(pInsertHF, insertFaceColor, 1.0);
			for (auto pE : TITGL::T_EIterator(pT)) {
				double eColor[3] = { 0.0, 1.0, 0.0 };
				draw_edge(pE, 5.0, eColor);
			}
		}
		
		for (THfIterator THfIter(pMesh, pT); !THfIter.end(); ++THfIter)
		{
			CHalfFaceGL * pHF = *THfIter;
			if (pInsertHF == pHF)
				continue;
			if (TIFGL::HalfFaceDual(pHF) != NULL) {
				CTetGL * pTetNeighbor = (CTetGL *)pHF->dual()->tet();
				if (pTetNeighbor->visible) {
					continue;
				}
			}
			draw_half_face(pHF, normalFaceColor, 1.0);
		}
		
		/*if (isInsertFace) {
			for (auto pE : TITGL::T_EIterator(pT)) {
				double eColor[3] = {0.0, 0.0, 0.0};
				draw_edge(pE, 10.0, eColor);
			}
		}*/
	}
	
}

void draw_circumsphereFunc() {
	CPoint newCenter = sphere.center;
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(newCenter[0], newCenter[1], newCenter[2]);
	if (circumSphereMod == SOLID_MODE)
		glutSolidSphere(sphere.radius * ZOOM_LEVEL, 20, 20);
	else if (circumSphereMod == WIRE_MODE)
		glutWireSphere(sphere.radius * ZOOM_LEVEL, 20, 20);
	glEnd();
}

void draw_arcs(CPoint A, CPoint B, CPoint O, int nSeg) {
	Arcs arc(A.getEigenVec(), B.getEigenVec(), O.getEigenVec());
	
	glLineWidth(arcWidth);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.8, 0.4, 0.4);
	for (int i = 0; i <= nSeg; ++i) {
		double theta = arc.getArcRadian() * double(i) / double(nSeg);
		Eigen::Vector3d v = arc.getPointFromArc(theta);
		CPoint newV = relocate(v);
		glVertex3d(newV[0], newV[1], newV[2]);

	}
	glEnd();
}

void draw_edges_on_sphere() {
	//CTetGL* pT = (CTetGL*)pShellingList->front();
	const std::set<CHalfFaceGL *> & boundaryHFs = pd3Para->getBoundaryHFSet();
	for (auto pHF : boundaryHFs) {
		for (auto pHE : TITGL::HF_HEIterator(pHF)) {
			auto pE = TIFGL::HalfEdgeEdge(pHE);
			TIFGL::VPtr pV1, pV2;
			pV1 = TIFGL::EdgeVertex1(pE);
			pV2 = TIFGL::EdgeVertex2(pE);
			if (drawSurfaceEdges) {
				CPoint p1 = pV1->position();
				CPoint p2 = pV2->position();

				glBegin(GL_LINES);
				glLineWidth(arcWidth);
				glColor3f(Edge_COLOR);
				glVertex3d(p1[0], p1[1], p1[2]);
				glVertex3d(p2[0], p2[1], p2[2]);
				glEnd();
			}
			draw_arcs(pV1->position(), pV2->position(), sphere.center, 100);
		}
	}
}

/*! display call back function
*/
void display()
{
	/* clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupLight();
	/* transform from the eye coordinate system to the world system */
	setupEye();
	glPushMatrix();
	/* transform from the world to the ojbect coordinate system */
	setupObject();

	draw_half_faces();
	draw_axis();
	if (drawCircumSphere) {
		draw_circumsphereFunc();
	}
	if (drawEdgeArcs) {
		draw_edges_on_sphere();
	}
	glPopMatrix();
	glutSwapBuffers();
}

/*! Called when a "resize" event is received by the window. */
void reshape(int w, int h)
{
	float ar;
	//std::cout << "w:" << w << "\th:" << h << std::endl;
	win_width = w;
	win_height = h;

	ar = (float)(w) / h;
	glViewport(0, 0, w, h);               /* Set Viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// magic imageing commands
	gluPerspective(40.0, /* field of view in degrees */
		ar, /* aspect ratio */
		1.0, /* Z near */
		100.0 /* Z far */);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

/*! helper function to remind the user about commands, hot keys */
void help()
{
	printf("w  -  Wireframe Display\n");
	printf("f  -  Flat Shading \n");
	printf("s  -  Smooth Shading\n");
	printf("d  -  Show solid circumshpre of first tet. \n");
	printf("a  -  Show wire circumshpre of first tet. \n");
	printf("?  -  Help Information\n");
	printf("esc - quit\n");
}

/*! Keyboard call back function */
void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		//Flat Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = 0;
		break;
	case 's':
		//Smooth Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = 1;
		break;
	case 'w':
		//Wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 'd':
		drawCircumSphere = !drawCircumSphere;
		break;
	case 'e':
		drawEdgeArcs = !drawEdgeArcs;
		break;
	case 'q':
		circumSphereMod = SOLID_MODE;
		break;
	case 'a':
		circumSphereMod = WIRE_MODE;
		break;
	case '?':
		help();
		break;
	case 27:
		exit(0);
		break;
	
	}
	glutPostRedisplay();
}

void specailKey(int key, int x, int y)
{
	CTetGL * pTet;
	switch (key)
	{

	case GLUT_KEY_RIGHT:
		if (shellingIter != pShellingList->end())
		{
			pTet = *shellingIter;
			pTet->visible = true;
			renderList.push_back(pTet);
			++shellingIter;
			pd3Para->mapNextTetToSphereRand();
		}
		break;
	case GLUT_KEY_LEFT:
		pTet = *shellingIter;
		pTet->visible = false;
		if (shellingIter != pShellingList->begin())
			renderList.pop_back();
			--shellingIter;
		break;
	}
}

/*! setup GL states */
void setupGLstate() {
	GLfloat lightOneColor[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat globalAmb[] = { .1, .1, .1, 1 };
	GLfloat lightOnePosition[] = { .0, 0.0, 1.0, 1.0 };

	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.35, 0.53, 0.70, 0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightOneColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	caculateCenter();
}

/*! mouse click call back function */
void  mouseClick(int button, int state, int x, int y) {
	/* set up an arcball around the Eye's center
	switch y coordinates to right handed system  */

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		gButton = GLUT_LEFT_BUTTON;
		arcball = CArcball(win_width, win_height, x - win_width / 2, win_height - y - win_height / 2);
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_MIDDLE_BUTTON;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_RIGHT_BUTTON;
	}
	return;
}

/*! mouse motion call back function */
void mouseMove(int x, int y)
{
	CPoint trans;
	CQrot       rot;

	/* rotation, call arcball */
	if (gButton == GLUT_LEFT_BUTTON)
	{
		rot = arcball.update(x - win_width / 2, win_height - y - win_height / 2);
		ObjRot = rot * ObjRot;
		glutPostRedisplay();
	}

	/*xy translation */
	if (gButton == GLUT_MIDDLE_BUTTON)
	{
		double scale = 10. / win_height;
		trans = CPoint(scale*(x - startx), scale*(starty - y), 0);
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

	/* zoom in and out */
	if (gButton == GLUT_RIGHT_BUTTON) {
		double scale = 10. / win_height;
		trans = CPoint(0, 0, scale*(starty - y));
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

}

/*! Normalize mesh
* \param pMesh the input mesh
*/
//void normalize_mesh(CTMeshGL * pMesh)
//{
//	CPoint s(0, 0, 0);
//	for (CTMeshGL::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
//	{
//		CMyVertexGL * v = *viter;
//		s = s + v->point();
//	}
//	s = s / pMesh->numVertices();
//
//	for (MyMeshFL::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
//	{
//		CMyVertexGL * v = *viter;
//		CPoint p = v->point();
//		p = p - s;
//		v->point() = p;
//	}
//
//	double d = 0;
//	for (MyMeshFL::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
//	{
//		CMyVertexGL * v = *viter;
//		CPoint p = v->point();
//		for (int k = 0; k < 3; k++)
//		{
//			d = (d > fabs(p[k])) ? d : fabs(p[k]);
//		}
//	}
//
//	for (MyMeshFL::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
//	{
//		CMyVertexGL * v = *viter;
//		CPoint p = v->point();
//		p = p / d;
//		v->point() = p;
//	}
//};

/*! Compute the face normal and vertex normal
* \param pMesh the input mesh
*/
//void compute_normal(MyMeshFL * pMesh)
//{
//	for (MyMeshFL::MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
//	{
//		CMyVertexGL * v = *viter;
//		CPoint n(0, 0, 0);
//		for (MyMeshFL::VertexFaceIterator vfiter(v); !vfiter.end(); ++vfiter)
//		{
//			CMyFaceGL * pF = *vfiter;
//
//			CPoint p[3];
//			CHalfEdge * he = pF->halfedge();
//			for (int k = 0; k < 3; k++)
//			{
//				p[k] = he->target()->point();
//				he = he->he_next();
//			}
//
//			CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
//			pF->normal() = fn / fn.norm();
//			n += fn;
//		}
//
//		n = n / n.norm();
//		v->normal() = n;
//	}
//};

void init_openGL()
{
	/* glut stuff */
	glutInit(&argcG, argvG);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("TMesh Viewer");	  /* Create window with given title */
	glViewport(0, 0, 800, 600);

	glutDisplayFunc(display);             /* Set-up callback functions */
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(specailKey);

	setupGLstate();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	shellingIter = pShellingList->begin();
	renderList.push_back(*shellingIter);
	++shellingIter;

	glutMainLoop();                       /* Start GLUT event-processing loop */
}

void init_openGL_EventPrepair()
{
	/* glut stuff */
	glutInit(&argcG, argvG);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mesh Viewer");	  /* Create window with given title */
	glViewport(0, 0, 800, 600);

	glutDisplayFunc(display);             /* Set-up callback functions */
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);
	setupGLstate();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}