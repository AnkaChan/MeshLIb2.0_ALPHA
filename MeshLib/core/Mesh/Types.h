#pragma once
#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
#include "../Mesh/Vertex.h"
#include "../Mesh/Edge.h"
#include "../Mesh/Face.h"
#include "../Parser/parser.h"
#include <sstream>

#define FACE_DEFUALT_COLOR 0.7, 0.7, 0.8 
#define EDGE_DEFUALT_COLOR 0.5, 0.5, 0.1 
#define VERTEX_DEFUALT_COLOR 0.8, 0.0, 0.0 

namespace MeshLib {
	struct _colorRGB {
		_colorRGB() {};
		_colorRGB(float R, float G, float B) :
			r(R), g(G), b(B)
		{};

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
		union
		{
			struct
			{
				double  u;
				double  v;
			};
			double uv[2];
		};
	};

	struct _visibility {
		bool user_defined_visbility = false;
		bool visible() {
			return user_defined_visbility;;
		};
	};

	class CVertexVisual : public CVertex, public _colorRGB, public _visibility {
	public:
		CVertexVisual() : _colorRGB(VERTEX_DEFUALT_COLOR) {};
			
		void _from_string()
		{
			static std::stringstream _sstream;
			
			CParser parser(this->string());
			for (auto tokenIter = parser.tokens().begin(); tokenIter != parser.tokens().end(); ++tokenIter) {
				_sstream.str("");
				_sstream.clear();
				CToken *pToken = *tokenIter;
				if (pToken->m_key == "uv") {
					std::string values = pToken->m_value;
					values.erase(values.begin());
					values.erase(values.end() - 1);
					_sstream << values;
					_sstream >> m_uv[0] >> m_uv[1];
				}if (pToken->m_key == "rgb") {
					std::string values = pToken->m_value;
					values.erase(values.begin());
					values.erase(values.end() - 1);
					_sstream << values;
					_sstream >> r >> g >> b;
				}
			}
		}

		void _to_string() 
		{
			std::stringstream ss;
			ss << "rgb=(" << r << " " << g << " " << b << ")"  << "  uv =(" << m_uv[0] << " " << m_uv[1] << ")";
			m_string = ss.str();
		}
	};
	class CEdgeVisual : public CEdge, public _colorRGB, public _visibility {
	public:
		CEdgeVisual() :
			_colorRGB(EDGE_DEFUALT_COLOR)
		{};
	};
	class CFaceVisual : public CFace, public _colorRGB, public _visibility {
	public:
		CFaceVisual() :
			_colorRGB(FACE_DEFUALT_COLOR)
		{};
	};

}