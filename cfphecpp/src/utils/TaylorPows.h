#ifndef UTILS_TAYLORPOWS_H_
#define UTILS_TAYLORPOWS_H_

#include <NTL/ZZ.h>
#include <iostream>
#include <map>

using namespace std;
using namespace NTL;

class TaylorPows {
public:

	long logp;

	map<string, double*> coeffsMap;
	map<string, ZZ*> powsMap;

	void setLogp(long logp);
	void insert(string& name, double*& coeffs, long size);

	void precompute();

	//-----------------------------------------

	TaylorPows();
};

static string EXPONENT = "Exponent";
static string SIGMOID = "Sigmoid";
static string LOGARITHM = "Logarithm";

#endif /* UTILS_TAYLORPOWS_H_ */
