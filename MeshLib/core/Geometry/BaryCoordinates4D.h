#pragma once

#define DIMENSION_BARYCOORD_4D 4

#include "Point4.h"
#include "..\..\core\TetMesh\Face.h"
#include "..\..\core\TetMesh\Edge.h"
#include "..\..\core\TetMesh\HalfEdge.h"
#include "..\..\core\TetMesh\Halfface.h"
#include "..\..\core\TetMesh\vertex.h"
#include "..\..\core\TetMesh\TInterface.h"
#include "..\..\core\TetMesh\TIterators2.h"
#include <vector>

namespace MeshLib
{
	namespace TMeshLib
	{
		class CBaryCoordinates4D {
		public:
			CBaryCoordinates4D() {};

			void setTet(CTet* pT) {
				pTetCorresponding = pT;
				int i = 0;
				for (CHalfFace* pHF : TIt::T_HFIterator(pT))
				{
					auto pV = TIf::TVertexVertex(TIf::HalfFaceOppositeTVertex(pHF));
					vpVerticeCorresponding[i] = pV;
					vpHalffaceCorresponding[i] = pHF;
					++i;
				}
			}

			CBaryCoordinates4D(CTet* pT) {
				setTet(pT);
			}
			
			CPoint bary2Descartes(CPoint4 bary) {
				CPoint d;
				for (int i = 0; i < DIMENSION_BARYCOORD_4D; ++i) {
					d += bary[i] * vpVerticeCorresponding[i]->position();
				}
				return d;
			}

			CPoint4 descartes2Bary(CPoint p) {
				CPoint4 bary;
				for (int i = 0; i < DIMENSION_BARYCOORD_4D; ++i) {
					TIf::HFPtr pHF = vpHalffaceCorresponding[i];
					TIf::VPtr pVa = vpVerticeCorresponding[i];
					TIf::VPtr pVOnHF;
					if (i < 3) {
						pVOnHF = vpVerticeCorresponding[i+1];
					}
					else {
						pVOnHF = vpVerticeCorresponding[0];
					}
					CPoint OppositeHFNomral = TIf::HalfFaceNormal(pHF);
					CPoint ba = pVa->position() - pVOnHF->position();
					CPoint bp = p - pVOnHF->position();

					bary[i] = (bp*OppositeHFNomral) / (ba*OppositeHFNomral);
				}
				return bary;
			}

			CVertex* getCorrespondingPVertex(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); return vpVerticeCorresponding[i]; };
			CHalfFace* getCorrespondingPHalfface(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); return vpHalffaceCorresponding[i]; };
			CTet* getCorrespondingPTet() { return pTetCorresponding; };
			void setVpVertice(CVertex* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); vpVerticeCorresponding[i] = value; };
			void setVpHalfface(CHalfFace* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); vpHalffaceCorresponding[i] = value; };
		private:
			typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
			typedef TIteratorCore<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIt;
			CVertex* vpVerticeCorresponding[DIMENSION_BARYCOORD_4D];
			CHalfFace* vpHalffaceCorresponding[DIMENSION_BARYCOORD_4D];
			CTet* pTetCorresponding;
		};

	}
}