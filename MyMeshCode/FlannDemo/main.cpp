#include <MeshLib/core/Mesh/MeshCoreHeader.h>
#include <MeshLib/3rdParty/flann/flann.hpp>
#include <MeshLib/3rdParty/flann/io/hdf5.h>

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

#include <stdio.h>
int main(int argc, char** argv)
{
	int nn = 3;
	flann::Matrix<float> dataset;
	flann::Matrix<float> query;
	flann::load_from_file(dataset, "dataset.hdf5", "dataset");
	flann::load_from_file(query, "dataset.hdf5", "query");
	flann::Matrix<int> indices(new int[query.rows*nn], query.rows, nn);
	flann::Matrix<float> dists(new float[query.rows*nn], query.rows, nn);
	// construct an randomized kd-tree index using 4 kd-trees
	flann::Index<flann::L2<float> > index(dataset, flann::KDTreeIndexParams(4));
	index.buildIndex();
	// do a knn search, using 128 checks
	index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));
	flann::save_to_file(indices, "result.hdf5", "result")

	delete[] dataset.ptr();
	delete[] query.ptr();
	delete[] indices.ptr();
	delete[] dists.ptr();
	return 0;
}