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

		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {
		public:
			VOutHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VOutHEIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HalfEdgePtr& operator*() { return *_iter; }
			HalfEdgePtr& value() { return *_iter; }

			VertexType::CHalfEdgePtrList::iterator begin() { return _pV->arhe().begin();  }
			VertexType::CHalfEdgePtrList::iterator end() { return _pV->arhe().end(); }

			VertexType::CHalfEdgePtrList::iterator get() { return _iter; }
		private:
			VertexType::CHalfEdgePtrList::iterator _iter;
			VertexPtr _pV;
		};

		class VCcwHEIterator {
		public:
			VCcwHEIterator
			VCcwHEIterator

		private:
			VertexPtr _pV;
			HalfEdgePtr _pHE;
		};
	};
}