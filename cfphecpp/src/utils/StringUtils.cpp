#include "StringUtils.h"

void StringUtils::show(vector<CZZ>& vals) {
	cout << "[";
	for (long i = 0; i < vals.size(); ++i) {
		cout << vals[i].toString() << ", ";
	}
	cout << "]" << endl;
}

void StringUtils::showcompare(vector<CZZ>& mvals, vector<CZZ>& dvals, string prefix) {
	for (long i = 0; i < mvals.size(); ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + "i: " << i << " : " << mvals[i].toString() << endl;
		cout << "d" + prefix + "i: " << i << " : " << dvals[i].toString() << endl;
		cout << "e" + prefix + "i: " << i << " : " << (mvals[i]-dvals[i]).toString() << endl;
		cout << "---------------------" << endl;
	}

}
