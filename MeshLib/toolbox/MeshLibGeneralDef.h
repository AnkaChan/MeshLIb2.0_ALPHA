#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cmath>
#include <MeshLib/core/Mesh/MeshCoreHeader.h>

using std::string;
using std::vector;
using std::list;
using std::set;
using std::map;
using std::cout;
using std::endl;
using namespace MeshLib;

typedef CInterface<CVertex, CEdge, CFace, CHalfEdge> IfGeneral;
typedef CIterators<IfGeneral> ItGeneral;