#pragma once
#include "Arcball.h"
#include <GL\glut.h>
#include <GL\freeglut_ext.h>
#include <MeshLib\core\Mesh\MesCoreHeader.h>

namespace MeshLib {
	namespace GLViewer{
		/* window width and height */
		int win_width, win_height;
		int gButton;
		int startx, starty;
		int shadeFlag = 0;

		/* rotation quaternion and translation vector for the object */
		CQrot       ObjRot(0, 0, 1, 0);
		CPoint      ObjTrans(0, 0, 0);
		typedef CInterface<CVertexVisual, CEdgeVisual, CFaceVisual, CHalfEdge> IFGL;
		IFGL::MeshPtr pMesh;

		/* arcball object */
		CArcball arcball;
		struct GLSetting
		{

		} m_glsetting;

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

			draw_axis();

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
			case '?':
				help();
				break;
			case 27:
				exit(0);
				break;

			}
			glutPostRedisplay();
		}
		/*! setup GL states */
		void setupGLstate() {
			GLfloat lightOneColor[] = { 0.8, 0.8, 0.8, 1.0 };
			GLfloat globalAmb[] = { .1, .1, .1, 1 };
			GLfloat lightOnePosition[] = { .0, 0.0, 1.0, 1.0 };

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

		void init_openGL()
		{
			/* glut stuff */
			glutInit(&argcG, argvG);                /* Initialize GLUT */
			glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
			glutInitWindowSize(800, 600);
			glutCreateWindow("Mesh Viewer");	  /* Create window with given title */
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
			glutMainLoop();                       /* Start GLUT event-processing loop */
		}

	}
}