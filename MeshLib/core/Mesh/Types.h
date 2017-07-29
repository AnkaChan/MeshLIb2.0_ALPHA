#pragma once
#include "../Geometry/Point.h"
#include "../Mesh/Vertex.h"
#include "../Mesh/Edge.h"
#include "../Mesh/Face.h"
namespace MeshLib {
	struct _colorRGBA {
		union
		{
			struct
			{
				unsigned char  r;
				unsigned char  g;
				unsigned char  b;
			};
			unsigned char color[3] = {0, 0, 0};
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

	class CVertexVisual : public CVertex, public _colorRGBA, public _normal {};
	class CEdgeVisual : public CEdge, public _colorRGBA, public _normal {};
	class CFaceVisual : public CFace, public _colorRGBA, public _normal {};

}