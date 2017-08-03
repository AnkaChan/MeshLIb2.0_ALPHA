#include <algorithm>
#include <iostream>

#include <MeshLib\core\Mesh\BaseMesh.h>
#include <MeshLib\core\Mesh\Vertex.h>
#include <MeshLib\core\Mesh\Edge.h>
#include <MeshLib\core\Mesh\Face.h>
#include <MeshLib\core\Mesh\HalfEdge.h>
#include <MeshLib\core\Mesh\iterators.h>
#include <MeshLib\core\Mesh\Property.h>

/*
#include "Mesh\Vertex.h"
#include "Mesh\Edge.h"
#include "Mesh\Face.h"
#include "Mesh\HalfEdge.h"
#include "Mesh\BaseMesh.h"

#include "Mesh\iterators.h"
#include "Parser\parser.h"

#include <Mesh/Property.h>
*/
using namespace MeshLib;
class myVertex : public CVertex {
public:
	int num = 0;
	Props & props() { return m_props; };
private:
	Props m_props;
};
class myHalfedge : public CHalfEdge {
public:
	int num = 10;
	Props & props() { return m_props; };
private:
	Props m_props;
};
class myEdge : public CEdge {
public:
	int num = 222;
	Props & props() { return m_props; };
private:
	Props m_props;
};
class myFace : public CFace {
public:
	double i = 1223.2312;
	Props & props() { return m_props; };
private:
	Props m_props;
};

class CMesh : public CBaseMesh<myVertex, myEdge, myFace, myHalfedge>
{
public:
	

};

int main() {
	typedef MeshVertexIterator<myVertex, myEdge, myFace, myHalfedge>		MeshVertexIterator;
	typedef MeshEdgeIterator<myVertex, myEdge, myFace, myHalfedge>			MeshEdgeIterator;
	typedef MeshFaceIterator<myVertex, myEdge, myFace, myHalfedge>			MeshFaceIterator;
	typedef MeshHalfEdgeIterator<myVertex, myEdge, myFace, myHalfedge>		MeshHalfEdgeIterator;

	typedef VertexVertexIterator<myVertex, myEdge, myFace, myHalfedge>		VertexVertexIterator;
	typedef VertexEdgeIterator<myVertex, myEdge, myFace, myHalfedge>			VertexEdgeIterator;
	typedef VertexFaceIterator<myVertex, myEdge, myFace, myHalfedge>			VertexFaceIterator;
	typedef VertexInHalfedgeIterator<myVertex, myEdge, myFace, myHalfedge>	VertexInHalfedgeIterator;
	typedef VertexOutHalfedgeIterator<myVertex, myEdge, myFace, myHalfedge>	VertexOutHalfedgeIterator;

	typedef FaceVertexIterator<myVertex, myEdge, myFace, myHalfedge>			FaceVertexIterator;
	typedef FaceEdgeIterator<myVertex, myEdge, myFace, myHalfedge>			FaceEdgeIterator;
	typedef FaceHalfedgeIterator<myVertex, myEdge, myFace, myHalfedge>		FaceHalfedgeIterator;
	typedef PropsHandle<myVertex, myEdge, myFace, myHalfedge> PropsH;

	std::vector<int> v(10);
	for (int& i : v) {
		i = 0;
	}

	CMesh mesh;
	mesh.read_m("C:/Users/dell/Desktop/Data/genus2/genus2.m");
	PropsH * phandle = new PropsH(&mesh);

	std::cout << "AddProperty test" << std::endl;
	phandle->AddVProp<std::string>("VProperty", "testV");
	phandle->AddEProp<std::string>("EProperty", "testE");
	phandle->AddFProp<std::string>("FProperty", "testF");
	phandle->AddHProp<std::string>("HProperty", "testH");

	std::cout << "DeleteProperty test" << std::endl;
	//phandle->DeleteVProp<std::string>("VProperty");
	//phandle->DeleteEProp<std::string>("EProperty");
	//phandle->DeleteFProp<std::string>("FProperty");
	//phandle->DeleteHProp<std::string>("HProperty");

	for (MeshVertexIterator viter(&mesh); !viter.end(); ++viter)
	{
		myVertex * pv = *viter;
		if (phandle->FindVProp("VProperty"))
		{
			std::cout << "id = " << pv->id()
				<< " VProperty = " << pv->props().Get<std::string>("VProperty") << std::endl;
		}
		else
		{
			std::cout << "VProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (MeshEdgeIterator eiter(&mesh); !eiter.end(); ++eiter)
	{
		myEdge * pe = *eiter;
		if (phandle->FindEProp("EProperty"))
		{
			myVertex * pv = mesh.edgeVertex1(pe);
			std::cout << "id = " << pv->id()
				<< " EProperty = " << pe->props().Get<std::string>("EProperty") << std::endl;
		}
		else
		{
			std::cout << "EProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		myFace * pf = *fiter;
		if (phandle->FindFProp("FProperty"))
		{
			myHalfedge * ph = mesh.faceHalfedge(pf);
			myVertex * pv = mesh.halfedgeVertex(ph);
			std::cout << "id = " << pv->id()
				<< " FProperty = " << pf->props().Get<std::string>("FProperty") << std::endl;
		}
		else
		{
			std::cout << "FProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		myFace * pf = *fiter;
		myHalfedge * ph = mesh.faceHalfedge(pf);
		do
		{
			if (phandle->FindHProp("HProperty"))
			{
				myVertex * pv = mesh.halfedgeVertex(ph);
				std::cout << "id = " << pv->id()
					<< " HProperty = " << ph->props().Get<std::string>("HProperty") << std::endl;
			}
			else
			{
				std::cout << "HProperty is not found!" << std::endl;
			}
		} while (ph != mesh.faceHalfedge(pf));
	}
	getchar();

	std::cout << "Local Property Test!" << std::endl;
	for (MeshVertexIterator viter(&mesh); !viter.end(); ++viter)
	{
		myVertex * pv = *viter;
		if (pv->id() < 100)
		{
			phandle->AddLocProp<std::string>(pv, "VLP", "TestVLP");
			std::cout << "id = " << pv->id()
				<< " VProperty = " << pv->props().Get<std::string>("VLP") << std::endl;
		}
	}
	getchar();

	for (MeshEdgeIterator eiter(&mesh); !eiter.end(); ++eiter)
	{
		myEdge * pe = *eiter;
		myVertex * pv = mesh.edgeVertex1(pe);
		if (pv->id() < 100)
		{
			phandle->AddLocProp<std::string>(pe, "ELP", "TestELP");
			std::cout << "id = " << pv->id()
				<< " EProperty = " << pe->props().Get<std::string>("ELP") << std::endl;
		}
	}
	getchar();

	for (MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		myFace * pf = *fiter;
		myHalfedge *  ph = mesh.faceHalfedge(pf);
		myVertex * pv = mesh.halfedgeSource(ph);
		if (pv->id() < 100)
		{
			phandle->AddLocProp<std::string>(pf, "FLP", "TestFLP");
			std::cout << "id = " << pv->id()
				<< " FProperty = " << pf->props().Get<std::string>("FLP") << std::endl;
		}
	}
	getchar();

	for (MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		myFace * pf = *fiter;
		myHalfedge * ph = mesh.faceHalfedge(pf);
		do
		{
			myVertex * pv = mesh.halfedgeVertex(ph);
			if (pv->id() < 100)
			{
				phandle->AddLocProp<std::string>(ph, "HLP", "TestHLP");
				std::cout << "id = " << pv->id()
					<< " HProperty = " << ph->props().Get<std::string>("HLP") << std::endl;
			}
		} while (ph != mesh.faceHalfedge(pf));
	}
	getchar();

	std::cout << "Test Write and Delete Local Property" << std::endl;
	getchar();

	//phandle->DeleteVProp<std::string>("VLP");
	//phandle->DeleteEProp<std::string>("ELP");
	//phandle->DeleteFProp<std::string>("FLP");
	//phandle->DeleteHProp<std::string>("HLP");
	for (MeshVertexIterator viter(&mesh); !viter.end(); ++viter)
	{
		myVertex * pv = *viter;
		if (pv->props().Find("VLP"))
		{
			pv->props().Write<std::string>("VLP", "ReWriteVLP");
			std::cout << "id = " << pv->id()
				<< " VProperty = " << pv->props().Get<std::string>("VLP") << std::endl;
		}
		else
		{
			std::cout << "VProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (MeshEdgeIterator eiter(&mesh); !eiter.end(); ++eiter)
	{
		myEdge * pe = *eiter;
		if (pe->props().Find("ELP"))
		{
			pe->props().Write<std::string>("ELP", "ReWriteELP");
			myVertex * pv = mesh.edgeVertex1(pe);
			std::cout << "id = " << pv->id()
				<< " EProperty = " << pe->props().Get<std::string>("ELP") << std::endl;
		}
		else
		{
			std::cout << "EProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		myFace * pf = *fiter;
		if (pf->props().Find("FLP"))
		{
			pf->props().Write<std::string>("FLP", "ReWriteFLP");
			myHalfedge * ph = mesh.faceHalfedge(pf);
			myVertex * pv = mesh.halfedgeVertex(ph);
			std::cout << "id = " << pv->id()
				<< " FProperty = " << pf->props().Get<std::string>("FLP") << std::endl;
		}
		else
		{
			std::cout << "FProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (MeshFaceIterator fiter(&mesh); !fiter.end(); ++fiter)
	{
		myFace * pf = *fiter;
		myHalfedge * ph = mesh.faceHalfedge(pf);
		do
		{
			if (ph->props().Find("HLP"))
			{
				ph->props().Write<std::string>("HLP", "ReWriteHLP");
				myVertex * pv = mesh.halfedgeVertex(ph);
				std::cout << "id = " << pv->id()
					<< " HProperty = " << ph->props().Get<std::string>("HLP") << std::endl;
			}
			else
			{
				std::cout << "HProperty is not found!" << std::endl;
			}
			ph = mesh.halfedgeNext(ph);
		} while (ph != mesh.faceHalfedge(pf));
	}
	getchar();
}
