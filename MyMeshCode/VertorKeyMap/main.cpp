#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;

typedef vector<int> KeyVec;
typedef std::pair<KeyVec, string> mapPair;
int main() {
	vector<int> v1 = {1, 7, 3, 2};
	vector<int> v2 = {1, -2, 3, 2};

	if (v1 == v2) {
		cout << "v1 = v2. " << endl;
	}
	else if (v1 > v2) {
		cout << "v1 > v2. " << endl;
	}
	else {
		cout << "v1 < v2. " << endl;
	}

	cout << "V1 before sort: " << v1[0] << " " << v1[1] << " " << v1[2] << " " << v1[3] << " " << endl;

	std::sort(v1.begin(), v1.end());

	cout << "V1 before sort: " << v1[0] << " " << v1[1] << " " << v1[2] << " " << v1[3] << " " << endl;


	map<vector<int>, string> vMap;
	vMap.insert(mapPair(v1, "v1_VALUE"));
	vMap.insert(mapPair(v2, "v2_VALUE"));

	cout << "The value of key v1: " << vMap[v1] << endl;
	cout << "The value of key v2: " << vMap[v2] << endl;

	KeyVec v3 = { 2, 2, 3 };
	if (vMap.count(v3) == 0) {
		cout << "Key v3 does not exist. " << endl;
	}

	getchar();
}