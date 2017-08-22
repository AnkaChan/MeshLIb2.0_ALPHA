#pragma once
#include <array>
#include <memory>
#include <list>
#include <MeshLib/core/Geometry/Point.h>

namespace MeshLib {
	struct VertexElement {
		VertexElement() {};
		VertexElement(int i, double v0, double v1, double v2) :
			id(i),
			point(v0, v1, v2)
		{};
		VertexElement(int i, const CPoint & p) :
			id(i),
			point(p)
		{};
		CPoint point;
		int id;
	};
	struct FaceElement {
		FaceElement() {};
		FaceElement(int i, int v0, int v1, int v2) :
			vertices{ { v0, v1, v2 } },
			id(i)
		{}
		std::array<int, 3> vertices;
		int id;
	};
	struct EdgeElement {
		EdgeElement(int idSource, int idTarget) :
			vertices{ { idSource, idTarget } }
		{};
		std::array<int, 2> vertices;
	};
	typedef std::list<VertexElement> VList;
	typedef std::list<FaceElement> FList;
	typedef std::list<EdgeElement> EList;

	class C2DMeshCore;

	class C2DMesh {
	public:
		C2DMesh();
		void readArray(const VList & vlist, const FList & flist);
		bool manifoldDeterminition();
		bool isD2();

	private:
		std::shared_ptr<C2DMeshCore> m_p2DMesh;
	};
}