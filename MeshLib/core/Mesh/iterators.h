/*!
*      \file Iterators.h
*      \brief Iterators for accessing geometric objects on a mesh
*	   \author David Gu
*      \date 10/07/2010
*
*/
 
#ifndef  _ITERATORS_H_
#define  _ITERATORS_H_
#include <iterator>
#include <algorithm>
#include <list>
#include <vector>

#include "Interface.h"
#include "HalfEdge.h"
#include "BaseMesh.h"

namespace MeshLib{

//v->out halfedge
/*!
	\brief VertexOutHalfedgeIterator, transverse all the outgoing halfedges of a vertex ccwly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexOutHalfedgeIterator
{
public:

	template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
	struct CIteratorCore : private CInterface<VertexType, EdgeType, FaceType, HalfEdgeType> {
	public:
		typedef std::list<CHalfEdge*> CHalfEdgePtrList;
		typedef std::list<CHalfEdge*>::iterator CHalfEdgePtrListIterator;

		class VOutHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
		public:
			VOutHEIterator(const VPtr& pV) : _pV(pV), _iter(pV->arhe().begin()) {};
			VOutHEIterator(const VPtr& pV, const CHalfEdgePtrListIterator& iter) : _pV(pV), _iter(iter) {};

			VOutHEIterator& operator++() { ++_iter; return *this; };
			VOutHEIterator  operator++(int) { VOutHEIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VOutHEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VOutHEIterator& otherIter) { return _iter != otherIter._iter; }
			HEPtr operator*() { return (HEPtr)(*_iter); }
			HEPtr value() { return (HEPtr)(*_iter); }

			VOutHEIterator begin() { return VOutHEIterator(_pV); }
			VOutHEIterator end() { return VOutHEIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			CHalfEdgePtrListIterator _iter;
			VPtr _pV;
		};

		/*
		* Iterator to access the out halfedge of a vertex in counter-clockwise direction.
		* Avalible only in manifold surface.
		* It is faster than the VoutHEIterator, so try to use this in manifold case.
		* \param pV the pointer to the vertex
		*/
		class VCcwOutHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
		public:
			VCcwOutHEIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};
			VCcwOutHEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};

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
			HEPtr operator*() { return _pHE; }
			HEPtr value() { return _pHE; }

			VCcwOutHEIterator begin() { return VCcwOutHEIterator(_pV); }
			VCcwOutHEIterator end() { return  VCcwOutHEIterator(_pV, NULL); }//why null???may be a he ?

			HEPtr get() { return _pHE; }//get what??
		private:
			VPtr _pV;
			HEPtr _pHE;
		};

		/*
		* Iterator to access the out halfedge of a vertex in clockwise direction.
		* Avalible only in manifold surface.
		* It is faster than the VoutHEIterator, so try to use this in manifold case.
		* \param pV the pointer to the vertex
		*/
		class VClwInHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
		public:
			VClwInHEIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};
			VClwInHEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};

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
			HEPtr operator*() { return _pHE; }
			HEPtr value() { return _pHE; }

			VClwInHEIterator begin() { return VClwInHEIterator(_pV); }
			VClwInHEIterator end() { return  VClwInHEIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VPtr _pV;
			HEPtr _pHE;
		};

		class VCcwFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			VCcwFIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};

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
			FPtr operator*() { return halfedgeFace(_pHE); }
			FPtr value() { return halfedgeFace(_pHE); }

			VCcwFIterator begin() { return VCcwFIterator(_pV); }
			VCcwFIterator end() { return  VCcwFIterator(_pV, NULL); }//why null???may be a he ?

			HEPtr get() { return _pHE; }
		private:
			VCcwFIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
		};

		class VClwFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			VClwFIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};
			VClwFIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};

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
			FPtr operator*() { return halfedgeFace(_pHE); }
			FPtr value() { return halfedgeFace(_pHE); }

			VClwFIterator begin() { return VClwFIterator(_pV); }
			VClwFIterator end() { return  VClwFIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VPtr _pV;
			HEPtr _pHE;
		};

		class VCcwEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			VCcwEIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};

			VCcwEIterator& operator++() {
				if (isBoundary(_pV)) {
					if (_pHE == vertexMostCcwOutHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwOutHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostCcwOutHalfEdge(_pV) ? _pHE = NULL : vertexNextCcwOutHalfEdge(_pHE);
				}
				return *this;
			};
			VCcwEIterator  operator++(int) {
				VCcwEIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == vertexMostCcwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
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
					_pHE = _pHE == vertexMostCcwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};

			bool operator==(const VCcwEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EPtr operator*() { return halfedgeEdge(_pHE); }
			EPtr value() { return halfedgeEdge(_pHE); }

			VCcwEIterator begin() { return VCcwEIterator(_pV); }
			VCcwEIterator end() { return  VCcwEIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VCcwEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VClwEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			VClwEIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};

			VClwEIterator& operator++() {
				if (isBoundary(_pV)) {
					if (_pHE == vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_next();
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
						_pHE = (HEPtr)_pHE->he_next();
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
			EPtr operator*() { return halfedgeEdge(_pHE); }
			EPtr value() { return halfedgeEdge(_pHE); }

			VClwEIterator begin() { return VClwEIterator(_pV); }
			VClwEIterator end() { return  VClwEIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VClwEIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			VVIterator(VPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};

			VVIterator& operator++() {
				if (isBoundary(_pV)) {
					if (_pLastV != NULL) {
						++_iter;
						_pLastV = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastV = halfedgeSource((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}

				return *this;
			};
			VVIterator  operator++(int) {
				VVIterator tmp(_pV, _Iter); if (isBoundary(_pV)) {
					if (_pLastV != NULL) {
						++_iter;
						_pLastV = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastV = halfedgeSource((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}
				return tmp;
			};

			bool operator==(const VVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VVIterator& otherIter) { return _iter != otherIter._iter; }
			VPtr operator*() {
				if (_pLastV == NULL)
					return (VPtr)(*_iter)->target();
				else
					return _pLastV;
			}
			VPtr value() {
				if (_pLastV == NULL)
					return (VPtr)(*_iter)->target();
				else
					return _pLastV;
			}

			VVIterator begin() { return VVIterator(_pV); }
			VVIterator end() { return VVIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			VVIterator(VPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};
			CHalfEdgePtrListIterator _iter;
			VPtr _pV;
			VPtr _pLastV = NULL;
		};

		class VCcwVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			VCcwVIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostClwOutHalfEdge(pV)) {};

			VCcwVIterator& operator++() {
				if (isBoundary(_pV)) {
					if (_pHE == vertexMostCcwOutHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
						reachBoundary = true;
					}
					else if (reachBoundary) {
						_pHE = NULL;
					}
					else {
						_pHE = vertexNextCcwOutHalfEdge(_pHE);
					}
				}
				else {
					_pHE = _pHE == vertexMostCcwOutHalfEdge(_pV) ? _pHE = NULL : vertexNextCcwOutHalfEdge(_pHE);
				}
				return *this;
			};
			VCcwVIterator  operator++(int) {
				VCcwVIterator tmp(pV, pHE);
				if (isboundary(_pV)) {
					if (_pHE == vertexMostCcwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_prev();
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
					_pHE = _pHE == vertexMostCcwInHalfEdge(_pV) ? _pHE = NULL : vertexNextClwInHalfEdge(_pHE);
				}
				return tmp;
			};
			bool operator==(const VCcwVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const VCcwVIterator& otherIter) { return _pHE != otherIter._pHE; }
			VPtr operator*() {
				if (reachBoundary)
					return halfedgeSource(_pHE);
				else
					return halfedgeTarget(_pHE);
			}
			VPtr value() {
				if (reachBoundary)
					return halfedgeSource(_pHE);
				else
					return halfedgeTarget(_pHE);
			}

			VCcwVIterator begin() { return VCcwVIterator(_pV); }
			VCcwVIterator end() { return  VCcwVIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VCcwVIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VClwVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			VClwVIterator(const VPtr& pV) : _pV(pV), _pHE(vertexMostCcwInHalfEdge(pV)) {};

			VClwVIterator& operator++() {
				if (isBoundary(_pV)) {
					if (_pHE == vertexMostClwInHalfEdge(_pV)) {
						_pHE = (HEPtr)_pHE->he_next();
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
						_pHE = (HEPtr)_pHE->he_next();
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
			VPtr operator*() {
				if (reachBoundary)
					return halfedgeTarget(_pHE);
				else
					return halfedgeSource(_pHE);
			}
			VPtr value() {
				if (reachBoundary)
					return halfedgeTarget(_pHE);
				else
					return halfedgeSource(_pHE);
			}

			VClwVIterator begin() { return VClwVIterator(_pV); }
			VClwVIterator end() { return  VClwVIterator(_pV, NULL); }

			HEPtr get() { return _pHE; }
		private:
			VClwVIterator(const VPtr& pV, const HEPtr& pHE) : _pV(pV), _pHE(pHE) {};
			VPtr _pV;
			HEPtr _pHE;
			bool reachBoundary = false;
		};

		class VEIterator : public std::iterator<std::forward_iterator_tag, EPtr>
		{
		public:
			VEIterator(VPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};

			VEIterator& operator++() {
				if (isBoundary(_pV)) {
					if (_pLastE != NULL) {
						++_iter;
						_pLastE = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastE = halfedgeEdge((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}

				return *this;
			};
			VEIterator  operator++(int) {
				VEIterator tmp(_pV, _Iter); if (isBoundary(_pV)) {
					if (_pLastE != NULL) {
						++_iter;
						_pLastE = NULL;
					}
					else {
						CHalfEdge * pLastHE = (*_iter)->he_prev();;
						if (pLastHE->he_sym() != NULL)
							++_iter;
						else {
							_pLastE = halfedgeEdge((HEPtr)pLastHE);
						}
					}
				}
				else {
					++_iter;
				}
				return tmp;
			};

			bool operator==(const VEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VEIterator& otherIter) { return _iter != otherIter._iter; }

			EPtr operator*() {
				if (_pLastE == NULL)
					return  halfedgeEdge((HEPtr)*_iter);
				else
					return _pLastE;
			}
			EPtr value() {
				if (_pLastE == NULL)
					return  halfedgeEdge((HEPtr)*_iter);
				else
					return _pLastE;
			}

			VEIterator begin() { return VEIterator(_pV); }
			VEIterator end() { return VEIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			VEIterator(VPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};
			CHalfEdgePtrListIterator _iter;
			VPtr _pV;
			EPtr _pLastE = NULL;
		};

		class VFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			VFIterator(VPtr pV) : _pV(pV), _iter(pV->arhe().begin()) {};

			VFIterator& operator++() { ++_iter; return *this; };
			VFIterator  operator++(int) { VFIterator tmp(_pV, _Iter); ++_iter; return tmp; };

			bool operator==(const VFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const VFIterator& otherIter) { return _iter != otherIter._iter; }
			FPtr operator*() { return (FPtr)(*_iter)->face(); }
			FPtr value() { return (FPtr)(*_iter)->face(); }

			VFIterator begin() { return VFIterator(_pV); }
			VFIterator end() { return VFIterator(_pV, _pV->arhe().end()); }

			CHalfEdgePtrListIterator get() { return _iter; }
		private:
			VFIterator(VPtr pV, CHalfEdgePtrListIterator iter) : _pV(pV), _iter(iter) {};
			CHalfEdgePtrListIterator _iter;
			VPtr _pV;
		};

		// bad orgnized
		class FHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {// here inherited?
		public:
			FHEIterator(FPtr pF) : _pF(pF), _pHE((HEPtr)pF->halfedge()) {};//here type cast

			FHEIterator& operator++()
			{
				_pHE = (HEPtr)_pHE->he_next();
				if (_pHE == (HEPtr)_pF->halfedge()) _pHE = NULL;
				return *this;
			};
			FHEIterator  operator++(int) { FHEIterator tmp(_pF, _pHE); ++_pHE; return tmp; };

			bool operator==(const FHEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const FHEIterator& otherIter) { return _pHE != otherIter._pHE; }
			HEPtr operator*() { return _pHE; }
			HEPtr value() { return _pHE; }

			FHEIterator begin() { return FHEIterator(_pF); }
			FHEIterator end() { return FHEIterator(_pF, NULL); }
			//FHEIterator end() { return FHIterator(_pF, (HEPtr)pF->halfedge()->he_prev())); }

			CHalfEdgePtrListIterator get() { return _pHE; }
		private:
			FHEIterator(FPtr pF, HEPtr iter) : _pF(pF), _pHE(iter) {};
			HEPtr _pHE;
			FPtr _pF;
		};

		//class FEIterator
		class FEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			FEIterator(FPtr pF) : _pF(pF), _pHE((HEPtr)pF->halfedge()) {};
			FEIterator(FPtr pF, HEPtr iter) : _pF(pF), _pHE(iter) {};

			FEIterator& operator++()
			{
				_pHE = (HEPtr)_pHE->he_next();
				if (_pHE == (HEPtr)_pF->halfedge()) _pHE = NULL;
				return *this;
			};
			FEIterator  operator++(int) { FEIterator tmp(_pF, _pHE); ++_pHE; return tmp; };

			bool operator==(const FEIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const FEIterator& otherIter) { return _pHE != otherIter._pHE; }
			EPtr operator*() { return (EPtr)_pHE->edge(); }
			EPtr value() { return (EPtr)_pHE->edge(); }

			FEIterator begin() { return FEIterator(_pF); }
			FEIterator end() { return FEIterator(_pF, NULL); }
			//FEIterator end() { return FEIterator(_pF, (HEPtr)pF->halfedge()->he_prev()); }

			CHalfEdgePtrListIterator get() { return _pHE; }
		private:
			HEPtr _pHE;
			FPtr _pF;
		};

		//class FVItertor
		class FVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			FVIterator(const FPtr& pF) : _pF(pF), _pHE(faceHalfedge(pF)) {};

			FVIterator& operator++()
			{
				_pHE = (HEPtr)_pHE->he_next();
				if (_pHE == (HEPtr)_pF->halfedge()) _pHE = NULL;
				return *this;
			};
			FVIterator  operator++(int) { FVIterator tmp(_pF, _pHE); ++_pHE; return tmp; };

			bool operator==(const FVIterator& otherIter) { return _pHE == otherIter._pHE; }
			bool operator!=(const FVIterator& otherIter) { return _pHE != otherIter._pHE; }
			VPtr operator*() { return (VPtr)_pHE->vertex(); }
			VPtr value() { return (VPtr)_pHE->vertex(); }

			FVIterator begin() { return FVIterator(_pF); }
			FVIterator end() { return FVIterator(_pF, NULL); }
			HEPtr get() { return _pHE; }
		private:
			FVIterator(const FPtr& pF, const HEPtr& iter) : _pF(pF), _pHE(iter) {};
			HEPtr _pHE;
			FPtr _pF;
		};


		//we need pmesh
		class MVIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
		public:
			MVIterator(MeshPtr pM) : _pM(pM), _iter(pM->vertices().begin()) {};
			MVIterator(MeshPtr pM, typename std::list<VPtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MVIterator& operator++() { ++_iter; return *this; };
			MVIterator  operator++(int) { MVIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MVIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MVIterator& otherIter) { return _iter != otherIter._iter; }
			VPtr& operator*() { return *_iter; }
			VPtr& value() { return *_iter; }

			MVIterator begin() { return MVIterator(_pM); }
			MVIterator end() { return MVIterator(_pM, _pM->vertices().end()); }

			typename std::list<VPtr>::iterator get() { return _iter; }
		private:
			typename std::list<VPtr>::iterator _iter;
			MeshPtr _pM;
		};

		class MFIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
		public:
			MFIterator(MeshPtr pM) : _pM(pM), _iter(pM->faces().begin()) {};
			MFIterator(MeshPtr pM, typename std::list<FPtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MFIterator& operator++() { ++_iter; return *this; };
			MFIterator  operator++(int) { MFIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MFIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MFIterator& otherIter) { return _iter != otherIter._iter; }
			FPtr& operator*() { return *_iter; }
			FPtr& value() { return *_iter; }

			MFIterator begin() { return MFIterator(_pM); }
			MFIterator end() { return MFIterator(_pM, _pM->faces().end()); }

			typename std::list<FPtr>::iterator get() { return _iter; }
		private:
			typename std::list<FPtr>::iterator _iter;
			MeshPtr _pM;
		};

		class MEIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
		public:
			MEIterator(MeshPtr pM) : _pM(pM), _iter(pM->edges().begin()) {};
			MEIterator(MeshPtr pM, typename std::list<EPtr>::iterator iter) : _pM(pM), _iter(iter) {};

			MEIterator& operator++() { ++_iter; return *this; };
			MEIterator  operator++(int) { MEIterator tmp(_pM, _Iter); ++_iter; return tmp; };

			bool operator==(const MEIterator& otherIter) { return _iter == otherIter._iter; }
			bool operator!=(const MEIterator& otherIter) { return _iter != otherIter._iter; }
			EPtr& operator*() { return *_iter; }
			EPtr& value() { return *_iter; }

			MEIterator begin() { return MEIterator(_pM); }
			MEIterator end() { return MEIterator(_pM, _pM->edges().end()); }

			typename std::list<EPtr>::iterator get() { return _iter; }
		private:
			typename std::list<EPtr>::iterator _iter;
			MeshPtr _pM;
		};

		//class MHEIterator

	};

	template<typename Interface>
	struct CIterators : public CIteratorCore<typename Interface::VType, typename Interface::EType, typename Interface::FType, typename Interface::HEType>
	{};


	/*!
	VertexOutHalfedgeIteartor constructor
	\param pMesh pointer to the current mesh
	\param v     pointer to the current vertex
	*/
	VertexOutHalfedgeIterator( CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> *  pMesh, VertexType *  v )
	{ m_pMesh = pMesh; m_vertex = v; m_halfedge = m_pMesh->vertexMostClwOutHalfEdge(v); };

	/*!
	VertexOutHalfedgeIterator destructor
	*/
	~VertexOutHalfedgeIterator(){};
	/*!
	prefix ++ operator, goes to the next ccw vertex out halfedge
	*/
	void operator++() //prefix
	{assert( m_halfedge != NULL ); 
	 if( m_halfedge == m_pMesh->vertexMostCcwOutHalfEdge(m_vertex) ) 
		 m_halfedge = NULL;
	 else
	 	 m_halfedge = m_pMesh->vertexNextCcwOutHalfEdge(m_halfedge); };

	/*!
		postfix ++ operator, goes to the next ccw vertex out halfedge
	*/
	void operator++(int) //postfix
	{assert( m_halfedge != NULL ); 
	 if( m_halfedge == m_pMesh->vertexMostCcwOutHalfEdge(m_vertex) ) 
		 m_halfedge = NULL;
	 else
	 	 m_halfedge = m_pMesh->vertexNextCcwOutHalfEdge(m_halfedge); };

	/*!
		The current halfedge the iterator pointing to.
	*/

	 HalfEdgeType * value() { return m_halfedge; };
	 /*!
		whether all the out halfedges have been visited.
	 */
	 bool end(){ return m_halfedge == NULL; };
	/*!
		The current halfedge the iterator pointing to.
	*/
	 HalfEdgeType * operator*() { return value(); };

private:
	/*!	
		Current mesh.
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> *        m_pMesh;
	/*!
		Current vertex.
	*/
	VertexType *      m_vertex;
	/*!
		Current halfedge.
	*/
	HalfEdgeType * m_halfedge;
};

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VOutHEIterator
{
public:
	VOutHEIterator(VertexType * V) {};
};

//v->in halfedge
/*!
	\brief VertexInHalfedgeIterator, transverse all the incoming halfedges of a vertex ccwly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexInHalfedgeIterator
{
public:
	/*!
	VertexInHalfedgeIteartor constructor
	\param pMesh pointer to the current mesh
	\param v     pointer to the current vertex
	*/
	VertexInHalfedgeIterator(CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> *  pMesh, VertexType * v )
	{ m_pMesh = pMesh; m_vertex = v; m_halfedge = m_pMesh->vertexMostClwInHalfEdge(v); };
	/*!
	VertexInHalfedgeIterator destructor
	*/
	~VertexInHalfedgeIterator(){};

	/*!
	prefix ++ operator, goes to the next ccw vertex in halfedge
	*/
		void operator++()	//prefix
	{
	 assert( m_halfedge != NULL ); 
	 if( m_halfedge == m_pMesh->vertexMostCcwInHalfEdge(m_vertex) ) 
		 m_halfedge = NULL; 
	 else
		m_halfedge = m_pMesh->vertexNextCcwInHalfEdge(m_halfedge); 
	};
	/*!
	postfix ++ operator, goes to the next ccw vertex in halfedge
	*/
	
	void operator++(int)	//postfix
	{
	 assert( m_halfedge != NULL ); 
	 if( m_halfedge == m_pMesh->vertexMostCcwInHalfEdge(m_vertex) ) 
		 m_halfedge = NULL; 
	 else
		m_halfedge = m_pMesh->vertexNextCcwInHalfEdge(m_halfedge); 
	};
	/*!
		The current halfedge the iterator pointing to.
	*/
	
	HalfEdgeType * value() { return m_halfedge; };
	/*!	
		Indicate whether all the in halfedges of the vertex have been transversed.
	*/
	 bool end(){ return m_halfedge == NULL; };
	 /*!
		The current halfedge the iterator pointing to.
	 */
	 HalfEdgeType * operator*() { return value(); };

private:
	/*!
		Current mesh.
	*/
	CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> *        m_pMesh;
	/*!
		Current vertex.
	*/
	VertexType *      m_vertex;
	/*!
		Current halfedge.
	*/
	HalfEdgeType * m_halfedge;
};


//v -> v

/*!
	\brief VertexVertexIterator, transverse all the neighboring vertices of a vertex ccwly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexVertexIterator
{
public:
	/*!
		VertexVertexIterator constructor
		\param v the current vertex
	*/
	VertexVertexIterator( VertexType *  v )
	{ 
		m_vertex = v; 
		m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge();
	};

	/*!
		VertexVertexIterator destructor
	*/
	~VertexVertexIterator(){};

	/*!
		VertexVertexIterator prefix operator ++, goes to the next neighboring vertex CCWly
	*/

	void operator++() //prefix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};


	/*!
		VertexVertexIterator postfix operator ++, goes to the next neighboring vertex CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};

	/*!
		The neighboring vertex, pointed by the current iterator
	*/

	 VertexType * value() 
	 { 
		 if( !m_vertex->boundary() )
		 {
			 return (VertexType*)m_halfedge->target(); 
		 }

		 if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
		 {
			 return (VertexType*)m_halfedge->target();
		 }

		 if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
		 {
			 return (VertexType*)m_halfedge->source();
		 }
		 return NULL;
	 };

	/*!
		The neighboring vertex, pointed by the current iterator
	*/
	 VertexType * operator*() { return value(); };

	/*!
		Indicate whether all the neighboring vertices have been accessed.
	*/
	 bool end(){ return m_halfedge == NULL; };

	/*!
		Reset the iterator.
	*/
	 void reset()	{ m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); };

private:
	/*!
		Current vertex
	*/
	VertexType *   m_vertex;
	/*!	
		Current halfedge.
	*/
	HalfEdgeType * m_halfedge;
};


//v -> edge
/*!
	\brief VertexEdgeIterator, transverse all the neighboring edges of a vertex ccwly.
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexEdgeIterator
{
public:
	/*!
		VertexEdgeIterator constructor
		\param v the current vertex
	*/
	VertexEdgeIterator( VertexType *  v )
	{ 
		m_vertex = v; 
		m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge();
	};

	/*!
		VertexVertexIterator destructor
	*/
	~VertexEdgeIterator(){};
/*!
		VertexVertexIterator prefix operator ++, goes to the next neighboring vertex CCWly
	*/

	void operator++() //prefix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};
	/*!
		VertexVertexIterator postfix operator ++, goes to the next neighboring vertex CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};
	/*!
		The neighboring edge, pointed by the current iterator
	*/

	 EdgeType * value() 
	 { 
		 assert( m_halfedge != NULL );
		 return (EdgeType*)m_halfedge->edge();
	 };

	/*!
		The neighboring edge, pointed by the current iterator
	*/
	 EdgeType * operator*() { return value(); };
	/*!
		Indicate whether all the neighboring edges have been accessed.
	*/
	 bool end(){ return m_halfedge == NULL; };
	/*!
		Reset the VerexEdgeIterator.
	*/
	 void reset()	{ m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); };

private:
	/*! current vertex 
	*/
	VertexType *   m_vertex;
	/*! current halfedge
	*/
	HalfEdgeType * m_halfedge;
};



// v->face
/*!
	\brief VertexFaceIterator, transverse all the neighboring faces of a vertex ccwly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexFaceIterator
{
public:
	/*!
		VertexFaceIterator constructor
		\param v the current vertex
	*/
	VertexFaceIterator( VertexType * & v )
	{ 
		m_vertex = v; 
		m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); 
	};
	/*!
		VertexFaceIterator destructor
	*/
	~VertexFaceIterator(){};
	/*!
		VertexFaceIterator prefix operator ++, goes to the next neighboring face CCWly
	*/
	void operator++() //prefix
	{
		assert( m_halfedge != NULL );  

		if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() ) 
		{
			m_halfedge = NULL;
			return;
		}
		m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
	};
	/*!
		VertexFaceIterator prefix operator ++, goes to the next neighboring face CCWly
	*/

	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL );  

		if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() ) 
		{
			m_halfedge = NULL;
			return;
		}
		m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
	};
	/*!
		The neighboring face, pointed by the current iterator
	*/
	FaceType * value() { return (FaceType*) m_halfedge->face(); };
	/*!
		The neighboring face, pointed by the current iterator
	*/
	 FaceType * operator*() { return value(); };
	/*!
		Indicate whether all the neighboring faces have been accessed.
	*/
	 bool end(){ return m_halfedge == NULL; };
	 /*!
	 Reset the VertexFaceIterator
	 */
	 void reset()	{ m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); };

private:
	/*!
	current vertex
	*/
	VertexType *   m_vertex;
	/*!
	current halfedge
	*/
	HalfEdgeType * m_halfedge;
};

// f -> halfedge
/*!
	\brief FaceHalfedgeIterator, transverse all the halfedges of a face CCWly.
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class FaceHalfedgeIterator
{
public:
	/*!
		FaceHalfedgeIterator constructor
		\param f the current face
	*/
	FaceHalfedgeIterator( FaceType * f )
	{ 
		m_face = f; 
		m_halfedge = (HalfEdgeType*)f->halfedge(); 
	};
	/*!
		FaceHalfedgeIterator destructor
	*/
	~FaceHalfedgeIterator(){};
	/*!
		VertexVertexIterator prefix operator ++, goes to the next halfedge CCWly
	*/
	void operator++() //prefix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		VertexVertexIterator prefix operator ++, goes to the next halfedge CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		The halfedge, pointed by the current iterator
	*/
	HalfEdgeType * value() { return m_halfedge; };
	/*!
		The halfedge, pointed by the current iterator
	*/
	HalfEdgeType * operator*() { return value(); };

	/*!
		Indicate whether all the halfedges have been accessed.
	*/
	bool end(){ return m_halfedge == NULL; };

private:
	/*!
		current face
	*/
	FaceType *        m_face;
	/*!
		current halfedge
	*/
	HalfEdgeType * m_halfedge;
};


// f -> edge
/*!
	\brief FaceEdgeIterator, transverse all the edges of a face CCWly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class FaceEdgeIterator
{
public:
	/*!
		FaceEdgeIterator constructor
		\param f the current face
	*/	
	FaceEdgeIterator( FaceType * f )
	{ 
		m_face = f; 
		m_halfedge = (HalfEdgeType*)f->halfedge(); 
	};

	/*!
		FaceEdgeIterator destructor
	*/	
	~FaceEdgeIterator(){};
	/*!
		FaceEdgeIterator prefix operator ++, goes to the next edge CCWly
	*/
	void operator++()	//prefix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == (HalfEdgeType*)m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		FaceEdgeIterator prefix operator ++, goes to the next edge CCWly
	*/
	void operator++(int)	//postfix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}
	/*!
		The edge, pointed by the current iterator
	*/
	EdgeType * value() { return (EdgeType*) m_halfedge->edge(); };
	/*!
		The edge, pointed by the current iterator
	*/
	EdgeType * operator*() { return value(); };
	/*!
		Indicate whether all the edges have been transversed.
	*/
	bool end(){ return m_halfedge == NULL; };

private:
	/*! Current face. */
	FaceType  *       m_face;
	/*! Current halfedge. */
	HalfEdgeType * m_halfedge;
};


// f -> vertex
/*!
	\brief FaceVertexIterator, transverse all the vertices of a face CCWly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class FaceVertexIterator
{
public:
	/*!
		FaceVertexIterator constructor
		\param f the current face
	*/
	FaceVertexIterator( FaceType * f )
	{ 
		m_face = f; 
		m_halfedge = (HalfEdgeType*)f->halfedge(); 
	};
	/*!
		FaceVertexIterator destructor
	*/
	
	~FaceVertexIterator(){};
	/*!
		FaceVertexIterator prefix operator ++, goes to the next vertex CCWly
	*/
	void operator++() //prefix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == (HalfEdgeType*)m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		FaceVertexIterator prefix operator ++, goes to the next vertex CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == (HalfEdgeType*)m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}
	/*!
		The vertex, pointed by the current iterator
	*/
	VertexType * value() { return (VertexType*) m_halfedge->target(); };
	/*!
		The vertex, pointed by the current iterator
	*/
	VertexType * operator*() { return value(); };
	/*!
		Indicate whether all the vertices have been accessed.
	*/
	bool end(){ return m_halfedge == NULL; };

private:
	/*!	Current face.
	*/
	FaceType         * m_face;
	/*!	Current halfedge.
	*/
	HalfEdgeType * m_halfedge;
};


// mesh->v
/*!
	\brief MeshVertexIterator, transverse all the vertices in the mesh.
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshVertexIterator
{
public:
	/*!
	MeshVertexIterator constructor, 
	\param pMesh the current mesh
	*/
	MeshVertexIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh )
	{
		m_pMesh = pMesh;
		m_iter = m_pMesh->vertices().begin();
	}
	/*!
	The vertex, pointed by the current iterator
	*/
	VertexType * value() { return *m_iter; };
	/*!
	The vertex, pointed by the current iterator
	*/
		
	VertexType * operator*(){ return value(); };
	/*!
		MeshVertexIterator prefix operator ++, goes to the next vertex 
	*/	
	void operator++()	 { ++ m_iter; }; //prefix
	/*!
		MeshVertexIterator prefix operator ++, goes to the next vertex 
	*/	
	void operator++(int) { ++ m_iter; }; //postfix
	/*!
		Indicate whether all the vertices have been accessed.
	*/
	bool end() { return m_iter == m_pMesh->vertices().end(); }
	
private:
	/*!
		Current mesh.
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * m_pMesh;
	/*! 
	Current vertex list iterator.
	*/
	typename std::list<VertexType*>::iterator m_iter;
};

// mesh->f
/*!
	\brief MeshFaceIterator, transverse all the faces in the mesh.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshFaceIterator
{
public:
	/*!
	MeshFaceIterator constructor, 
	\param pMesh the current mesh
	*/
	MeshFaceIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh )
	{
      m_pMesh = pMesh;
      m_iter = pMesh->faces().begin();
	}
	/*!
	The face, pointed by the current iterator
	*/
	FaceType * value() { return *m_iter; };
	/*!
	The face, pointed by the current iterator
	*/
	FaceType * operator*(){ return value(); };

	/*!
		MeshFaceIterator prefix operator ++, goes to the next vertex 
	*/
	void operator++() { ++ m_iter; }; //prefix
	/*!
		MeshFaceIterator postfix operator ++, goes to the next vertex 
	*/
	void operator++(int) { ++ m_iter; }; //postfix
	/*!
		Indicate whether all the faces have been accessed.
	*/
	bool end() { return m_iter == m_pMesh->faces().end(); }

private:
	/*! Current mesh.
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * m_pMesh;
	/*! Current face list iterator.
	*/
	typename std::list<FaceType*>::iterator  m_iter;
};

//Mesh->e
/*!
	\brief MeshEdgeIterator, transverse all the edges in the mesh.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshEdgeIterator
{
public:
	/*!
	MeshEdgeIterator constructor, 
	\param pMesh the current mesh
	*/	
	MeshEdgeIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh )
	{
		m_pMesh = pMesh;
		m_iter = m_pMesh->edges().begin();
	}
	/*!
	The edge, pointed by the current iterator
	*/	
	EdgeType * value() { return *m_iter; };
	/*!
	The edge, pointed by the current iterator
	*/	
	EdgeType * operator*(){ return value(); };
	/*!
		MeshEdgeIterator prefix operator ++, goes to the next edge
	*/	
	void operator++() { ++ m_iter; }; //prefix
	/*!
		MeshEdgeIterator postfix operator ++, goes to the next edge
	*/	
	void operator++(int) {m_iter++; }; //postfix
	/*!
		Indicate whether all the edges have been accessed.
	*/	
	bool end() { return m_iter == m_pMesh->edges().end(); }


private:
	/*!
	current mesh
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * m_pMesh;
	/*!
	current edge list iterator
	*/
	typename std::list<EdgeType*>::iterator m_iter;
};

// Mesh->he
/*!
	\brief MeshHalfEdgeIterator, transverse all the halfedges in the mesh.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshHalfEdgeIterator
{
public:
	/*!
	MeshHalfedgeIterator constructor, 
	\param pMesh the current mesh
	*/
	MeshHalfEdgeIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh )
	{
     m_pMesh = pMesh;
     m_iter = m_pMesh->edges().begin();
     m_id = 0;
	}
	/*!
	The halfedge, pointed by the current iterator
	*/	
	HalfEdgeType * value() { EdgeType * e = *m_iter; return (HalfEdgeType*)e->halfedge(m_id); };
	/*!
	The halfedge, pointed by the current iterator
	*/	
	HalfEdgeType * operator*(){ return value(); };
	/*!
		MeshVertexIterator prefix operator ++, goes to the next halfedge 
	*/
	void operator++() //prefix
	{ 
		++m_id;

		switch( m_id )
		{
		case 1:
			{
				EdgeType * e = *m_iter;
				if( e->halfedge(m_id) == NULL )
				{
					m_id = 0;
					++ m_iter;
				}
			}
			break;
		case 2:
			m_id = 0;
			++m_iter;
			break;
		}
	};
	/*!
		MeshVertexIterator postfix operator ++, goes to the next vertex 
	*/
	void operator++(int) //postfix
	{ 
		++m_id;

		switch( m_id )
		{
		case 1:
			{
				EdgeType * e = *m_iter;
				if( e->halfedge(m_id) == NULL )
				{
					m_id = 0;
					++ m_iter;
				}
			}
			break;
		case 2:
			m_id = 0;
			++m_iter;
			break;
		}
	};
	/*!
	Indicate whether all the halfedges have been accessed
	*/
	bool end() { return m_iter == m_pMesh->edges().end(); }
	

private:
	/*!
		Current halfedge
	*/
	HalfEdgeType * m_he;
	/*!
		Current mesh
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> *	 m_pMesh;
	/*!
		Current edge list iterator
	*/
	typename std::list<EdgeType*>::iterator m_iter;
	int  m_id;
};


} //Iterators

#endif
