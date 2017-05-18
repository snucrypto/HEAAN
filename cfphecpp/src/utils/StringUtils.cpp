#include "StringUtils.h"

void StringUtils::show(vector<CZZ>& vals) {
	cout << "[";
	for (long i = 0; i < vals.size(); ++i) {
		cout << vals[i].toString() << ", ";
	}
	cout << "]" << endl;
}

//-----------------------------------------

void StringUtils::showcompare(CZZ& val1, CZZ& val2, string prefix) {
	cout << "---------------------" << endl;
	cout << "m" + prefix + ":" << val1.toString() << endl;
	cout << "d" + prefix + ":" << val2.toString() << endl;
	cout << "e" + prefix + ":" << (val1-val2).toString() << endl;
	cout << "---------------------" << endl;
}

void StringUtils::showcompare(vector<CZZ>& vals1, vector<CZZ>& vals2, string prefix) {
	for (long i = 0; i < vals1.size(); ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << vals1[i].toString() << endl;
		cout << "d" + prefix + ": " << i << " :" << vals2[i].toString() << endl;
		cout << "e" + prefix + ": " << i << " :" << (vals1[i]-vals2[i]).toString() << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::showcompare(vector<CZZ>& vals1, CZZ& val2, string prefix) {
	for (long i = 0; i < vals1.size(); ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << vals1[i].toString() << endl;
		cout << "d" + prefix + ": " << i << " :" << val2.toString() << endl;
		cout << "e" + prefix + ": " << i << " :" << (vals1[i]-val2).toString() << endl;
		cout << "---------------------" << endl;
	}
}

void StringUtils::showcompare(CZZ& val1, vector<CZZ>& vals2, string prefix) {
	for (long i = 0; i < vals2.size(); ++i) {
		cout << "---------------------" << endl;
		cout << "m" + prefix + ": " << i << " :" << val1.toString() << endl;
		cout << "d" + prefix + ": " << i << " :" << vals2[i].toString() << endl;
		cout << "e" + prefix + ": " << i << " :" << (val1-vals2[i]).toString() << endl;
		cout << "---------------------" << endl;
	}
}
