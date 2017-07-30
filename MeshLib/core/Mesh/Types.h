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
		_normal() : normal(0.0, 0.0, 0.0) {};
		CPoint normal;
	};

	struct uv {
		double u = 0.0, v = 0.0;
	};

	class CVertexVisual : public CVertex, public _colorRGB, public _normal {};
	class CEdgeVisual : public CEdge, public _colorRGB, public _normal {};
	class CFaceVisual : public CFace, public _colorRGB, public _normal {};

}