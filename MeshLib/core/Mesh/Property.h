#pragma once

//2017.07.25 Extened from 
//https://stackoverflow.com/questions/20225392/c-dynamic-properties

#include <map>
#include <list>
#include <algorithm>

class Prop
{
public:
	virtual ~Prop() {};
};

template<typename T>
class Property : public Prop
{
public:
	virtual ~Property() {};

	Property(T d) { data = d; };
	T GetValue() { return data; };
	void WriteValue(T d) { data = d; };
private:
	T data;
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

	template<typename T>
	T Get(std::string name)
	{
		Property<T>* p = (Property<T>*)props[name];
		return p->GetValue();
	};

	template<typename T>
	void Write(std::string name, T data)
	{
		Property<T>* p = (Property<T>*)props[name];
		p->WriteValue(data);
	}

	template<typename T>
	void Add(std::string name, T data)
	{
		props[name] = new Property<T>(data);
	};

	template<typename T>
	void Delete(std::string name)
	{
		Property<T>* p = (Property<T>*)props[name];
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

template<typename V, typename E, typename F, typename H>
class PropsHandle
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;

public:
	PropsHandle(MeshLib::CBaseMesh<V, E, F, H> * pm) { pmesh = pm; };
	//friend element
	friend class VLPIterator;
	//add property
	template<typename T>
	bool AddVProp(std::string name, T data);

	template<typename T>
	bool AddEProp(std::string name, T data);

	template<typename T>
	bool AddFProp(std::string name, T data);

	template<typename T>
	bool AddHProp(std::string name, T data);

	//delete property
	template<typename T>
	bool DeleteVProp(std::string name);

	template<typename T>
	bool DeleteEProp(std::string name);

	template<typename T>
	bool DeleteFProp(std::string name);

	template<typename T>
	bool DeleteHProp(std::string name);

	//find property
	bool FindVProp(std::string name);
	
	bool FindEProp(std::string name);
	
	bool FindFProp(std::string name);

	bool FindHProp(std::string name);

	//Local Property
	//add local prop
	template<typename T>
	bool AddLocProp(V * pv, std::string name, T data);

	template<typename T>
	bool AddLocProp(E * pe, std::string name, T data);

	template<typename T>
	bool AddLocProp(F * pf, std::string name, T data);

	template<typename T>
	bool AddLocProp(H * ph, std::string name, T data);
	
	//delete local prop
	template<typename T>
	void DeleteVLocProp(std::string name);
	
	template<typename T>
	void DeleteELocProp(std::string name);

	template<typename T>
	void DeleteFLocProp(std::string name);

	template<typename T>
	void DeleteHLocProp(std::string name);

private:
	Mesh * pmesh;
	std::list<std::string> vnames;
	std::list<std::string> enames;
	std::list<std::string> fnames;
	std::list<std::string> hnames;
	//for local property
	std::list<std::pair<V *, std::string>> vlocp;
	std::list<std::pair<E *, std::string>> elocp;
	std::list<std::pair<F *, std::string>> flocp;
	std::list<std::pair<H *, std::string>> hlocp;
};

template<typename V, typename E, typename F, typename H>
class VLPIterator 
{
	typedef PropsHandle<V, E, F, H>	PH;
	typedef std::pair<V*, std::string> VSPair;
	typedef std::list<VSPair> VSPList;
public:

	VLPIterator(PH & ppropsh, std::string name0)
	{
		m_tlist = ppropsh.vlocp;
		m_name = name0;
		for (auto piter = m_tlist.begin(); ; ++piter)
		{
			if (piter == m_tlist.end())
			{
				m_iter = m_tlist.end();
			}
			VSPair vs_temp = *piter;
			if (vs_temp.second == m_name)
			{
				m_iter = piter;
				break;
			}
		}
	}
	VSPair value() { reutrn *m_iter; };
	VSPair operator*() { return value(); };

	void operator++() {};
	void operator++(int) {};

private:
	VSPList &	m_tlist;
	VSPList::Iterator m_iter;
	VSPair		m_tspair;
	std::string	m_name;
};

//add property
template<typename V, typename E, typename F, typename H>
bool PropsHandle<V, E, F, H>::AddVProp(std::string name, T data)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshVertexIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(vnames.begin(), vnames.end(), name);
	if (niter != vnames.end())
		return false;
	else
		vnames.push_back(name);

	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		V * pelement = *iter;
		pelement->props().Add<T>(name, data);
	}
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddEProp(std::string name, T data)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshEdgeIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(enames.begin(), enames.end(), name);
	if (niter != enames.end())
		return false;
	else
		enames.push_back(name);

	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		E * pelement = *iter;
		pelement->props().Add<T>(name, data);
	}
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddFProp(std::string name, T data)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshFaceIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(fnames.begin(), fnames.end(), name);
	if (niter != fnames.end())
		return false;
	else
		fnames.push_back(name);

	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		F * pelement = *iter;
		pelement->props().Add<T>(name, data);
	}
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddHProp(std::string name, T data)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshHalfEdgeIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(hnames.begin(), hnames.end(), name);
	if (niter != hnames.end())
		return false;
	else
		hnames.push_back(name);

	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		H * pelement = *iter;
		pelement->props().Add<T>(name, data);
	}
	return true;
}

//delete property
template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::DeleteVProp(std::string name)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshVertexIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(vnames.begin(), vnames.end(), name);
	if (niter == vnames.end())
		return false;
	else
		vnames.erase(niter);
	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		V * pelement = *iter;
		pelement->props().Delete<T>(name);
	}
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::DeleteEProp(std::string name)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshVertexIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(enames.begin(), enames.end(), name);
	if (niter == enames.end())
		return false;
	else
		enames.erase(niter);
	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		E * pelement = *iter;
		pelement->props().Delete<T>(name);
	}
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::DeleteFProp(std::string name)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshFaceIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(fnames.begin(), fnames.end(), name);
	if (niter == fnames.end())
		return false;
	else
		fnames.erase(niter);
	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		F * pelement = *iter;
		pelement->props().Delete<T>(name);
	}
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::DeleteHProp(std::string name)
{
	typedef MeshLib::CBaseMesh<V, E, F, H>	Mesh;
	typedef MeshLib::MeshHalfEdgeIterator<V, E, F, H>	Iterator;

	std::list<std::string>::iterator niter;
	niter = std::find(hnames.begin(), hnames.end(), name);
	if (niter == hnames.end())
		return false;
	else
		hnames.erase(niter);
	for (Iterator iter(pmesh); !iter.end(); ++iter)
	{
		H * pelement = *iter;
		pelement->props().Delete<T>(name);
	}
	return true;
}

//find property
template<typename V, typename E, typename F, typename H>
bool PropsHandle<V, E, F, H>::FindVProp(std::string name)
{
	std::list<std::string>::iterator iter;
	iter = std::find(vnames.begin(), vnames.end(), name);
	if (iter == vnames.end())
		return false;
	else
		return true;
}

template<typename V, typename E, typename F, typename H>
bool PropsHandle<V, E, F, H>::FindEProp(std::string name)
{
	std::list<std::string>::iterator iter;
	iter = std::find(names.begin(), names.end(), name);
	if (iter == names.end())
		return false;
	else
		return true;
}

template<typename V, typename E, typename F, typename H>
bool PropsHandle<V, E, F, H>::FindFProp(std::string name)
{
	std::list<std::string>::iterator iter;
	iter = std::find(fnames.begin(), fnames.end(), name);
	if (iter == names.end())
		return false;
	else
		return true;
}

template<typename V, typename E, typename F, typename H>
bool PropsHandle<V, E, F, H>::FindHProp(std::string name)
{
	std::list<std::string>::iterator iter;
	iter = std::find(hnames.begin(), hnames.end(), name);
	if (iter == hnames.end())
		return false;
	else
		return true;
}

//Local Property
template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddLocProp(V * pv, std::string name, T data)
{
	if (pv->props().Find(name))
		return false;
	if (std::find(vnames.begin(), vnames.end(), name) == vnames.end())
		vnames.push_back(name);

	std::pair<V *, std::string> vnpair{ pv, name };
	vlocp.push_back(vnpair);
	pv->props().Add<T>(name, data);
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddLocProp(E * pe, std::string name, T data)
{
	if (pe->props().Find(name))
		return false;
	if (std::find(enames.begin(), enames.end(), name) == enames.end())
		enames.push_back(name);

	std::pair<E *, std::string> enpair{ pe, name };
	elocp.push_back(enpair);
	pe->props().Add<T>(name, data);
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddLocProp(F * pf, std::string name, T data)
{
	if (pf->props().Find(name))
		return false;
	if (std::find(fnames.begin(), fnames.end(), name) == fnames.end())
		fnames.push_back(name);

	std::pair<F *, std::string> fnpair{ pe, name };
	flocp.push_back(fnpair);
	pf->props().Add<T>(name, data);
	return true;
}

template<typename V, typename E, typename F, typename H>
template<typename T>
bool PropsHandle<V, E, F, H>::AddLocProp(H * ph, std::string name, T data)
{
	if (ph->props().Find(name))
		return false;
	if (std::find(hnames.begin(), hnames.end(), name) == hnames.end())
		hnames.push_back(name);

	std::pair<H *, std::string> hnpair{ ph, name };
	hlocp.push_back(hnpair);
	ph->props().Add<T>(name, data);
	return true;
}

//Delete Local Property
template<typename V, typename E, typename F, typename H>
template<typename T>
void PropsHandle<V, E, F, H>::DeleteVLocProp(std::string name)
{
	std::list<std::pair<V *, std::string>>::iterator vniter;
	for ( vniter = vlocp.begin(); vniter != vlocp.end();)
	{
		std::pair<V *, std::string> vnpair = *vniter;
		if (vnpair.second == name)
		{
			V * pv = vnpair.first;
			pv->props().Delete<T>(name);
			vniter = vlocp.erase(vniter);
		}
		else
		{
			vniter++;
		}
	}
}

template<typename V, typename E, typename F, typename H>
template<typename T>
void PropsHandle<V, E, F, H>::DeleteELocProp(std::string name)
{
	std::list<std::pair<E *, std::string>>::iterator eniter;
	for (eniter = elocp.begin(); eniter != elocp.end();)
	{
		std::pair<E *, std::string> enpair = *eniter;
		if (enpair.second == name)
		{
			E * pe = enpair.first;
			pe->props().Delete<T>(name);
			eniter = elocp.erase(eniter);
		}
		else
		{
			eniter++;
		}
	}
}

template<typename V, typename E, typename F, typename H>
template<typename T>
void PropsHandle<V, E, F, H>::DeleteFLocProp(std::string name)
{
	std::list<std::pair<F *, std::string>>::iterator fniter;
	for (fniter = flocp.begin(); fniter != flocp.end();)
	{
		std::pair<F *, std::string> fnpair = *fniter;
		if (fnpair.second == name)
		{
			F * pf = fnpair.first;
			pf->props().Delete<T>(name);
			fniter = flocp.erase(fniter);
		}
		else
		{
			fniter++;
		}
	}
}

template<typename V, typename E, typename F, typename H>
template<typename T>
void PropsHandle<V, E, F, H>::DeleteHLocProp(std::string name)
{
	std::list<std::pair<H *, std::string>>::iterator hniter;
	for (hniter = hlocp.begin(); hniter != hlocp.end();)
	{
		std::pair<H *, std::string> hnpair = *hniter;
		if (hnpair.second == name)
		{
			H * ph = hnpair.first;
			ph->props().Delete<T>(name);
			hniter = hlocp.erase(hniter);
		}
		else
		{
			hniter++;
		}
	}
}

