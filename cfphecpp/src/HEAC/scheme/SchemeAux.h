#ifndef SCHEME_SCHEMEAUX_H_
#define SCHEME_SCHEMEAUX_H_

#include "../czz/CZZ.h"
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <iostream>
#include <map>

using namespace std;
using namespace NTL;

class SchemeAux {
public:

	long logp;
	CZZ** ksiPows;
	map<string, double*> taylorCoeffsMap;
	map<string, ZZ*> taylorPowsMap;

	//-----------------------------------------

	SchemeAux(long logp, long logKsiSize);

	//-----------------------------------------

	void precomputeKsiPows(long logKsiSize);
	void precomputeTaylorPows();
	void insertTaylorPows(string& name, double*& coeffs, long size);
};

static string LOGARITHM = "Logarithm";
static string EXPONENT  = "Exponent";
static string SIGMOID   = "Sigmoid";
static string SIGMOID1  = "Sigmoid1";

#endif /* SCHEME_SCHEMEAUX_H_ */
