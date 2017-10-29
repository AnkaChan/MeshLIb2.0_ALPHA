#pragma once
#include <iostream>
#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include <MeshLib/core/bmp/RgbImage.h>

enum InterpolationMethod
{
	Rect,
	Nearest
};

namespace MeshLib {
	template<typename V, typename E, typename F, typename H>
	class CTextureMapper {
	private:
		typedef CInterface<CVertexVisual, CEdge, CFace, CHalfEdge> If;
		typedef CIterators<If> It;
		typedef CBaseMesh<V, E, F, H> CMesh;
		typedef CBaseMesh<V, E, F, H> * CMeshPtr;
	public:
		CTextureMapper() {};
		CTextureMapper(CMeshPtr new_pMesh, RgbImage * new_pImage) {
			setMesh(new_pMesh);
			setImage(new_pImage);
		};

		void setMesh(CMeshPtr new_pMesh) {
			pMesh = new_pMesh;
		}

		void setImage(RgbImage * new_pImage) {
			pImage = new_pImage;
			imgCols = pImage->GetNumCols();
			imgRows = pImage->GetNumRows();

		}

		float  rectangle_interpolation(double a, double b, double c, double d, double w, double h) {
			double s1 = w*h, s2 = (1.0 - w)*h, s3 = w*(1.0 - h), s4 = (1.0 - w)*(1.0 - h);
			return a*s4 + b*s3 + c*s2 + d*s1;
		}

		void uvToRgbRect(RgbImage * pImg, double u, double v, float & r, float & g, float & b) {
			if (u > 1 && u < 0) {
				std::cerr << "Wrong u value: " << u << std::endl;
				return;
			}
			else if (v > 1 && v < 0) {
				std::cerr << "Wrong v value: " << v << std::endl;
				return;
			}

			int x_floor = u * (double)imgCols;
			int y_floor = v * (double)imgRows;

			double lambda1 = u * (double)imgCols - (double)x_floor; 
			double lambda2 = v * (double)imgCols - (double)y_floor;

			float R1, B1, G1, R2, B2, G2, R3, B3, G3, R4, B4, G4;

			pImage->GetRgbPixel(x_floor, y_floor, &R1, &G1, &B1);
			pImage->GetRgbPixel(x_floor+1, y_floor, &R2, &G2, &B2);
			pImage->GetRgbPixel(x_floor, y_floor+1, &R3, &G3, &B3);
			pImage->GetRgbPixel(x_floor+1, y_floor+1, &R4, &G4, &B4);


			r = rectangle_interpolation(R1, R2, R3, R4, lambda1, lambda2);
			g = rectangle_interpolation(G1, G2, G3, G4, lambda1, lambda2);
			b = rectangle_interpolation(B1, B2, B3, B4, lambda1, lambda2);
		}

		void mapTextureToRGB() {
			for (V* pV : It::MVIterator(pMesh)) {
				switch (interpoMethod)
				{
				case Rect:
					uvToRgbRect(pImage, pV->uv()[0], pV->uv()[1], pV->r, pV->g, pV->b);
					break;
				case Nearest:

					break;
				default:
					break;
				}

			}

		};
	private:
		CMeshPtr pMesh;
		RgbImage * pImage;
		long imgCols, imgRows;
		InterpolationMethod interpoMethod = Rect;

	};
}