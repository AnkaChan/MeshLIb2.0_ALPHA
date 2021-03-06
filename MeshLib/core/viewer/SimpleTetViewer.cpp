#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <MeshLib/core/viewer/Arcball.h>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include "TMeshVisualTypes.h"
#include "SimpleTetViewer.h"
#define DEFAULT_FACE_COLOR 0.8,0.8,0.8
#define DEFAULT_EDGE_COLOR 0.2,0.2,0.2
#define DEFAULT_VERTEX_COLOR 0,0,0
#define DEFAULT_EDGE_SIZE 1.0
#define DEFAULT_VERTEX_SIZE 5.0
#define MAX(a,b) ((a)>(b) ? (a) : (b))

using namespace MeshLib;
using namespace TMeshLib;


/* window width and height */
int win_width, win_height;
int gButton;
int startx, starty;
enum ShadeFlag { _vertex, _face };
ShadeFlag shadeFlag = _face;
GLfloat  faceDefaultColor[3] = { 0.7, 0.7, 0.8 };
GLfloat  edgeDefaultColor[3] = { 0.5, 0.5, 0.1 };
GLfloat  vertexDefaultColor[3] = { 0.8, 0.0, 0.0 };
struct GLTetSetting m_glSetting;

//This is the function pointer to the user defined key responding function.
//It will be called after a key was pressed.
//It can be changed by user, by calling CSampleMeshViewer::setUserKeyFunc
void(*custom_user_key_func)(unsigned char key);
void(*custom_idle_func)(void);

/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
CPoint      ObjTrans(0, 0, 0);

typedef TInterface<CTVertex, CVertexVisual, CHalfEdge, CTEdge, CEdgeVisual, CHalfFaceVisual, CFace, CTetVisual> TIFGL;
typedef TIterators<TIFGL> TITGL;
//This is a global variant, stores the pointer to mesh to be visualized.
TIFGL::TMeshPtr pTMesh;
/* arcball object */
CArcball arcball;

/*! setup the object, transform from the world to the object coordinate system */
void setupObject(void) {
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
void setupLight() {
	GLfloat lightOnePosition[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! draw axis */
void draw_axis() {
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
void draw_edge(TIFGL::EPtr pE, double width, double * color) {
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
void draw_half_face(TIFGL::HFPtr pHF, double * colorRGB, double alpha = 1.0f, bool invertNormal = false) {

	CPoint n(0.0, 0.0, 0.0);

	TIFGL::HEPtr pHE = pHF->half_edge();
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
	std::list<TIFGL::HFPtr> &HalfFaces = pTMesh->half_faces();

	double defaultFaceColor[3] = { DEFAULT_FACE_COLOR };
	double defaultEdgeColor[3] = { DEFAULT_EDGE_COLOR };
	//refresh all the visibility
	for (TIFGL::EPtr pE : TITGL::TM_EIterator(pTMesh)) {
		pE->visible = false;
	}
	//check visibility and draw halfFaces;
	for (TIFGL::HFPtr pHF : HalfFaces) {
		TIFGL::TPtr pTet     = TIFGL::HalfFaceTet(pHF);
		TIFGL::HFPtr pDualHF = TIFGL::HalfFaceDual(pHF);
		TIFGL::HEPtr pHE     = TIFGL::HalfFaceHalfEdge(pHF);

		if (!pTet->isVisible()) {
			pHF->visible = false;
		} else if (pDualHF != NULL ) {
			TIFGL::TPtr pTetNeighbor = TIFGL::HalfFaceTet(pDualHF);
			if (pTetNeighbor->isVisible()) {
				pHF->visible = false;
			}
			else {
				pHF->visible = true;
			}
		}
		else {
			pHF->visible = true;
		}
		//draw halffaces
		if (m_glSetting.halfFaceColorMode != GLTetSetting::none && pHF->isVisible())
		{
			switch (m_glSetting.halfFaceColorMode)
			{
			case GLTetSetting::defaultColor:
				draw_half_face(pHF, defaultFaceColor);
				break;
			case GLTetSetting::userDefined:
				if (m_glSetting.enableAlpha) {
					draw_half_face(pHF, pHF->color, pHF->alpha);
				}
				else {
					draw_half_face(pHF, pHF->color);
				}
				break;
			default:
				break;
			}
		}
		
		for (int i = 0; i < 3; ++i) {
			TIFGL::VPtr pV = TIFGL::HalfEdgeTarget(pHE);
			TIFGL::EPtr pE = TIFGL::HalfEdgeEdge(pHE);
			if (pHF->isVisible()) {
				pV->visible = true;
				pE->visible = true;
			}
			pHE = TIFGL::HalfEdgeNext(pHE);
			++i;
		}
	}

	//draw edges
	if (m_glSetting.edgeColorMode != GLTetSetting::none)
	{
		for (TIFGL::EPtr pE : TITGL::TM_EIterator(pTMesh)) {
			if (pE->isVisible() || m_glSetting.renderAllEdges) {
				switch (m_glSetting.edgeColorMode)
				{
				case GLTetSetting::defaultColor:
					if (m_glSetting.useCustomEdgeSize && pE->size > 0) {
						draw_edge(pE, pE->size, defaultEdgeColor);
					}
					else {
						draw_edge(pE, DEFAULT_EDGE_SIZE, defaultEdgeColor);
					}
					break;
				case GLTetSetting::userDefined:
					if (m_glSetting.useCustomEdgeSize && pE->size > 0) {
						draw_edge(pE, pE->size, pE->color);
					}
					else {
						draw_edge(pE, DEFAULT_EDGE_SIZE, pE->color);
					}
					break;
				default:
					break;
				}
			}
		}
	}
	//draw vertices
	//wait to be completed

}
void caculateTetVisibility() {
	for (TIFGL::TPtr pT : TITGL::TM_TIterator(pTMesh)) {
		pT->visible = true;

		for (TIFGL::VPtr pV: TITGL::T_VIterator(pT)) {
			double v;
			switch (m_glSetting.cutPlaneAxis)
			{
			case GLTetSetting::xAxis:
				v = pV->position()[0];
				break;
			case GLTetSetting::yAxis:
				v = pV->position()[1];
				break;
			case GLTetSetting::zAxis:
				v = pV->position()[2];
				break;
			default:
				break;
			}

			if (v * m_glSetting.cutPlaneShowMode < m_glSetting.cutPlaneShowMode *  m_glSetting.cutPlanePosition) {
				pT->visible = false;
				continue;
			}
		}
	}
}

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

	if (m_glSetting.enableCutPlane) {
		caculateTetVisibility();
	}

	draw_half_faces();
	if (m_glSetting.drawAxis) {
		draw_axis();
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

	printf("?  -  Help Information\n");
	printf("esc - quit\n");
}

void default_user_key_func(unsigned char key) {
	return;
}
/*! Keyboard call back function */
void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		//Flat Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = _face;
		break;
	case 's':
		//Smooth Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		shadeFlag = _vertex;
		break;
	case 'w':
		//Wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case '?':
		help();
		break;
	case 'c':
		m_glSetting.enableCutPlane = !m_glSetting.enableCutPlane;
		break;
	case 'x':
		m_glSetting.cutPlaneAxis = GLTetSetting::xAxis;
		break;
	case 'y':
		m_glSetting.cutPlaneAxis = GLTetSetting::yAxis;
		break;
	case 'z':
		m_glSetting.cutPlaneAxis = GLTetSetting::zAxis;
		break;
	case '=':
		m_glSetting.cutPlanePosition += m_glSetting.cutPlaneStep;
		break;
	case '-':
		m_glSetting.cutPlanePosition -= m_glSetting.cutPlaneStep;
		break;
	case 'r':
		m_glSetting.cutPlaneShowMode *= -1;
		break;
	default:
		custom_user_key_func(key);
		break;
	}
	glutPostRedisplay();
}

void specailKey(int key, int x, int y)
{
	switch (key)
	{
	}
}

/*! setup GL states */
void setupGLstate() {
	GLfloat lightOneColor[]    = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat globalAmb[]        = { .1, .1, .1, 1 };
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
		rot    = arcball.update(x - win_width / 2, win_height - y - win_height / 2);
		ObjRot = rot * ObjRot;
		glutPostRedisplay();
	}

	/*xy translation */
	if (gButton == GLUT_MIDDLE_BUTTON)
	{
		double scale = 10. / win_height;
		trans        = CPoint(scale*(x - startx), scale*(starty - y), 0);
		startx       = x;
		starty       = y;
		ObjTrans     = ObjTrans + trans;
		glutPostRedisplay();
	}

	/* zoom in and out */
	if (gButton      == GLUT_RIGHT_BUTTON) {
		double scale = 10. / win_height;
		trans        = CPoint(0, 0, scale*(starty - y));
		startx       = x;
		starty       = y;
		ObjTrans     = ObjTrans + trans;
		glutPostRedisplay();
	}

}
void _Idle_func() {
	custom_idle_func();
	display();
}
void default_idle_func() {};
void init_openGL()
{
	int argc = 1;
	char * s = "None";
	char ** argv = &s;
	/* glut stuff */
	glutInit(&argc, argv);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("TMesh Viewer");	  /* Create window with given title */
	glViewport(0, 0, 800, 600);

	glutDisplayFunc(display);             /* Set-up callback functions */
	glutIdleFunc(_Idle_func);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(specailKey);

	setupGLstate();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutMainLoop();                       /* Start GLUT event-processing loop */
}

CSimpleTetViewer::CSimpleTetViewer()
{
}
void CSimpleTetViewer::setMeshPointer(void * pM, bool toNormalize)
{
	m_pM = (TIFGL::TMeshPtr)pM;
	pTMesh = m_pM;
	
	custom_user_key_func = default_user_key_func;
	custom_idle_func = default_idle_func;
	if (toNormalize)
		normalizeTMesh();
}
void CSimpleTetViewer::normalizeTMesh()
{
	CPoint center(0, 0, 0);
	CPoint min(100000000, 100000000, 100000000);
	CPoint max(-100000000, -100000000, -100000000);
	for (auto pV : TITGL::TM_VIterator(m_pM)) {
		CPoint v = pV->position();
		center += v;
		for (int i = 0; i < 3; ++i) {
			min[i] = v[i] < min[i] ? v[i] : min[i];
			max[i] = v[i] > max[i] ? v[i] : max[i];
		}
	}
	center = center / m_pM->vertices().size();
	CPoint scale = (max - min) / 3;
	double l = MAX(scale[0], MAX(scale[1], scale[2]));
	for (auto pV : TITGL::TM_VIterator(m_pM)) {
		pV->position() = (pV->position() - center) / l;
	}
	//wait to be fulfilled
}
void CSimpleTetViewer::setUserKeyFunc(void(*newUserFunc)(unsigned char key))
{
	custom_user_key_func = newUserFunc;
}
void CSimpleTetViewer::setUserIdleFunc(void(*newIdleFunc)(void))
{
	custom_idle_func = newIdleFunc;
}

void CSimpleTetViewer::show()
{
	init_openGL();
}

GLTetSetting & MeshLib::TMeshLib::CSimpleTetViewer::setting()
{
	return m_glSetting;
}
#undef MAX