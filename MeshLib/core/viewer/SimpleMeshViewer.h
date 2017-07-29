#pragma once
#include "Arcball.h"
#include <GL\glut.h>
#include <GL\freeglut_ext.h>

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
	}
}