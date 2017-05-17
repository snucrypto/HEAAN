#ifndef UTILS_TAYLORPOWS_H_
#define UTILS_TAYLORPOWS_H_

#include <NTL/ZZ.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace NTL;

class TaylorPows {
public:

	long logp;

	map<string, vector<double>> coeffsMap;
	map<string, vector<ZZ>> powsMap;

	void setLogp(long logp);
	void insert(string& name, vector<double>& coeffs);

	void precompute();

	//-----------------------------------------

	TaylorPows();
};

static string EXPONENT = "Exponent";
static string SIGMOID = "Sigmoid";

#endif /* UTILS_TAYLORPOWS_H_ */
