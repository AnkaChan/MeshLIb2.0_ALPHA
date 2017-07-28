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
	struct CIterators : private CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>{
	public:
		typedef std::list<CHalfEdge*> CHalfEdgePtrList;
		typedef std::list<CHalfEdge*>::iterator CHalfEdgePtrListIterator;

		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VOutHEIterator(const VertexPtr& pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VOutHEIterator(const VertexPtr& pV, const CHalfEdgePtrListIterator& iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr operator*() { return (HalfEdgePtr)(*_iter); }
			HalfEdgePtr value() { return (HalfEdgePtr)(*_iter); }

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
		class VCcwOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VCcwOutHEIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};
			VCcwOutHEIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VCcwOutHEIterator& operator++() { 
				_pHE = _pHE == vertexMostCcwOutHalfEdge(_pV) ? NULL : vertexNextCcwOutHalfEdge(_pHE);
				return *this; 
			};
			VCcwOutHEIterator  operator++(int) { 
				VCcwOutHEIterator tmp(_pV, _Iter); 
				_pHE = _pHE == vertexMostCcwOutHalfEdge(_pV) ? NULL : vertexNextCcwOutHalfEdge(_pHE);
				return tmp; 
			};
			 
			bool operator==(const VCcwOutHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwOutHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HalfEdgePtr operator*() { return _pHE; }
			HalfEdgePtr value() { return _pHE; }

			VCcwOutHEIterator begin() { return VCcwOutHEIterator(_pV); }
			VCcwOutHEIterator end() { return  VCcwOutHEIterator(_pV, NULL); }//why null???may be a he ?

			HalfEdgePtr get() { return _pHE; }//get what??
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};

		/* 
		* Iterator to access the out halfedge of a vertex in clockwise direction.
		* Avalible only in manifold surface.
		* It is faster than the VoutHEIterator, so try to use this in manifold case.
		* \param pV the pointer to the vertex
		*/
		class VClwInHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VClwInHEIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};
			VClwInHEIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwInHEIterator& operator++() {
				_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				return *this;
			};
			VClwInHEIterator  operator++(int) {
				VClwInHEIterator tmp(pV, _pHE);
				_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				return tmp;
			};

			bool operator==(const VClwInHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwInHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HalfEdgePtr operator*() { return _pHE; }
			HalfEdgePtr value() { return _pHE; }

			VClwInHEIterator begin() { return VClwInHEIterator(_pV); }
			VClwInHEIterator end() { return  VClwInHEIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};

		class VCcwFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VCcwFIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};
			VCcwFIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VCcwFIterator& operator++() {
				_pHE = _pHE == vertexMostCcwOutHalfEdge(_pV) ? NULL : vertexNextCcwOutHalfEdge(_pHE);
				return *this;
			};
			VCcwFIterator  operator++(int) {
				VCcwFIterator tmp(_pV, _Iter);
				_pHE = _pHE == vertexMostCcwOutHalfEdge(_pV) ? NULL : vertexNextCcwOutHalfEdge(_pHE);
				return tmp;
			};

			bool operator==(const VCcwFIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwFIterator& otherIter) { return _pHE != otherIter._pHE; }
			FacePtr operator*() { return halfedgeFace(_pHE); }
			FacePtr value() { return halfedgeFace(_pHE); }

			VCcwFIterator begin() { return VCcwFIterator(_pV); }
			VCcwFIterator end() { return  v(_pV, NULL); }//why null???may be a he ?

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};

		class VClwFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VClwFIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};
			VClwFIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};

			VClwFIterator& operator++() {
				_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				return *this;
			};
			VClwFIterator  operator++(int) {
				VClwFIterator tmp(pV, _pHE);
				_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
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
		};

		class VCcwEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr> {
		public:
			VCcwEIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};

			VCcwEIterator& operator++() {
				if (isboundary(_pV)) {
					if (_pHE == vertexMostCcwInHalfEdge(_pV)){
						_pHE = (HalfEdgePtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					} else{
						_pHE = vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostCcwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return *this;
			};
			VCcwEIterator  operator++(int) {
				VCcwEIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == vertexMostCcwInHalfEdge(_pHE)) {
						_pHE = (HalfEdgePtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostCcwInHalfEdge(_pHE) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
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
			VCcwEIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};
			VertexPtr _pV;
			HalfEdgePtr _pHE;
			bool reachBoundary = false;
		};
	
		class VClwEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr> {
		public:
			VClwEIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostCcwOutHalfEdge(pV)) {};

			VClwEIterator& operator++() {
				if (isboundary(_pV)) {
					if (_pHE == vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HalfEdgePtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextClwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return *this;
			};
			VClwEIterator  operator++(int) {
				VClwEIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HalfEdgePtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};

			bool operator==(const VClwEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EdgePtr operator*() { return halfedgeEdge(_pHE); }
			EdgePtr value() { return halfedgeEdge(_pHE); }

			VClwEIterator begin() { return VClwEIterator(_pV); }
			VClwEIterator end() { return  VClwEIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VClwEIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};
			VertexPtr _pV;
			HalfEdgePtr _pHE;
			bool reachBoundary = false;
		};
		
		class VVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			VVIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};

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
			VVIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		class VCcwVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			VCcwVIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};

			VCcwVIterator& operator++() {
				if (isboundary(_pV)) {
					if (_pHE == vertexMostCcwInHalfEdge(_pHE)) {
						_pHE = (HalfEdgePtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostCcwInHalfEdge(_pHE) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return *this;
			};
			VCcwVIterator  operator++(int) {
				VCcwVIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == vertexMostCcwInHalfEdge(_pHE)) {
						_pHE = (HalfEdgePtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostCcwInHalfEdge(_pHE) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};
			bool operator==(const VCcwVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwVIterator& otherIter) { return _pHE != otherIter._pHE; }
			EdgePtr operator*() { 
				if (reachBoundary)
					return halfedgeSource(_pHE);
				else
					return halfedgeTarget(_pHE);
			}
			EdgePtr value() {
				if (reachBoundary)
					return halfedgeSource(_pHE);
				else
					return halfedgeTarget(_pHE); 
			}

			VCcwVIterator begin() { return VCcwVIterator(_pV); }
			VCcwVIterator end() { return  VCcwVIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VCcwVIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};
			VertexPtr _pV;
			HalfEdgePtr _pHE;
			bool reachBoundary = false;
		};

		class VClwVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			VClwVIterator(const VertexPtr& pV) : _pV(pV), _pHE(vertexMostCcwOutHalfEdge(pV)) {};

			VClwVIterator& operator++() {
				if (isboundary(_pV)) {
					if (_pHE == vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HalfEdgePtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextClwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return *this;
			};
			VClwVIterator  operator++(int) {
				VClwVIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HalfEdgePtr)_pHE->he_next();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwInHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostClwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};

			bool operator==(const VClwVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VClwVIterator& otherIter) { return _pHE != otherIter._pHE; }
			VertexPtr operator*() { 
				if (reachBoundary)
					return halfedgeTarget(_pHE);
				else
					return halfedgeSource(_pHE);
			}
			VertexPtr value() { 
				if (reachBoundary)
					return halfedgeTarget(_pHE);
				else
					return halfedgeSource(_pHE); 
			}

			VClwVIterator begin() { return VClwVIterator(_pV); }
			VClwVIterator end() { return  VClwVIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VClwVIterator(const VertexPtr& pV, const HalfEdgePtr& pHE) : _pV(pV), _pHE(pHE) {};
			VertexPtr _pV;
			HalfEdgePtr _pHE;
			bool reachBoundary = false;
		};

		class VEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr>
		{
		public:
			VEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};

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
			VEIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;

		};

		class VFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			VFIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};

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
			VFIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		// bad orgnized
		class FHIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {// here inherited?
		public:
			FHIterator(FacePtr pF) : _pF(pF), _iter((HalfEdgePtr)pF->halfedge()) {};//here type cast

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
			FHIterator end() { return FHIterator(_pF, NULL); }
			//FHIterator end() { return FHIterator(_pF, (HalfEdgePtr)pF->halfedge()->he_prev())); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			FHIterator(FacePtr pF, HalfEdgePtr iter) : _pF(pF), _iter(iter) {};
			HalfEdgePtr _iter;
			FacePtr _pF;
		};

		//class FEIterator
		class FEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr> {
		public:
			FEIterator(FacePtr pF) : _pF(pF), _iter((HalfEdgePtr)pF->halfedge()) {};
			FEIterator(FacePtr pF, HalfEdgePtr iter) : _pF(pF), _iter(iter) {};

			FEIterator& operator++()
			{
				_iter = (HalfEdgePtr)_iter->he_next();
				if (_iter == (HalfEdgePtr)_pF->halfedge()) _iter = NULL;
				return *this;
			};
			FEIterator  operator++(int) { FEIterator tmp(_pF, _Iter); ++_iter; return tmp; };

			bool operator==(const FEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const FEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return (EdgePtr)_iter->edge(); }
			HalfEdgePtr& value() { return (EdgePtr)_iter->edge(); }

			FEIterator begin() { return FEIterator(_pF); }
			FEIterator end() { return FEIterator(_pF, NULL); }
			//FEIterator end() { return FEIterator(_pF, (HalfEdgePtr)pF->halfedge()->he_prev()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			HalfEdgePtr _iter;
			EdgePtr _pF;
		};

		//class FVItertor
		class FVItertor : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			FVItertor(FacePtr pF) : _pF(pF), _iter((HalfEdgePtr)pF->halfedge()) {};
			FVItertor(FacePtr pF, HalfEdgePtr iter) : _pF(pF), _iter(iter) {};

			FVItertor& operator++()
			{
				_iter = (HalfEdgePtr)_iter->he_next();
				if (_iter == (HalfEdgePtr)_pF->halfedge()) _iter = NULL;
				return *this;
			};
			FVItertor  operator++(int) { FVItertor tmp(_pF, _Iter); ++_iter; return tmp; };

			bool operator==(const FVItertor& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const FVItertor& otherIter) { return _iter != otherIter._iter; }
			VertexPtr& operator*() { return (VertexPtr)_iter->vertex(); }
			VertexPtr& value() { return (VertexPtr)_iter->vertex(); }

			FVItertor begin() { return FVItertor(_pF); }
			FVItertor end() { return FVItertor(_pF, NULL); }
			//FVItertor end() { return FVItertor(_pF, (HalfEdgePtr)pF->halfedge()->he_prev()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			HalfEdgePtr _iter;
			EdgePtr _pF;
		};


		//we need pmesh
		class MVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			MVIterator(MeshPtr pM) : _pM(pM), _iter(pM->vertices().begin()) {};
			MVIterator(MeshPtr pM, typename std::list<VertexPtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MVIterator& operator++() { ++_iter; return *this; };
			MVIterator  operator++(int) { MVIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MVIterator& otherIter) { return _iter != otherIter._iter; }
			VertexPtr& operator*() { return *_iter; }
			VertexPtr& value() { return *_iter; }

			MVIterator begin() { return MVIterator(_pM); }
			MVIterator end() { return MVIterator(_pM, _pM->vertices().end()); }

			typename std::list<VertexPtr>::iterator get() { return _iter; }
		private:
			typename std::list<VertexPtr>::iterator _iter;
			MeshPtr _pM;
		};

		class MFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {
		public:
			MFIterator(MeshPtr pM) : _pM(pM), _iter(pM->faces().begin()) {};
			MFIterator(MeshPtr pM, typename std::list<FacePtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MFIterator& operator++() { ++_iter; return *this; };
			MFIterator  operator++(int) { MFIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MFIterator& otherIter) { return _iter != otherIter._iter; }
			FacePtr& operator*() { return *_iter; }
			FacePtr& value() { return *_iter; }

			MFIterator begin() { return MFIterator(_pM); }
			MFIterator end() { return MFIterator(_pM, _pM->faces().end()); }

			typename std::list<FacePtr>::iterator get() { return _iter; }
		private:
			typename std::list<FacePtr>::iterator _iter;
			MeshPtr _pM;
		};

		class MEIterator : public std::iterator<std::forward_iterator_tag, EdgePtr> {
		public:
			MEIterator(MeshPtr pM) : _pM(pM), _iter(pM->edges().begin()) {};
			MEIterator(MeshPtr pM, typename std::list<EdgePtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MEIterator& operator++() { ++_iter; return *this; };
			MEIterator  operator++(int) { MEIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MEIterator& otherIter) { return _iter != otherIter._iter; }
			EdgePtr& operator*() { return *_iter; }
			EdgePtr& value() { return *_iter; }

			MEIterator begin() { return MEIterator(_pM); }
			MEIterator end() { return MEIterator(_pM, _pM->edges().end()); }

			typename std::list<EdgePtr>::iterator get() { return _iter; }
		private:
			typename std::list<EdgePtr>::iterator _iter;
			MeshPtr _pM;
		};

		//class MHEIterator

	};

	template<typename Interface>
	struct CIteratorsI : public CIterators<typename Interface::VertexType, typename Interface::EdgeType, typename Interface::FaceType, typename Interface::HalfEdgeType>
	{};
}
