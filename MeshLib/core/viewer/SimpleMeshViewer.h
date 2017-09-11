#pragma once
/*!
*      \file SimpleMeshViewer.h
*      \Class of a simple mesh Viewer
*	   \author Anka
*      \date 30/07/2017
*
*/
#include "Arcball.h"
#include <GL\glut.h>
#include <GL\freeglut_ext.h>
#include <MeshLib/core/Mesh/MeshHeaders.h>
#include <MeshLib/core/bmp/RgbImage.h>
#define MAX(a,b) ((a)>(b) ? (a) : (b))

/*
* CSimpleMeshViewer is designed to visualize mesh in a much easier way.
* It is simple to use, just need a pointer to your mesh, which is derived from MeshLib::CBaseMesh.
* To use the based visualization function, any mesh pointer will do.
*
* Howerver, if you want to use some higher level function, like changing the color or size of some element,
* the template parameters of MeshLib::CBaseMesh must be child class of:
* CVertexVisual, CEdgeVisual, CFaceViusal, CHalfEdge
* give classes of CVertex, CEdge and CFace will cause error.
* Thank you!
*/

namespace MeshLib {

	/*
	* The struct containing the configuration of the viewer.
	*/
	struct GLSetting
	{
		//enum type to store coloring mode of geometric elements.
		//none        : means do not draw this element
		//defaultColor: means use default color to draw this element.
		//userDefined : means to user defined color, to use this, the coressponding geometric element's class must 
		//              be child class of CVertexVisual, CFaceVisual or CEdgeVisual
		enum ColorMode { none, defaultColor, userDefined };
		//Face's coloring mode
		ColorMode faceColorMode = defaultColor;
		//vertex's coloring mode
		ColorMode vertexColorMode = none;
		//edge's coloring mode
		ColorMode edgeColorMode = none;
		//size of veretex
		double vertexSize = 4.0;
		//size of edge
		double edgeSize = 2.0;
		//whether to show uv using texture map. Requiring giving texture file first.
		bool showUV = false;
	};

	/*
	* The openGL functions.
	* Those are C style codes. It is okay not to read them
	*/
	namespace GLViewer {
		/* window width and height */
		int win_width, win_height;
		int gButton;
		int startx, starty;
		enum ShadeFlag { _vertex, _face };
		ShadeFlag shadeFlag = _face;
		GLfloat  faceDefaultColor[3] = { 0.7, 0.7, 0.8 };
		GLfloat  edgeDefaultColor[3] = { 0.5, 0.5, 0.1 };
		GLfloat  vertexDefaultColor[3] = { 0.8, 0.0, 0.0 };
		struct GLSetting m_glSetting;

		//This is the function pointer to the user defined key responding function.
		//It will be called after a key was pressed.
		//It can be changed by user, by calling CSampleMeshViewer::setUserKeyFunc
		void(*custom_user_key_func)(unsigned char key);
		void(*custom_idle_func)(void);

		/* rotation quaternion and translation vector for the object */
		CQrot       ObjRot(0, 0, 1, 0);
		CPoint      ObjTrans(0, 0, 0);
		typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> IFGL;
		typedef CIterators<IFGL> ITGL;
		//This is a global variant, stores the pointer to mesh to be visualized.
		IFGL::MeshPtr pMesh;
		/* texture id and image */
		int textureFlag = 2;
		GLuint texName;
		RgbImage image;
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
		void draw_faces() {
			glBegin(GL_TRIANGLES);
			for (auto pF : ITGL::MFIterator(pMesh)) {
				for (auto pV : ITGL::FVIterator(pF)) {
					CPoint pt = pV->point();
					CPoint n;
					switch (shadeFlag)
					{
					case _face:
						n = pF->normal();
						break;
					case _vertex:
						n = pV->normal();
						break;
					}
					glNormal3d(n[0], n[1], n[2]);
					switch (m_glSetting.faceColorMode)
					{
					case GLSetting::ColorMode::defaultColor:
						glColor3fv(faceDefaultColor);
						break;
					case GLSetting::ColorMode::userDefined:
						glColor3f(pF->color[0], pF->color[1], pF->color[2]);
						break;
					default:
						break;
					}
					if (m_glSetting.showUV) 
						glTexCoord2d(pV->uv()[0], pV->uv()[1]);
					glVertex3d(pt[0], pt[1], pt[2]);
				}
			}
			glEnd();
		}

		void draw_edges() {
			glEnable(GL_LINE_SMOOTH);
			glLineWidth(m_glSetting.edgeSize);
			//glEnable(GL_BLEND);
			glDisable(GL_LIGHTING);
			for (auto pE : ITGL::MEIterator(pMesh)) {
				glBegin(GL_LINES);
				switch (m_glSetting.edgeColorMode) {
				case GLSetting::ColorMode::defaultColor:
					glColor3fv(edgeDefaultColor);
					break;
				case GLSetting::ColorMode::userDefined:
					glColor3f(pE->color[0], pE->color[1], pE->color[2]);
					break;
				default:
					break;
				}
				CPoint v1 = IFGL::edgeVertex1(pE)->point();
				CPoint v2 = IFGL::edgeVertex2(pE)->point();
				glVertex3f(v1[0], v1[1], v1[2]);
				glVertex3f(v2[0], v2[1], v2[2]);
			}
			glEnd();
			glEnable(GL_LIGHTING);
		}

		void draw_vertices() {
			for (auto pV : ITGL::MVIterator(pMesh)) {
				glPointSize(m_glSetting.vertexSize);
				glBegin(GL_POINTS);
				switch (m_glSetting.vertexColorMode) {
				case GLSetting::ColorMode::defaultColor:
					glColor3fv(vertexDefaultColor);
					break;
				case GLSetting::ColorMode::userDefined:
					glColor3f(pV->color[0], pV->color[1], pV->color[2]);
					break;
				default:
					break;
				}
				glVertex3f(pV->point()[0], pV->point()[1], pV->point()[2]);
			}
			glEnd();
		}


		/*! display call back function
		*/
		void display() {
			/* clear frame buffer */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			setupLight();
			/* transform from the eye coordinate system to the world system */
			setupEye();
			glPushMatrix();
			/* transform from the world to the ojbect coordinate system */
			setupObject();

			draw_axis();
			if (m_glSetting.faceColorMode != GLSetting::none)
				draw_faces();
			if (m_glSetting.edgeColorMode != GLSetting::none)
				draw_edges();
			if (m_glSetting.vertexColorMode != GLSetting::none)
				draw_vertices();

			glPopMatrix();
			glutSwapBuffers();
		}

		/*! Called when a "resize" event is received by the window. */
		void reshape(int w, int h) {
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
			case 't':
				textureFlag = (textureFlag + 1) % 3;
				switch (textureFlag)
				{
				case 0:
					glDisable(GL_TEXTURE_2D);
					break;
				case 1:
					glEnable(GL_TEXTURE_2D);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
					break;
				case 2:
					glEnable(GL_TEXTURE_2D);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					break;
				}
				break;
			default:
				custom_user_key_func(key);
				break;
			}
			glutPostRedisplay();
		}
		/*! setup GL states */
		void setupGLstate() {
			GLfloat lightOneColor[] = { 0.8, 0.8, 0.8, 1.0 };
			GLfloat globalAmb[] = { .1, .1, .1, 1 };
			GLfloat lightOnePosition[] = { .0, 0.0, 1.0, 1.0 };

			//glEnable(GL_CULL_FACE);
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
		}
		/*! initialize bitmap image texture */
		void initialize_bmp_texture()
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &texName);
			glBindTexture(GL_TEXTURE_2D, texName);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//	int ImageWidth  = image.GetNumRows();
			//	int ImageHeight = image.GetNumCols();
			int ImageWidth = image.GetNumCols();
			int ImageHeight = image.GetNumRows();
			GLubyte *pr = (GLubyte *)image.ImageData();

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_RGB,
				GL_UNSIGNED_BYTE, pr);

			if (textureFlag == 1)
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			else if (textureFlag == 2)
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);
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

		void _Idle_func() {
			custom_idle_func();
			display();
		}
		void default_idle_func() {};
		void init_openGL()
		{
			/* glut stuff */
			int argcG = 1;
			char* s = "none";
			char** argvG = &s;

			glutInit(&argcG, argvG);                /* Initialize GLUT */
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
			glutInitWindowSize(800, 600);
			glutCreateWindow("Mesh Viewer");	  /* Create window with given title */
			glViewport(0, 0, 800, 600);

			glutDisplayFunc(display);             /* Set-up callback functions */
			glutIdleFunc(_Idle_func);
			glutReshapeFunc(reshape);
			glutMouseFunc(mouseClick);
			glutMotionFunc(mouseMove);
			glutKeyboardFunc(keyBoard);

			setupGLstate();
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
			initialize_bmp_texture();
			glutMainLoop();                       /* Start GLUT event-processing loop */
		}
	}

	/*
	* The main class of our viwer.
	* It manages the data used in GLViwer. User can use this class to communicate with OpenGL.
	* Easy to use, any mesh can be visualized.
	*/

	class CSimpleMeshViewer {
	public:
		typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> IF;
		typedef CIterators<IF> IT;
		/*
		* Construct function, can take one to three paremeters, two with default parementer.
		* \param pM           : pointer to your mesh, it can be pointer to any mesh .
		* \param toComputeN   : whether to compute normal for vertices and faces again
		* \param toNormalizer : wether to normalize your mesh
		*/
		CSimpleMeshViewer(void * pM, bool toComputeN = true, bool toNormalize = false) : m_pM((GLViewer::IFGL::MeshPtr)pM) {
			GLViewer::pMesh = (GLViewer::IFGL::MeshPtr)pM;
			if (toComputeN)
				computeNormal();
			GLViewer::custom_user_key_func = GLViewer::default_user_key_func;
			GLViewer::custom_idle_func = GLViewer::default_idle_func;

			if (toNormalize)
				normalizeMesh();
		}
		void setMeshPointer(void * pM, bool toComputeN = true, bool toNormalize = false){
			m_pM = (GLViewer::IFGL::MeshPtr)pM;
			GLViewer::pMesh = (GLViewer::IFGL::MeshPtr)pM;
			if (toComputeN)
				computeNormal();
			GLViewer::custom_user_key_func = GLViewer::default_user_key_func;
			GLViewer::custom_idle_func = GLViewer::default_idle_func;
			if (toNormalize)
				normalizeMesh();
		}
		void computeNormal();
		void normalizeMesh();
		/*
		* You can set your key responding function own here.
		* \param newUserFunc: It is a pointer to a function, whose return type is void and its input parameter is
		*                     a unsigned char, which is the key captured by OpenGL. You can use this to do you own
		*                     key reaction. Note that 's', 'h', 'f', '?' has already been occupied.
		*/
		void setUserKeyFunc(void(*newUserFunc)(unsigned char key));
		void setUserIdleFunc(void(*newIdleFunc)(void));
		/*
		* Show mesh. This function will open a new window. After the window was closed, the function will return to
		* your code, where the show() function was called, without terminating whole function.
		*/
		void show();
		GLSetting& setting() { return GLViewer::m_glSetting; };
		void setTexture(char* texturePath);
	private:

		GLViewer::IFGL::MeshPtr m_pM;
	};
	void CSimpleMeshViewer::computeNormal() {
		for (auto pV : IT::MVIterator(m_pM))
		{
			CPoint n(0, 0, 0);
			for (auto pF : IT::VFIterator(pV))
			{
				CPoint fn = IF::faceNormal(pF);
				pF->normal() = fn;
				n += fn;
			}
			n = n / n.norm();
			pV->normal() = n;
		}
	}

	void CSimpleMeshViewer::normalizeMesh()
	{
		CPoint center(0, 0, 0);
		CPoint min(0, 0, 0);
		CPoint max(0, 0, 0);
		for (auto pV : IT::MVIterator(m_pM)) {
			CPoint v = pV->point();
			center += v;
			for (int i = 0; i < 3; ++i) {
				min[i] = v[i] < min[i] ? v[i] : min[i];
				max[i] = v[i] > max[i] ? v[i] : max[i];
			}
		}
		center = center / m_pM->vertices().size();
		CPoint scale = max - min;
		double l = MAX(scale[0], MAX(scale[1], scale[2]));
		for (auto pV : IT::MVIterator(m_pM)) {
			pV->point() = (pV->point() - center) / l;
		}
	}

	void CSimpleMeshViewer::setUserKeyFunc(void(*newUserFunc)(unsigned char key))
	{
		GLViewer::custom_user_key_func = newUserFunc;
	}
	void CSimpleMeshViewer::setUserIdleFunc(void(*newIdleFunc)(void))
	{
		GLViewer::custom_idle_func = newIdleFunc;
	}

	void CSimpleMeshViewer::show()
	{
		GLViewer::init_openGL();
	}
	void CSimpleMeshViewer::setTexture(char* texturePath)
	{
		GLViewer::image.LoadBmpFile(texturePath);
	}
}