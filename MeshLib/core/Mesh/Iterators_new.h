#pragma once
#include <iterator>
#include <algorithm>
#include <list>
#include <vector>

#include "Interface.h"
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"
#include "HalfEdge.h"

namespace MeshLib {
	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	class CIterators : private CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>{
	public:
		typedef std::list<CHalfEdge*> CHalfEdgePtrList;
		typedef std::list<CHalfEdge*>::iterator CHalfEdgePtrListIterator;
		using CInterface::MeshPtr;//may not work ?

		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VOutHEIterator(const VertexPtr& pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VOutHEIterator(const VertexPtr& pV, const CHalfEdgePtrListIterator& iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return (HalfEdgePtr)*_iter; }
			HalfEdgePtr& value() { return (HalfEdgePtr)*_iter; }

			VOutHEIterator begin() { return VOutHEIterator(_pV);  }
			VOutHEIterator end() { return VOutHEIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:      
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		/* 
		* Iterator to access the out halfedge of a vertex in counter-clockwise direction.
		* Avalible only in manifold surface.
		* It is faster than the VoutHEIterator, so try to use this in manifold case.
		* \param pV the pointer to the vertex
		*/
		class VCcwInHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VCcwInHEIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};
			VCcwInHEIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VCcwInHEIterator& operator++() { 
				_pHE = pHE != vertexMostClwInHalfEdge(_pHE) ? vertexNextClwInHalfEdge(_pHE) : NULL;

				return *this; 
			};
			VCcwInHEIterator  operator++(int) { 
				VCcwInHEIterator tmp(_pV, _Iter); 
				_pHE = pHE != vertexMostClwInHalfEdge(_pHE) ? vertexNextClwInHalfEdge(_pHE) : NULL;
				return tmp; 
			};
			 
			bool operator==(const VCcwInHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwInHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HalfEdgePtr operator*() { return _pHE; }
			HalfEdgePtr value() { return _pHE; }

			VCcwOutHEIterator begin() { return VCcwOutHEIterator(_pV); }
			VCcwOutHEIterator end() { return  VCcwOutHEIterator(_pV, NULL); }//why null???may be a he ?

			HalfEdgePtr get() { return _pHE; }//get what??
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};

		class VClwOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VClwOutHEIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};
			VClwOutHEIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwOutHEIterator& operator++() {
				_pHE = pHE != vertexMostCcwOutHalfEdge(_pHE) ? vertexNextCcwOutHalfEdge(_pHE) : NULL;
				return *this;
			};
			VClwOutHEIterator  operator++(int) {
				VClwOutHEIterator tmp(pV, pHE);
				_pHE = pHE != vertexMostCcwOutHalfEdge(_pHE) ? vertexNextCcwOutHalfEdge(_pHE) : NULL;
				return tmp;
			};

			bool operator==(const VClwOutHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwOutHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HalfEdgePtr operator*() { return _pHE; }
			HalfEdgePtr value() { return _pHE; }

			VClwOutHEIterator begOut() { return VClwOutHEIterator(_pV); }
			VClwOutHEIterator end() { return  VClwOutHEIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};

		class VCcwFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VCcwFIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};
			VCcwFIterator(const VertexPtr& pV, const HalEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VCcwFIterator& operator++() {
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwInHalfEdge(_pHE) : NULL;
				return *this;
			};
			VCcwFIterator  operator++(int) {
				VCcwFIterator tmp(pV, pHE);
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwInHalfEdge(_pHE) : NULL;
				return tmp;
			};

			bool operator==(const VCcwFIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwFIterator& otherIter) { return _pHE != otherIter._pHE; }
			FacePtr operator*() { return halfedgeFace(_pHE); }
			FacePtr value() { return halfedgeFace(_pHE); }

			VCcwFIterator begin() { return VCcwFIterator(_pV); }
			VCcwFIterator end() { return  VCcwFIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		}

		class VClwFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VClwFIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwInHalfEdge(pV)) {};
			VClwFIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwFIterator& operator++() {
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwInHalfEdge(_pHE) : NULL;
				return *this;
			};
			VClwFIterator  operator++(int) {
				VClwFIterator tmp(pV, pHE);
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwOutHalfEdge(_pHE) : NULL;
				return tmp;
			};

			bool operator==(const VClwFIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwFIterator& otherIter) { return _pHE != otherIter._pHE; }
			FacePtr operator*() { return halfedgeFace(_pHE); }
			FacePtr value() { return halfedgeFace(_pHE); }

			VClwFIterator begin() { return VClwFIterator(_pV); }
			VClwFIterator end() { return  VClwFIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		}

		class VCcwEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr> {
		public:
			VCcwEIterator(const VertexPtr& pV) : _pV(pV) {};
			VCcwEIterator(const VertexPtr& pV, const HalEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VCcwEIterator& operator++() {
				if (_pHE != vertexMostCcwInHalfEdge(_pHE))
					_pHE = vertexNextCcwInHalfEdge(_pHE);
				else {
					_pHE = (HalfEdgePtr)_pHE->he_sym()->he_next();
				}

				return *this;
			};
			VCcwEIterator  operator++(int) {
				VCcwEIterator tmp(pV, pHE);
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwOutHalfEdge(_pHE) : NULL;
				return tmp;
			};

			bool operator==(const VCcwEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EdgePtr operator*() { return halfedgeEdge(_pHE); }
			EdgePtr value() { return halfedgeEdge(_pHE); }

			VCcwEIterator begin() { return VCcwEIterator(_pV); }
			VCcwEIterator end() { return  VCcwEIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		}

		class VClwFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VClwFIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwInHalfEdge(pV)) {};
			VClwFIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwFIterator& operator++() {
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwInHalfEdge(_pHE) : NULL;
				return *this;
			};
			VClwFIterator  operator++(int) {
				VClwFIterator tmp(pV, pHE);
				_pHE = pHE != vertexMostCcwInHalfEdge(_pHE) ? vertexNextCcwOutHalfEdge(_pHE) : NULL;
				return tmp;
			};

			bool operator==(const VClwFIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwFIterator& otherIter) { return _pHE != otherIter._pHE; }
			FacePtr operator*() { return halfedgeFace(_pHE); }
			FacePtr value() { return halfedgeFace(_pHE); }

			VClwFIterator begin() { return VClwFIterator(_pV); }
			VClwFIterator end() { return  VClwFIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		}

		class VInHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VInHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VInHEIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};

			VInHEIterator& operator++() { ++_iter; return *this; };
			VInHEIterator  operator++(int) { VInHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VInHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VInHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return (HalfEdgePtr)(*_iter)->he_prev(); }//only diff
			HalfEdgePtr& value() { return  (HalfEdgePtr)(*_iter)->he_prev(); }//must we add this fuc?

			VInHEIterator begin() { return VInHEIterator(_pV); }
			VInHEIterator end() { return VInHEIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		class VVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			VVIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VVIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};

			VVIterator& operator++() { ++_iter; return *this; };
			VVIterator  operator++(int) { VVIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VVIterator& otherIter) { return _iter != otherIter._iter; }
			VertexPtr& operator*() { return (VertexPtr)(*_iter)->vertex(); }
			VertexPtr& value() { return (VertexPtr)(*_iter)->vertex(); }

			VVIterator begin() { return VVIterator(_pV); }
			VVIterator end() { return VVIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		class VEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr>
		{
		public:
			VEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VEIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};

			VEIterator& operator++() { ++_iter; return *this; };
			VEIterator  operator++(int) { VEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VEIterator& otherIter) { return _iter != otherIter._iter; }
		
			EdgePtr& operator*() { return halfedgeEdge(*_iter); }
			EdgePtr& value() { return halfedgeEdge(*_iter); }

			VOutHEIterator begin() { return VOutHEIterator(_pV); }
			VOutHEIterator end() { return VOutHEIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;

		};

		class VFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VFIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VFIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};

			VFIterator& operator++() { ++_iter; return *this; };
			VFIterator  operator++(int) { VFIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VFIterator& otherIter) { return _iter != otherIter._iter; }
			FacePtr& operator*() { return (FacePtr)*_iter->face(); }
			FacePtr& value() { return (FacePtr)*_iter->face(); }

			VFIterator begin() { return VFIterator(_pV);  }
			VFIterator end() { return VFIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		// bad orgnized
		class FHIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {// here inherited?
		public:
			FHIterator(FacePtr pF) : _pF(pF), _iter((HalfEdgePtr)pF->halfedge()) {};//here type cast
			FHIterator(FacePtr pF, HalfEdgePtr iter) : _pF(pF), _iter(iter) {};

			FHIterator& operator++() 
			{
				_iter= (HalfEdgePtr)_iter->he_next();
				if (_iter == (HalfEdgePtr)_pF->halfedge()) _iter = NULL;
				return *this;
			};
			FHIterator  operator++(int) { FHIterator tmp(_pF, _Iter); ++_iter; return tmp; };

			bool operator==(const FHIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const FHIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return _iter; }
			HalfEdgePtr& value() { return _iter; }

			FHIterator begin() { return FHIterator(_pF); }
			FHIterator end() { return FHIterator(_pF,NULL); }
			//FHIterator end() { return (HalfEdgePtr)pF->halfedge()->he_prev(); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			HalfEdgePtr _iter;
			FacePtr _pF;
		};

		//class FEIterator
		//class FVItertor


		//we need pmesh
		class MVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			MVIterator(MeshPtr pM) : _pM(pM), _iter(pM->vertices().begin()) {};
			MVIterator(MeshPtr pM, std::list<VertexPtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MVIterator& operator++() { ++_iter; return *this; };
			MVIterator  operator++(int) { MVIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MVIterator& otherIter) { return _iter != otherIter._iter; }
			VertexPtr& operator*() { return *_iter; }
			VertexPtr& value() { return *_iter; }

			MVIterator begin() { return MVIterator(_pM); }
			MVIterator end() { return MVIterator(_pM, _pM->vertices().end()); }

			std::list<VertexPtr>::iterator get() { return _iter; }
		private:
			std::list<VertexPtr>::iterator _iter;
			MeshPtr _pM;
		};

		class MFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			MFIterator(MeshPtr pM) : _pM(pM), _iter(pM->faces().begin()) {};
			MFIterator(MeshPtr pM, std::list<FacePtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MFIterator& operator++() { ++_iter; return *this; };
			MFIterator  operator++(int) { MFIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MFIterator& otherIter) { return _iter != otherIter._iter; }
			FacePtr& operator*() { return *_iter; }
			FacePtr& value() { return *_iter; }

			MFIterator begin() { return MFIterator(_pM); }
			MFIterator end() { return MFIterator(_pM, _pM->faces().end()); }

			std::list<FacePtr>::iterator get() { return _iter; }
		private:
			std::list<FacePtr>::iterator _iter;
			MeshPtr _pM;
		};

		class MEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr> {
		public:
			MEIterator(MeshPtr pM) : _pM(pM), _iter(pM->edges().begin()) {};
			MEIterator(MeshPtr pM, std::list<EdgePtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MEIterator& operator++() { ++_iter; return *this; };
			MEIterator  operator++(int) { MEIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MEIterator& otherIter) { return _iter != otherIter._iter; }
			EdgePtr& operator*() { return *_iter; }
			EdgePtr& value() { return *_iter; }

			MEIterator begin() { return MEIterator(_pM); }
			MEIterator end() { return MEIterator(_pM, _pM->edges().end()); }

			std::list<EdgePtr>::iterator get() { return _iter; }
		private:
			std::list<EdgePtr>::iterator _iter;
			MeshPtr _pM;
		};

		//class MHEIterator

	};
}