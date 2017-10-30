#pragma once
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>
#include "TMeshVisualTypes.h"

namespace MeshLib {
	namespace TMeshLib {
		/*
		* The struct containing the configuration of the viewer.
		*/
		struct GLTetSetting
		{
			//enum type to store coloring mode of geometric elements.
			//none        : means do not draw this element
			//defaultColor: means use default color to draw this element.
			//userDefined : means to user defined color, to use this, the coressponding geometric element's class must 
			//              be child class of CVertexVisual, CFaceVisual or CEdgeVisual
			enum ColorMode { none, defaultColor, userDefined };
			//Face's coloring mode
			ColorMode halfFaceColorMode = defaultColor;
			//vertex's coloring mode
			ColorMode vertexColorMode = none;
			//edge's coloring mode
			ColorMode edgeColorMode = none;
			//size of veretex
			double vertexSize = 4.0;
			//size of edge
			double edgeSize = 2.0;
			//to use defualt size or custom size
			bool useCustomVertexSize = false;
			bool useCustomEdgeSize = false;
			bool renderAllEdges = false;
			bool drawAxis = false;

			bool enableAlpha = false;
			//cut plane settings
			bool enableCutPlane = false;
			enum cutPlaneAxisMode { xAxis, yAxis, zAxis };
			cutPlaneAxisMode cutPlaneAxis = xAxis;
			double cutPlaneStep = 0.05;
			int cutPlaneShowMode = 1; //set to 1 or -1, determining to show the positive or negative side of the cut plane
			double cutPlanePosition = 0;
		};
		class CSimpleTetViewer {
		public:
			typedef TInterface<CTVertex, CVertexVisual, CHalfEdge, CTEdge, CEdgeVisual, CHalfFaceVisual, CFace, CTetVisual> TIFGL;
			typedef TIterators<TIFGL> TITGL;
			/*
			* Construct function, can take one to three paremeters, two with default parementer.
			* \param pM           : pointer to your mesh, it can be pointer to any mesh .
			* \param toComputeN   : whether to compute normal for vertices and faces again
			* \param toNormalizer : wether to normalize your mesh
			*/
			CSimpleTetViewer();
			void setMeshPointer(void * pM, bool toNormalize = false);

			void normalizeTMesh();
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
			GLTetSetting& setting();
		private:
			TIFGL::TMeshPtr m_pM;
		};
	}
}
