#pragma once
#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include "GuassianMap.h"
#include <iostream>
#include <cmath>

#define STRING_EN(pE) (pE->k*pow((If::edgeVertex1(pE)->point()-If::edgeVertex2(pE)->point()).norm(),2))

namespace MeshLib {
	struct _edgeSphericalHarmonic {
		double k;
	};
	struct _vectexSphericalHarmonic {
		CPoint newPos;
	};

	template<typename V, typename E,  typename F, typename HE>
	class SphericalHarmonicMapCore {
	private:
		typedef CInterface<V, E, F, HE> If;
		typedef CIterators<If> It;
		typedef CBaseMesh<V, E, F, HE> MeshType;
	public:
		SphericalHarmonicMapCore() {};
		void setInputMesh(MeshType* newpMesh);
		void map();
		void guassianMap();
		void centerVisualMap(CPoint center);
		void setStep(double newStep);
		void setStopEpsion(double newEpsion);
		double totalEnergy();
		bool adjustPointVisualOneStep();

	private:
		MeshType * pMesh;
		void calculateStringConstraints();
		void iterativelyAdjustPoint();
		double halfedgeStringEnergy(HE * pHE);
		double step = 0.001;
		double Epsion = 0.00001;
	};

	template <typename If>
	struct SphericalHarmonicMap : public SphericalHarmonicMapCore<typename If::VType, typename If::EType, typename If::FType, typename If::HEType> {};

	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::setInputMesh(MeshType* newpMesh)
	{
		pMesh = newpMesh;
	}

	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::map()
	{
		calculateStringConstraints();

		std::cout << "Applying Guassian Map.\n";
		GuassianMap guassianMapper;
		guassianMapper.setInputMesh(pMesh);
		guassianMapper.map();

		std::cout << "Iteratively Adjusting Point.\n";
		iterativelyAdjustPoint();
	}
	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::guassianMap()
	{
		calculateStringConstraints();

		std::cout << "Applying Guassian Map.\n";
		GuassianMap guassianMapper;
		guassianMapper.setInputMesh(pMesh);
		guassianMapper.map();

	}
	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::centerVisualMap(CPoint center)
	{
		calculateStringConstraints();

		std::cout << "Applying Center Visual Map.\n";

		for (V* pV : It::MVIterator(pMesh)) {
			CPoint & vP = pV->point();
			CPoint newP = vP - center;
			newP /= newP.norm();
			vP = newP;
		}
	}
	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::calculateStringConstraints()
	{
		for (E* pE : It::MEIterator(pMesh)) {
			pE->k = halfedgeStringEnergy(If::edgeHalfedge(pE, 0)) + halfedgeStringEnergy(If::edgeHalfedge(pE, 1));
		}
	}
	template<typename V, typename E, typename F, typename HE>
	void SphericalHarmonicMapCore<V, E, F, HE>::iterativelyAdjustPoint()
	{
		int numV = pMesh->vertices().size();
		int fixedVId0 = 1;
		int fixedVId1 = numV / 2;
		int fixedVId2 = numV - 2;

		double formalEnergy, currentEnergy;
		currentEnergy = totalEnergy();
		do{
			CPoint newCenter(0,0,0);
			for (auto pV : It::MVIterator(pMesh)) {
				CPoint nP; 
				CPoint& P = pV->point();
				double totalK = 0.0; 
				int vId = pV->id();
				if (vId == fixedVId0 || vId == fixedVId1 || vId == fixedVId2) {
					//continue;
				}
				for (auto pNV : It::VVIterator(pV)) { 
					auto pE = If::vertexEdge(pV, pNV); 
					nP += pNV->point()*pE->k; 
					totalK += pE->k; 
				}
				CPoint d = P - nP / totalK;
				CPoint tangentComponent = d - (P * d) * P;
				pV->newPos = P - step * tangentComponent;
				//tangentComponent /= tangentComponent.norm();
				newCenter += pV->newPos;
			}
			newCenter /= numV;

			for (auto pV : It::MVIterator(pMesh)) {
				CPoint& P = pV->point();
				P = pV->newPos - newCenter;
				P /= P.norm();
			}
			formalEnergy = currentEnergy;
			currentEnergy = totalEnergy();
			std::cout << "New Harmonic Energy: " << currentEnergy << std::endl;
		} while (abs(currentEnergy - formalEnergy) > Epsion);
		std::cout << "Algorithm has converged" << std::endl;
	}

	template<typename V, typename E, typename F, typename HE>
	inline bool SphericalHarmonicMapCore<V, E, F, HE>::adjustPointVisualOneStep()
	{
		int numV = pMesh->vertices().size();

		double formalEnergy, currentEnergy;
		currentEnergy = totalEnergy();
		CPoint newCenter(0, 0, 0);
		for (auto pV : It::MVIterator(pMesh)) {
			CPoint nP;
			CPoint& P = pV->point();
			double totalK = 0.0;
			int vId = pV->id();
	
			for (auto pNV : It::VVIterator(pV)) {
				auto pE = If::vertexEdge(pV, pNV);
				nP += pNV->point()*pE->k;
				totalK += pE->k;
			}
			CPoint d = P - nP / totalK;
			CPoint tangentComponent = d - (P * d) * P;
			tangentComponent /= sqrt(tangentComponent.norm());
			pV->newPos = P - step * tangentComponent;
			newCenter += pV->newPos;
		}
		newCenter /= numV;
		for (auto pV : It::MVIterator(pMesh)) {
			CPoint& P = pV->point();
			P = pV->newPos - newCenter;
			P /= P.norm();
		}
		formalEnergy = currentEnergy;
		currentEnergy = totalEnergy();
		std::cout << "New Harmonic Energy: " << currentEnergy << std::endl;
		if (abs(currentEnergy - formalEnergy) > Epsion) {
			return false;
		}
		else {
			return true;
		}
	}

	template<typename V, typename E, typename F, typename HE>
	inline double SphericalHarmonicMapCore<V, E, F, HE>::halfedgeStringEnergy(HE * pHE)
	{
		CPoint v0 = If::halfedgeVec(If::halfedgeNext(pHE));
		CPoint v1 = If::halfedgeVec(If::halfedgePrev(pHE));

		return -(v0*v1)/(v0^v1).norm();
	}
	template<typename V, typename E, typename F, typename HE>
	inline double SphericalHarmonicMapCore<V, E, F, HE>::totalEnergy()
	{
		double e = 0.0; 
		for (auto pE : It::MEIterator(pMesh)){ 
			e += STRING_EN(pE); 
		} 
		return e;
	}
}