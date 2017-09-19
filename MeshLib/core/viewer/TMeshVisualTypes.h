#pragma once
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>

namespace MeshLib {
	namespace TMeshLib {
		struct _colorRGB {
			union
			{
				struct
				{
					double  r;
					double  g;
					double  b;
					double  alpha;
				};
				double color[4] = { 0, 0, 0, 0};
			};
		};

		struct _normal
		{
			_normal() : m_normal(0.0, 0.0, 0.0) {};
			CPoint& normal() { return m_normal; };
		private:
			CPoint m_normal;
		};

		struct _uv {
			CPoint2 uv;
		};

		struct _size
		{
			double size = 0.0;
		};

		struct _visibility {
			//user defined visibility
			//a visibility that overwrites all other visibility
			bool alwaysVisible = false;
			bool customVisibility = true;
			//use by viewer, should not be changed by user.
			bool visible = true;

			bool isVisible() { 
				if (alwaysVisible) {
					return true;
				}
				else {
					return visible && customVisibility;
				}
			};
		};

		class CVertexVisual : public CVertex, public _colorRGB, public _size, public _visibility {};
		class CEdgeVisual : public CEdge, public _colorRGB, public _size, public _visibility {};
		class CHalfFaceVisual : public CHalfFace, public _colorRGB , public _normal , public _visibility {};
		class CTetVisual : public CTet, public _visibility {};
	}
}