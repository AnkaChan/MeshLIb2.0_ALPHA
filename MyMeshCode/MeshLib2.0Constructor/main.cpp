#include <algorithm>
#include <iostream>
#include <MeshLib/core/Mesh/BaseMesh.h>
#include <MeshLib/core/Mesh/Interface.h>
#include <MeshLib/core/Mesh/Vertex.h>
#include <MeshLib/core/Mesh/HalfEdge.h>
#include <MeshLib/core/Mesh/Edge.h>
#include <MeshLib/core/Mesh/Face.h>
#include <MeshLib/core/Mesh/iterators_new.h>
#include <MeshLib/core/Mesh/Property.h>

using namespace MeshLib;
class myVertex : public CVertex {
public:
	int num = 0;
};
class myHalfedge : public CHalfEdge {
public:
	int num = 10;
};
using std::cout;
using std::endl;
int main() {
	typedef CInterface<myVertex, CEdge, CFace, myHalfedge> Interface;
	typedef CIteratorsI<Interface> Iterators;
	typedef Interface::MeshType CMesh;
	typedef PropsHandle<myVertex, CEdge, CFace, myHalfedge> PropsH;

	std::vector<int> v(10);
	for (int& i : v) {
		i = 0;
	}

	CMesh mesh;
	mesh.read_m("C:/Users/dell/Desktop/Data/genus2/genus2.m");
	Iterators::MEIterator meIter(&mesh);
	//cout << "Iterating Edges." << endl;
	//for (auto pE : meIter) {
	//	Interface::VPtr pV1, pV2;
	//	pV1 = Interface::edgeVertex1(pE);
	//	pV2 = Interface::edgeVertex2(pE);

	//	cout << "The edge's length:" << Interface::edgeLength(pE)
	//		<< "\nThe two vertice are: " << Interface::vertexId(pV1) << ", " << Interface::vertexId(pV2) << endl;

	//	cout << "Vertex 1: " << pV1->point() << endl;
	//	cout << "Vertex 2: " << pV2->point() << endl;

	//}
	/*
	cout << "edgehalfedge test" << endl;
	for (auto pe : meIter)
	{
		Interface::HEPtr ph = Interface::edgeHalfedge(pe, 0);
		Interface::VPtr pv0 = Interface::halfedgeSource(ph),
			pv1 = Interface::halfedgeTarget(ph);
		std::cout << "the first halfedge: id0 = " << pv0->id() << " id1 = " << pv1->id() << std::endl;
		ph = Interface::edgeHalfedge(pe, 1);
		pv0 = Interface::halfedgeSource(ph);
		pv1 = Interface::halfedgeTarget(ph);
		std::cout << "the second halfedge: id0 = " << pv0->id() << " id1 = " << pv1->id() << std::endl;
	}
	*/
	Iterators::MVIterator mvIter(&mesh);
	Iterators::MFIterator mfIter(&mesh);
	PropsH * phandle = new PropsH(&mesh);
	std::cout << "AddVProperty test" << std::endl;
	phandle->AddVProp<std::string>("VProperty", "testV");
	phandle->AddEProp<std::string>("EProperty", "testE");
	phandle->AddFProp<std::string>("FProperty", "testF");
	phandle->AddHProp<std::string>("HProperty", "testH");

	phandle->DeleteVProp<std::string>("VProperty");
	phandle->DeleteEProp<std::string>("EProperty");
	phandle->DeleteFProp<std::string>("FProperty");
	phandle->DeleteHProp<std::string>("HProperty");
	for (Interface::VPtr pv : mvIter)
	{
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
	for (Interface::EPtr pe : meIter)
	{
		if (phandle->FindEProp("EProperty"))
		{
			Interface::VPtr pv = mesh.edgeVertex1(pe);
			std::cout << "id = " << pv->id()
				<< " EProperty = " << pe->props().Get<std::string>("EProperty") << std::endl;
		}
		else
		{
			std::cout << "EProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (Interface::FPtr pf : mfIter)
	{
		if (phandle->FindFProp("FProperty"))
		{
			Interface::HEPtr ph = mesh.faceHalfedge(pf);
			Interface::VPtr pv = mesh.halfedgeVertex(ph);
			std::cout << "id = " << pv->id()
				<< " FProperty = " << pf->props().Get<std::string>("FProperty") << std::endl;
		}
		else
		{
			std::cout << "FProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (Interface::FPtr pf : mfIter)
	{
		Interface::HEPtr ph = mesh.faceHalfedge(pf);
		do
		{
			if (phandle->FindHProp("HProperty"))
			{
				Interface::VPtr pv = mesh.halfedgeVertex(ph);
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
	for (Interface::VPtr pv : mvIter)
	{
		if (pv->id() < 100)
		{
			phandle->AddLocProp<std::string>(pv, "VLP", "TestVLP");
			std::cout << "id = " << pv->id()
				<< " VProperty = " << pv->props().Get<std::string>("VLP") << std::endl;
		}
	}
	getchar();

	for (Interface::EPtr pe : meIter)
	{
		Interface::VPtr pv = mesh.edgeVertex1(pe);
		if (pv->id() < 100)
		{
			phandle->AddLocProp<std::string>(pe, "ELP", "TestELP");
			std::cout << "id = " << pv->id()
				<< " EProperty = " << pe->props().Get<std::string>("ELP") << std::endl;
		}
	}
	getchar();

	for (Interface::FPtr pf : mfIter)
	{
		Interface::HEPtr ph = mesh.faceHalfedge(pf);
		Interface::VPtr pv = mesh.halfedgeSource(ph);
		if (pv->id() < 100)
		{
			phandle->AddLocProp<std::string>(pf, "FLP", "TestFLP");
			std::cout << "id = " << pv->id()
				<< " FProperty = " << pf->props().Get<std::string>("FLP") << std::endl;
		}
	}
	getchar();

	for (Interface::FPtr pf : mfIter)
	{
		Interface::HEPtr ph = mesh.faceHalfedge(pf);
		do
		{
			Interface::VPtr pv = mesh.halfedgeVertex(ph);
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
	
	phandle->DeleteVProp<std::string>("VLP");
	phandle->DeleteEProp<std::string>("ELP");
	phandle->DeleteFProp<std::string>("FLP");
	phandle->DeleteHProp<std::string>("HLP");
	for (Interface::VPtr pv : mvIter)
	{
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
	for (Interface::EPtr pe : meIter)
	{
		if (pe->props().Find("ELP"))
		{
			pe->props().Write<std::string>("ELP", "ReWriteELP");
			Interface::VPtr pv = mesh.edgeVertex1(pe);
			std::cout << "id = " << pv->id()
				<< " EProperty = " << pe->props().Get<std::string>("ELP") << std::endl;
		}
		else
		{
			std::cout << "EProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (Interface::FPtr pf : mfIter)
	{
		if (pf->props().Find("FLP"))
		{
			pf->props().Write<std::string>("FLP", "ReWriteFLP");
			Interface::HEPtr ph = mesh.faceHalfedge(pf);
			Interface::VPtr pv = mesh.halfedgeVertex(ph);
			std::cout << "id = " << pv->id()
				<< " FProperty = " << pf->props().Get<std::string>("FLP") << std::endl;
		}
		else
		{
			std::cout << "FProperty is not found!" << std::endl;
		}
	}
	getchar();
	for (Interface::FPtr pf : mfIter)
	{
		Interface::HEPtr ph = mesh.faceHalfedge(pf);
		do
		{
			if (ph->props().Find("HLP"))
			{
				ph->props().Write<std::string>("HLP", "ReWriteHLP");
				Interface::VPtr pv = mesh.halfedgeVertex(ph);
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

	cout << "Iterating Edges." << endl;
	for (auto pV : mvIter) {
		cout << "Vertex id: " << Interface::vertexId(pV) << " Position: " << pV->point() << endl;

		if (Interface::isBoundary(pV))
			cout << "Vertex on boundary." << endl;
		else
			cout << "Vertex in interior." << endl;

		Iterators::VVIterator vvIter(pV);
		cout << "Iterating vertices using halfedge list:" << endl;
		cout << "The number of halfedges: " << pV->arhe().size() << endl;;
		for (auto pNeiV : vvIter) {
			cout << "Neighbor Vertex id: " << Interface::vertexId(pV) << " Position: " << pV->point() << endl;
		}

		cout << "-----------------------------" << endl;
		cout << "Iterating vertices using halfedge conection(CCW):" << endl;
		Iterators::VCcwVIterator vccwvIter(pV);
		for (auto pNeiV : vvIter){
			cout << "Neighbor Vertex id: " << Interface::vertexId(pV) << " Position: " << pV->point() << endl;
		}
		
		//Iterators::VOutHEIterator voutheIter(pV);
		//for (auto pHE : voutheIter) {
		//	cout << "The halfedge: " << pHE << endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << endl;
		//}
		//cout << "-----------------------------" << endl;
		//cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		//Iterators::VCcwOutHEIterator vccwheiter(pV);
		//for (Interface::HEPtr pHE : vccwheiter) {
		//	std::cout << "The halfedge: " << pHE << std::endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	Interface::VPtr pSourceV = Interface::halfedgeSource(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << " Source vertex id: " << Interface::vertexId(pSourceV) << endl;
		//}
		//cout << "-----------------------------" << endl;
		//cout << "Iterating halfedges using halfedge conection(CCW):" << endl;
		//Iterators::VClwInHEIterator vclwheiter(pV);
		//for (Interface::HEPtr pHE : vclwheiter) {
		//	std::cout << "The halfedge: " << pHE << std::endl;
		//	Interface::VPtr pTargetV = Interface::halfedgeTarget(pHE);
		//	Interface::VPtr pSourceV = Interface::halfedgeSource(pHE);
		//	cout << "Target vertex id: " << Interface::vertexId(pTargetV) << " Source vertex id: " << Interface::vertexId(pSourceV) << endl;
		//}
		
		getchar();

	}

	getchar();

	Interface::VPtr pV = mesh.idVertex(10);
	Iterators::VCcwOutHEIterator vheiter(pV);
	for (Interface::HEPtr pHE : vheiter) {
		std::cout << "This is the halfedge pointer:" << (int)pHE << std::endl;
	}
	  
	auto find_zero = [] (const int& a) {return a == 0; };
	auto zeros = std::find_if(v.begin(), v.end(), find_zero);

	/*Iterators::VCcwOutHEIterator vccwheiter(pV);
	for (Interface::HEPtr pHE : vccwheiter) {
		std::cout << "This is the halfedge pointer:" << (int)pHE << std::endl;
	}*/
	getchar();
}