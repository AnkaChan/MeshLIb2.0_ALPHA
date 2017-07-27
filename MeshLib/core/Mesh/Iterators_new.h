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
	class CIterators : private CInterface<VertexType, EdgeType, FaceType, HalfEdgeType>{//private??
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
			HalfEdgePtr& operator*() { return (HalfEdgePtr)*_iter; }
			HalfEdgePtr& value() { return (HalfEdgePtr)*_iter; }

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
				if (_pHE == vertexMostCcwOutHalfEdge(_pV))
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

		class VInHEIterator : public std::iterator<std::forward_iterator_tag, HalfEdgePtr> {
		public:
			VInHEIterator(VertexPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VInHEIterator(VertexPtr pV, VertexType::CHalfEdgePtrList::iterator iter) : _pV(pV), _iter(iter) {};

			VInHEIterator& operator++() { ++_iter; return *this; };
			VInHEIterator  operator++(int) { VInHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VInHEIterator& otherIter) { return _iter == otherIter._iter; }//bug?????
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

		class VVIterator : public std::iterator<std::forward_iterator_tag, VertexPtr> {//vertexptr ? 
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

		class VFIterator : public std::iterator<std::forward_iterator_tag, FacePtr> {//?
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

		// bad orgnized
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