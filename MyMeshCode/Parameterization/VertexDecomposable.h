#pragma once
#include <memory>
#include <vector>
#include <list>
#include <random>
#include <iostream>

#include <MeshLib/core/TetMesh/BaseTMesh.h>
#include <MeshLib/core/TetMesh/vertex.h>
#include <MeshLib/core/TetMesh/tvertex.h>
#include <MeshLib/core/TetMesh/edge.h>
#include <MeshLib/core/TetMesh/tedge.h>
#include <MeshLib/core/TetMesh/face.h>
#include <MeshLib/core/TetMesh/halfface.h>
#include <MeshLib/core/TetMesh/halfedge.h>
#include <MeshLib/core/TetMesh/tet.h>
#include <MeshLib/core/TetMesh/titerators.h>
#include <MeshLib/core/TetMesh/tinterface.h>

using std::cout;
using std::endl;

namespace MeshLib {
	namespace TMeshLib {
		template< typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class CVertexDecomposer {
		public:
			CVertexDecomposer(typename TIf::MeshPtr pTMesh) :
				m_pTMesh(pTMesh), 
				m_pVertexDecompositionList(new std::list<V*>)
			{};

			bool vertexDecompositionGreedily(T * pStartT);
		private:
			typedef TInterface<TV, V, HE, TE, E, HF, F, T> TIf;
			typedef TIterators<TIf> TIt;

			typename TIf::MeshPtr m_pTMesh;
			std::shared_ptr<std::list<V*>> m_pVertexDecompositionList;
		};
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool CVertexDecomposer<TV, V, HE, TE, E, HF, F, T>::vertexDecompositionGreedily(T * pStartT)
		{
			return false;
		}
	}
}