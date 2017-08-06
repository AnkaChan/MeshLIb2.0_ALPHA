#pragma once

//2017.07.25 Extened from 
//https://stackoverflow.com/questions/20225392/c-dynamic-properties

#include <map>
#include <list>
#include <algorithm>

namespace MeshLib
{
	class BaseHandle;
	class Prop
	{
	public:
		virtual ~Prop() {};
	};

	template<typename DataType>
	class Property : public Prop
	{
	public:
		virtual ~Property() {};

		Property(DataType d) { m_data = d; };
		DataType & data() { return m_data; };
	private:
		DataType m_data;
	};

	class Props
	{
	public:
		~Props()
		{
			std::vector<Prop*>::iterator iter;
			for (iter = m_props.begin(); iter != m_props.end(); ++iter)
				delete *iter;
			m_props.clear();
		};
		std::vector<Prop *> & pvector() { return m_props; };
		Prop * pvector(int index) { return m_props[index]; };
		void pvector(Prop * pp) { m_props.push_back(pp); };
	private:
		std::vector<Prop*> m_props;
	};

	class BaseHandle
	{
	public:
		int & id() { return m_id; };

	private:
		int m_id;
	};

	template<typename DataType>
	class VPropHandle : public BaseHandle
	{
		typedef DataType DType;
	};

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	class PropsManage
	{
		typedef MeshLib::CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>	Mesh;

	public:
		PropsManage(Mesh * pm) { pmesh = pm; };

		//add property
		template<typename DataType>
		void addproperty(VPropHandle<DataType> & vhandle, DataType data);

		template<typename DataType>
		DataType & property(VertexType * pv, VPropHandle<DataType> vhandle);

		//delete property
		//template<typename DataType>
		//bool DeleteVProp(VPropHandle<DataType> vhandle);

		//find property
		//bool FindVProp(VPropHandle<DataType> vhandle);

		//Local Property

		std::list<std::pair<VertexType *, std::string>> & vlocpro() { return m_vlocp; };

		std::list<std::pair<VertexType *, std::string>> & elocpro() { return m_elocp; };

		std::list<std::pair<VertexType *, std::string>> & flocpro() { return m_flocp; };

		std::list<std::pair<VertexType *, std::string>> & hlocpro() { return m_hlocp; };

	private:
		Mesh * pmesh;
		std::list<BaseHandle *> m_vhandles;
		std::list<BaseHandle *> m_ehandles;
		std::list<BaseHandle *> m_fhandles;
		std::list<BaseHandle *> m_hhandles;
		//for local property
		std::list<std::pair<VertexType *, BaseHandle *>>		m_vlocp;
		std::list<std::pair<EdgeType *, BaseHandle *>>			m_elocp;
		std::list<std::pair<FaceType *, BaseHandle *>>			m_flocp;
		std::list<std::pair<HalfEdgeType *, BaseHandle *>>		m_hlocp;
	};


	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	void PropsManage<VertexType, EdgeType, FaceType, HalfEdgeType>::addproperty(
		VPropHandle<DataType> & vhandle, DataType data)
	{
		vhandle.id() = m_vhandles.size();
		m_vhandles.push_back(&vhandle);
		for (auto viter = pmesh->vertices().begin();
			viter != pmesh->vertices().end(); ++viter)
		{
			Property<DataType> * propp = new Property<DataType>(data);
			(*viter)->props().pvector(propp);
		}
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	DataType & PropsManage<VertexType, EdgeType, FaceType, HalfEdgeType>::property(VertexType * pv, VPropHandle<DataType> vhandle)
	{
		return ((Property<DataType> *)pv->props().pvector(vhandle.id()))->data();
	}

}