#pragma once

//2017.07.25 Extened from 
//https://stackoverflow.com/questions/20225392/c-dynamic-properties

#include <map>
#include <list>
#include <algorithm>

namespace MeshLib
{
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

		Property(DataType d) { data = d; };
		DataType GetValue() { return data; };
		void WriteValue(DataType d) { data = d; };
	private:
		DataType data;
	};

	class Props
	{
	public:
		~Props()
		{
			std::map<std::string, Prop*>::iterator iter;
			for (iter = props.begin(); iter != props.end(); ++iter)
				delete (*iter).second;
			props.clear();
		};

		template<typename DataType>
		DataType Get(std::string name)
		{
			Property<DataType>* p = (Property<DataType>*)props[name];
			return p->GetValue();
		};

		template<typename DataType>
		void Write(std::string name, DataType data)
		{
			Property<DataType>* p = (Property<DataType>*)props[name];
			p->WriteValue(data);
		}

		template<typename DataType>
		void Add(std::string name, DataType data)
		{
			props[name] = new Property<DataType>(data);
		};

		template<typename DataType>
		void Delete(std::string name)
		{
			Property<DataType>* p = (Property<DataType>*)props[name];
			delete p;
			props.erase(name);
		}

		bool Find(std::string name)
		{
			std::map<std::string, Prop*>::iterator iter;
			iter = props.find(name);
			if (iter == props.end())
				return false;
			else
				return true;
		}

	private:
		std::map<std::string, Prop*> props;
	};

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	class PropsHandle
	{
		typedef MeshLib::CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType>	Mesh;

	public:
		PropsHandle(Mesh * pm) { pmesh = pm; };

		//add property
		template<typename DataType>
		bool AddVProp(std::string name, DataType data);

		template<typename DataType>
		bool AddEProp(std::string name, DataType data);

		template<typename DataType>
		bool AddFProp(std::string name, DataType data);

		template<typename DataType>
		bool AddHProp(std::string name, DataType data);

		//delete property
		template<typename DataType>
		bool DeleteVProp(std::string name);

		template<typename DataType>
		bool DeleteEProp(std::string name);

		template<typename DataType>
		bool DeleteFProp(std::string name);

		template<typename DataType>
		bool DeleteHProp(std::string name);

		//find property
		bool FindVProp(std::string name);

		bool FindEProp(std::string name);

		bool FindFProp(std::string name);

		bool FindHProp(std::string name);

		//Local Property
		//add local prop
		template<typename DataType>
		bool AddLocProp(VertexType * pv, std::string name, DataType data);

		template<typename DataType>
		bool AddLocProp(EdgeType * pe, std::string name, DataType data);

		template<typename DataType>
		bool AddLocProp(FaceType * pf, std::string name, DataType data);

		template<typename DataType>
		bool AddLocProp(HalfEdgeType * ph, std::string name, DataType data);

		//delete local prop
		template<typename DataType>
		void DeleteVLocProp(std::string name);

		template<typename DataType>
		void DeleteELocProp(std::string name);

		template<typename DataType>
		void DeleteFLocProp(std::string name);

		template<typename DataType>
		void DeleteHLocProp(std::string name);

		std::list<std::pair<VertexType *, std::string>> & vlocpro() { return m_vlocp; };

		std::list<std::pair<VertexType *, std::string>> & elocpro() { return m_elocp; };

		std::list<std::pair<VertexType *, std::string>> & flocpro() { return m_flocp; };

		std::list<std::pair<VertexType *, std::string>> & hlocpro() { return m_hlocp; };

	private:
		Mesh * pmesh;
		std::list<std::string> m_vnames;
		std::list<std::string> m_enames;
		std::list<std::string> m_fnames;
		std::list<std::string> m_hnames;
		//for local property
		std::list<std::pair<VertexType *, std::string>>		m_vlocp;
		std::list<std::pair<EdgeType *, std::string>>		m_elocp;
		std::list<std::pair<FaceType *, std::string>>		m_flocp;
		std::list<std::pair<HalfEdgeType *, std::string>>	m_hlocp;
	};

	//add property
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddVProp(std::string name, DataType data)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_vnames.begin(), m_vnames.end(), name);
		if (niter != m_vnames.end())
			return false;
		else
			m_vnames.push_back(name);

		for (VertexType * pv : pmesh->vertices())
		{
			pv->props().Add<DataType>(name, data);
		}

		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddEProp(std::string name, DataType data)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_enames.begin(), m_enames.end(), name);
		if (niter != m_enames.end())
			return false;
		else
			m_enames.push_back(name);

		for (EdgeType * pe : pmesh->edges())
		{
			pe->props().Add<DataType>(name, data);
		}
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddFProp(std::string name, DataType data)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_fnames.begin(), m_fnames.end(), name);
		if (niter != m_fnames.end())
			return false;
		else
			m_fnames.push_back(name);

		for (FaceType * pf : pmesh->faces())
		{
			pf->props().Add<DataType>(name, data);
		}
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddHProp(std::string name, DataType data)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_hnames.begin(), m_hnames.end(), name);
		if (niter != m_hnames.end())
			return false;
		else
			m_hnames.push_back(name);

		for (FaceType * pf : pmesh->faces())
		{
			HalfEdgeType * ph = pmesh->faceHalfedge(pf);
			do
			{
				ph->props().Add<DataType>(name, data);
				ph = pmesh->halfedgeNext(ph);
			} while (ph != pmesh->faceHalfedge(pf));
		}
		return true;
	}

	//delete property
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteVProp(std::string name)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_vnames.begin(), m_vnames.end(), name);
		if (niter == m_vnames.end())
			return false;
		else
			m_vnames.erase(niter);

		for (VertexType * pv : pmesh->vertices())
		{
			pv->props().Delete<DataType>(name);
		}
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteEProp(std::string name)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_enames.begin(), m_enames.end(), name);
		if (niter == m_enames.end())
			return false;
		else
			m_enames.erase(niter);

		for (EdgeType * pe : pmesh->edges())
		{
			pe->props().Delete<DataType>(name);
		}
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteFProp(std::string name)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_fnames.begin(), m_fnames.end(), name);
		if (niter == m_fnames.end())
			return false;
		else
			m_fnames.erase(niter);
		for (FaceType * pf : pmesh->faces())
		{
			pf->props().Delete<DataType>(name);
		}
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteHProp(std::string name)
	{
		std::list<std::string>::iterator niter;
		niter = std::find(m_hnames.begin(), m_hnames.end(), name);
		if (niter == m_hnames.end())
			return false;
		else
			m_hnames.erase(niter);

		for (FaceType * pf : pmesh->faces())
		{
			HalfEdgeType * ph = pmesh->faceHalfedge(pf);
			do
			{
				ph->props().Delete<DataType>(name);
				ph = pmesh->halfedgeNext(ph);
			} while (ph != pmesh->faceHalfedge(pf));
		}
		return true;
	}

	//find property
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::FindVProp(std::string name)
	{
		std::list<std::string>::iterator iter;
		iter = std::find(m_vnames.begin(), m_vnames.end(), name);
		if (iter == m_vnames.end())
			return false;
		else
			return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::FindEProp(std::string name)
	{
		std::list<std::string>::iterator iter;
		iter = std::find(m_enames.begin(), m_enames.end(), name);
		if (iter == m_enames.end())
			return false;
		else
			return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::FindFProp(std::string name)
	{
		std::list<std::string>::iterator iter;
		iter = std::find(m_fnames.begin(), m_fnames.end(), name);
		if (iter == m_fnames.end())
			return false;
		else
			return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::FindHProp(std::string name)
	{
		std::list<std::string>::iterator iter;
		iter = std::find(m_hnames.begin(), m_hnames.end(), name);
		if (iter == m_hnames.end())
			return false;
		else
			return true;
	}

	//Local Property

	//Add local property
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddLocProp(VertexType * pv, std::string name, DataType data)
	{
		if (pv->props().Find(name))
			return false;
		if (std::find(m_vnames.begin(), m_vnames.end(), name) == m_vnames.end())
			m_vnames.push_back(name);

		std::pair<VertexType *, std::string> vnpair{ pv, name };
		m_vlocp.push_back(vnpair);
		pv->props().Add<DataType>(name, data);
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddLocProp(EdgeType * pe, std::string name, DataType data)
	{
		if (pe->props().Find(name))
			return false;
		if (std::find(m_enames.begin(), m_enames.end(), name) == m_enames.end())
			m_enames.push_back(name);

		std::pair<EdgeType *, std::string> enpair{ pe, name };
		m_elocp.push_back(enpair);
		pe->props().Add<DataType>(name, data);
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddLocProp(FaceType * pf, std::string name, DataType data)
	{
		if (pf->props().Find(name))
			return false;
		if (std::find(m_fnames.begin(), m_fnames.end(), name) == m_fnames.end())
			m_fnames.push_back(name);

		std::pair<FaceType *, std::string> fnpair{ pf, name };
		m_flocp.push_back(fnpair);
		pf->props().Add<DataType>(name, data);
		return true;
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	bool PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::AddLocProp(HalfEdgeType * ph, std::string name, DataType data)
	{
		if (ph->props().Find(name))
			return false;
		if (std::find(m_hnames.begin(), m_hnames.end(), name) == m_hnames.end())
			m_hnames.push_back(name);

		std::pair<HalfEdgeType *, std::string> hnpair{ ph, name };
		m_hlocp.push_back(hnpair);
		ph->props().Add<DataType>(name, data);
		return true;
	}

	//Delete Local Property
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	void PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteVLocProp(std::string name)
	{
		std::list<std::pair<VertexType *, std::string>>::iterator vniter;
		for (vniter = m_vlocp.begin(); vniter != m_vlocp.end();)
		{
			std::pair<VertexType *, std::string> vnpair = *vniter;
			if (vnpair.second == name)
			{
				VertexType * pv = vnpair.first;
				pv->props().Delete<DataType>(name);
				vniter = m_vlocp.erase(vniter);
			}
			else
			{
				vniter++;
			}
		}
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	void PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteELocProp(std::string name)
	{
		std::list<std::pair<EdgeType *, std::string>>::iterator eniter;
		for (eniter = m_elocp.begin(); eniter != m_elocp.end();)
		{
			std::pair<EdgeType *, std::string> enpair = *eniter;
			if (enpair.second == name)
			{
				EdgeType * pe = enpair.first;
				pe->props().Delete<DataType>(name);
				eniter = m_elocp.erase(eniter);
			}
			else
			{
				eniter++;
			}
		}
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	void PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteFLocProp(std::string name)
	{
		std::list<std::pair<FaceType *, std::string>>::iterator fniter;
		for (fniter = m_flocp.begin(); fniter != m_flocp.end();)
		{
			std::pair<FaceType *, std::string> fnpair = *fniter;
			if (fnpair.second == name)
			{
				FaceType * pf = fnpair.first;
				pf->props().Delete<DataType>(name);
				fniter = m_flocp.erase(fniter);
			}
			else
			{
				fniter++;
			}
		}
	}

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	template<typename DataType>
	void PropsHandle<VertexType, EdgeType, FaceType, HalfEdgeType>::DeleteHLocProp(std::string name)
	{
		std::list<std::pair<HalfEdgeType *, std::string>>::iterator hniter;
		for (hniter = m_hlocp.begin(); hniter != m_hlocp.end();)
		{
			std::pair<HalfEdgeType *, std::string> hnpair = *hniter;
			if (hnpair.second == name)
			{
				HalfEdgeType * ph = hnpair.first;
				ph->props().Delete<DataType>(name);
				hniter = m_hlocp.erase(hniter);
			}
			else
			{
				hniter++;
			}
		}
	}

}