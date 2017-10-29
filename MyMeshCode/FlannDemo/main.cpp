#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include <flann/flann.hpp>
#include <flann/io/hdf5.h>

#include <MeshLib/core/Mesh/boundary.h>
//#include <MeshLib/core/viewer/SimpleMeshViewer.h>
#include <MeshLib/toolbox/FileIO.h>
#include <iostream>
#include <set>
using std::cout; 
using std::endl;
using namespace MeshLib;

typedef CInterface<CVertex, CEdge, CFace, CHalfEdge> If;
typedef CIterators<If> It;

const int nDim = 3;
int main(int argc, char** argv)
{
	If::MeshPtr pMesh = new If::MeshType;
	pMesh->read_m(argv[1]);


	int nn = 3;
	double a = 0;
	double * pA = &a;
	flann::Matrix<double> dataset(pA, nDim, pMesh->numVertices());
	flann::Matrix<double> query;
	
	dataset.rows();

	for (auto pV : It::MVIterator(pMesh)) {
		CPoint p = pV->point();
	}

	flann::Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
	flann::Matrix<float> dists(new float[query.rows*nn], query.rows, nn);
	// construct an randomized kd-tree index using 4 kd-trees
	flann::Index<flann::L2<float> > index(dataset, flann::KDTreeIndexParams(4));
	index.buildIndex();
	// do a knn search, using 128 checks
	index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));

	delete[] dataset.ptr();
	delete[] query.ptr();
	delete[] indices.ptr();
	delete[] dists.ptr();
	return 0;
}