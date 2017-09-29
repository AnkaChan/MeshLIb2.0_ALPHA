#pragma once
#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
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

	struct _uv {
		CPoint2 uv;
	};

	struct _visibility {
		bool user_defined_visbility = false;
		bool visible() {
			return user_defined_visbility;;
		};
	};

	class CVertexVisual : public CVertex, public _colorRGB, public _visibility {};
	class CEdgeVisual : public CEdge, public _colorRGB, public _visibility {};
	class CFaceVisual : public CFace, public _colorRGB, public _visibility {};

}