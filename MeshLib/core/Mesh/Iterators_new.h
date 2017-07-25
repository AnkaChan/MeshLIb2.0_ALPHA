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
			VOutHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
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
	};
}