#pragma once
#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include "GuassianMap.h"
#include <iostream>
#include <cmath>
#include <omp.h>

#define STRING_EN(pE) (pE->k*pow((If::edgeVertex1(pE)->point()-If::edgeVertex2(pE)->point()).norm(),2))
#define PRINTING_COUNT 1000
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
		void setInitalMap(MeshType* newpMesh);
		void map();
		void guassianMap();
		void centerVisualMap(CPoint center);
		void setStep(double newStep);
		void setStopEpsion(double newEpsion);
		double totalEnergy();
		bool adjustPointVisualOneStep();
		void iterativelyAdjustPoint();
		void iterativelyAdjustPoint_parallel();
		bool dynamicStep = false;
		void setDynamicStepSize(double newSize);

	private:
		MeshType * pMesh;
		void calculateStringConstraints();
		double halfedgeStringEnergy(HE * pHE);
		double step = 0.001;
		double Epsion = 0.00001;
		double dynamicStepSize = 50;
	};

	template <typename If>
	struct SphericalHarmonicMap : public SphericalHarmonicMapCore<typename If::VType, typename If::EType, typename If::FType, typename If::HEType> {};

	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::setInputMesh(MeshType* newpMesh)
	{
		pMesh = newpMesh;
		pMesh->prepare_for_parallel();
	}

	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::setInitalMap(MeshType * pInitialMapMesh)
	{
		calculateStringConstraints();
		for (If::VPtr pV : It::MVIterator(pMesh)) {
			pV->point() = (pInitialMapMesh->idVertex(pV->id()))->point();
		}
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
	inline void SphericalHarmonicMapCore<V, E, F, HE>::setStep(double newStep)
	{
		step = newStep;
	}
	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::setStopEpsion(double newEpsion)
	{
		Epsion = newEpsion;
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
		double minErr = 1000000000;

		double formalEnergy, currentEnergy;
		currentEnergy = totalEnergy();
		int count = 0;
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
				if (!dynamicStep) {
					tangentComponent /= sqrt(tangentComponent.norm());
				}
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
			++count;
			if (count >= PRINTING_COUNT) {
				count = 0;
				std::cout << "New Harmonic Energy: " << currentEnergy << ". Step ERR: " << abs(currentEnergy - formalEnergy) << "\n";
			}
			if (dynamicStep) {
				if (abs(currentEnergy - formalEnergy) < minErr) {
					minErr = abs(currentEnergy - formalEnergy);
					if (dynamicStepSize * minErr < step) {
						step = dynamicStepSize * minErr;
					}
				}
			}
		} while (abs(currentEnergy - formalEnergy) > Epsion);
		std::cout << "Algorithm has converged" << std::endl;
	}

	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::iterativelyAdjustPoint_parallel()
	{
		int numV = pMesh->vertices().size();

		double minErr = 1000000000;
		double newCenter_0, newCenter_1, newCenter_2;

		double formalEnergy, currentEnergy;
		currentEnergy = totalEnergy();
		int count = 0;
		do {
			newCenter_0 = 0;
			newCenter_1 = 0;
			newCenter_2 = 0;
#pragma omp parallel for reduction(+:newCenter_0, newCenter_1, newCenter_2)
			for (int i = 0; i < numV; ++i) {
				If::VPtr pV = pMesh->vertices_vec[i];
				CPoint nP;
				CPoint& P = pV->point();
				double totalK = 0.0;

				for (auto pNV : It::VVIterator(pV)) {
					auto pE = If::vertexEdge(pV, pNV);
					nP += pNV->point()*pE->k;
					totalK += pE->k;
				}
				CPoint d = P - nP / totalK;
				CPoint tangentComponent = d - (P * d) * P;
				pV->newPos = P - step * tangentComponent;
				if (!dynamicStep) {
					tangentComponent /= sqrt(tangentComponent.norm());
				}
				//tangentComponent /= tangentComponent.norm();
				newCenter_0 += pV->newPos[0];
				newCenter_1 += pV->newPos[1];
				newCenter_2 += pV->newPos[2];
			}
			CPoint newCenter(newCenter_0, newCenter_1, newCenter_2);
			newCenter /= numV;

#pragma omp parallel for
			for (int i = 0; i < numV; ++i) {
				If::VPtr pV = pMesh->vertices_vec[i];
				CPoint& P = pV->point();
				P = pV->newPos - newCenter;
				P /= P.norm();
			}
			formalEnergy = currentEnergy;
			currentEnergy = totalEnergy();
			++count;
			if (count >= PRINTING_COUNT) {
				count = 0;
				std::cout << "New Harmonic Energy: " << currentEnergy << ". Step ERR: " << abs(currentEnergy - formalEnergy) << "\n";
			}
			if (dynamicStep) {
				if (abs(currentEnergy - formalEnergy) < minErr) {
					minErr = abs(currentEnergy - formalEnergy);
					if (dynamicStepSize * minErr < step) {
#pragma omp critical 
						{
							step = dynamicStepSize * minErr;
						}
					}
				}
			}
		} while (abs(currentEnergy - formalEnergy) > Epsion);
		std::cout << "Algorithm has converged" << std::endl;
	}

	template<typename V, typename E, typename F, typename HE>
	inline void SphericalHarmonicMapCore<V, E, F, HE>::setDynamicStepSize(double newSize)
	{
		dynamicStepSize = newSize;
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
			if (!dynamicStep) {
				tangentComponent /= sqrt(tangentComponent.norm());
			}
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