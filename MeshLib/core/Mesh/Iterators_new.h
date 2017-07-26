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
	class Iterators {
	public:
		typedef CInterface<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType> Interface;
		using CInterface::VertexPtr;
		using CInterface::EdgePtr;
		using CInterface::HalfEdgePtr;
		using CInterface::FacePtr;

		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VOutHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};//arhe 存的是基类的指针//用list.begin赋值 iter
			VOutHEIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return *_iter; }
			HalfEdgePtr& value() { return *_iter; }

			VOutHEIterator begin() { return VOutHEIterator(_pV);  }
			VOutHEIterator end() { return VOutHEIterator(_pV, _pV->arhe().end()); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			VertexType::CHalfEdgePtrList::iterator _iter;
			VertexPtr _pV;
		};

		class VCcwOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VCcwOutHEIterator(VertexPtr pV) : _pV(pV), _pHE(Interface::vertexMostClwOutHalfEdge(pV)) {};
			VCcwOutHEIterator(VertexPtr pV, HalfEdgePtr pHE) : _pV(pV), _pHE(pHE) {};

			VCcwOutHEIterator& operator++() { 
				_pHE = Interface::vertexNextCcwOutHalfEdge(_pHE); 
				if (_pHE == Interface::vertexMostClwOutHalfEdge(_pV))
					_pHE = NULL;
				return *this; 
			};
			VCcwOutHEIterator  operator++(int) { 
				VCcwOutHEIterator tmp(_pV, _Iter); 
				_pHE = Interface::vertexNextCcwOutHalfEdge(_pHE); 
				if (_pHE == Interface::vertexMostClwOutHalfEdge(_pV))
					_pHE = NULL;
				return tmp; 
			};

			bool operator==(const VCcwOutHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwOutHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HalfEdgePtr operator*() { return _pHE; }
			HalfEdgePtr value() { return _pHE; }

			VCcwOutHEIterator begin() { return VCcwOutHEIterator(_pV); }
			VCcwOutHEIterator end() { return  VCcwOutHEIterator(_pV, NULL); }

			HalfEdgePtr get() { return _pHE; }
		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};

		class VInHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VInHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VInHEIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VInHEIterator& operator++() { ++_iter; return *this; };
			VInHEIterator  operator++(int) { VInHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VInHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VInHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return (HalfEdgePtr)(*_iter)->he_prev(); }//only diff
			HalfEdgePtr& value() { return  (HalfEdgePtr)(*_iter)->he_prev(); }//msut we add this fuc?

			VInHEIterator begin() { return VInHEIterator(_pV); }
			VInHEIterator end() { return VInHEIterator(_pV, _pV->arhe().end()); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			VertexType::CHalfEdgePtrList::iterator _iter;
			VertexPtr _pV;
		};

		class VVIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VVIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VVIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VVIterator& operator++() { ++_iter; return *this; };
			VVIterator  operator++(int) { VVIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VVIterator& otherIter) { return _iter != otherIter._iter; }
			VertexPtr& operator*() { return (VertexPtr)(*_iter)->vertex(); }
			VertexPtr& value() { return (VertexPtr)(*_iter)->vertex(); }

			VVIterator begin() { return VVIterator(_pV); }
			VVIterator end() { return VVIterator(_pV, _pV->arhe().end()); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			VertexType::CHalfEdgePtrList::iterator _iter;
			VertexPtr _pV;
		};

		class VEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VEIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VEIterator& operator++() { ++_iter; return *this; };
			VEIterator  operator++(int) { VEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VEIterator& otherIter) { return _iter != otherIter._iter; }
			EdgePtr& operator*() { return (EdgePtr)*_iter->edge(); }
			EdgePtr& value() { return (EdgePtr)*_iter->edge(); }

			VEIterator begin() { return VEIterator(_pV); }
			VEIterator end() { return VEIterator(_pV, _pV->arhe().end()); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			VertexType::CHalfEdgePtrList::iterator _iter;
			VertexPtr _pV;
		};

		class VFIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VFIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VFIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VFIterator& operator++() { ++_iter; return *this; };
			VFIterator  operator++(int) { VFIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VFIterator& otherIter) { return _iter != otherIter._iter; }
			FacePtr& operator*() { return (FacePtr)*_iter->face(); }
			FacePtr& value() { return (FacePtr)*_iter->face(); }

			VFIterator begin() { return VFIterator(_pV);  }
			VFIterator end() { return VFIterator(_pV, _pV->arhe().end()); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			VertexType::CHalfEdgePtrList::iterator _iter;
			VertexPtr _pV;
		};

		// bad orgnize
		class FHIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {// here inherited?
		public:
			FHIterator(FacePtr pF) : _pF(pF), _iter((HalfEdgePtr)pF->halfedge()) {};//here type cast
			FHIterator(FacePtr pF, HalfEdgePtr iter) : _pF(pF), _iter(iter) {};

			FHIterator& operator++() 
			{
				assert(_iter != NULL);
				_iter= (HalfEdgePtr)_iter->he_next(); return *this; 
				if (_iter == (HalfEdgePtr)_pF->halfedge()) { _iter = NULL; return; }
			};
			FHIterator  operator++(int) { FHIterator tmp(_pF, _Iter); ++_iter; return tmp; };

			bool operator==(const FHIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const FHIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return _iter; }
			HalfEdgePtr& value() { return _iter; }

			FHIterator begin() { return (HalfEdgePtr)pF->halfedge(); }
			FHIterator end() { return (HalfEdgePtr)pF->halfedge()->he_prev(); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			HalfEdgePtr _iter;
			FacePtr _pF;
		};

		//class FEIterator
		//class FVItertor


	};
}