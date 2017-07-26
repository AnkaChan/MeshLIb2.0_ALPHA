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

		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VOutHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VOutHEIterator(VertexPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return *_iter; }
			HalfEdgePtr& value() { return *_iter; }

			VOutHEIterator begin() { return VOutHEIterator(_pV);  }
			VOutHEIterator end() { return VOutHEIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			CHalfEdgePtrListIterator _iter;
			VertexPtr _pV;
		};

		class VCcwOutHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VCcwOutHEIterator(VertexPtr pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};
			VCcwOutHEIterator(VertexPtr pV, HalfEdgePtr pHE) : _pV(pV), _pHE(pHE) {};

			VCcwOutHEIterator& operator++() { 
				_pHE = vertexNextCcwOutHalfEdge(_pHE); 
				if (_pHE == vertexMostClwOutHalfEdge(_pV))
					_pHE = NULL;
				return *this; 
			};
			VCcwOutHEIterator  operator++(int) { 
				VCcwOutHEIterator tmp(_pV, _Iter); 
				_pHE = vertexNextCcwOutHalfEdge(_pHE); 
				if (_pHE == vertexMostClwOutHalfEdge(_pV))
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

		class VEIterator {
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
	};
}