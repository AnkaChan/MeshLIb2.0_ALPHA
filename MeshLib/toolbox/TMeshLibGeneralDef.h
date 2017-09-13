#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cmath>
#include <MeshLib/core/TetMesh/TMeshLibHeaders.h>

using std::string;
using std::vector;
using std::list;
using std::set;
using std::map;
using std::cout;
using std::endl;
using namespace MeshLib;
using namespace TMeshLib;

typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIfGeneral;
typedef TIterators<TIfGeneral> TItGeneral;

