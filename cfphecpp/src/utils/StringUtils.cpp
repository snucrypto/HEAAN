#include "StringUtils.h"

void StringUtils::show(vector<CZZ>& vals) {
	cout << "[";
	for (long i = 0; i < vals.size(); ++i) {
		cout << vals[i].toString() << ", ";
	}
	cout << "]" << endl;
}
