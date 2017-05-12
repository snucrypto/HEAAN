#include "StringUtils.h"

void StringUtils::show(vector<CZZ>& vals) {
	cout << "------------------" << endl;
	for (long i = 0; i < vals.size(); ++i) {
		cout << vals[i].toString() << endl;
	}
	cout << "------------------" << endl;
}
