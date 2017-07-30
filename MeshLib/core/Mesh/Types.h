#pragma once
#include "../Geometry/Point.h"
#include "../Mesh/Vertex.h"
#include "../Mesh/Edge.h"
#include "../Mesh/Face.h"
namespace MeshLib {
	struct _colorRGB {
		union
		{
			struct
			{
				float  r;
				float  g;
				float  b;
			};
			float color[3] = {0, 0, 0};
		};
	};

	struct _normal
	{
		_normal() : m_normal(0.0, 0.0, 0.0) {};
		CPoint& normal() { return m_normal; };
	private:
		CPoint m_normal;
	};

	struct uv {
		double u = 0.0, v = 0.0;
	};

	class CVertexVisual : public CVertex, public _colorRGB {};
	class CEdgeVisual : public CEdge, public _colorRGB {};
	class CFaceVisual : public CFace, public _colorRGB {};

}