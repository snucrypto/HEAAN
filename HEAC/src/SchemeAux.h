#ifndef SCHEME_SCHEMEAUX_H_
#define SCHEME_SCHEMEAUX_H_

#include "CZZ.h"
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <iostream>
#include <map>

using namespace std;
using namespace NTL;

class SchemeAux {
public:

	long logp;
	CZZ** ksiPows; ///< storing ksi pows for fft calculation
	map<string, double*> taylorCoeffsMap; ///< storing taylor coefficients for function calculation
	map<string, ZZ*> taylorPowsMap; ///< storing taylor coefficients * p for function calculation

	//-----------------------------------------

	SchemeAux(long logp, long logKsiSize);

	//-----------------------------------------

	void precomputeKsiPows(long logKsiSize);
	void precomputeTaylorPows();
	void insertTaylorPows(string& name, double*& coeffs, long size);
};

static string LOGARITHM = "Logarithm"; ///< log(x)
static string EXPONENT  = "Exponent"; ///< exp(x)
static string SIGMOID   = "Sigmoid"; ///< sigmoid(x) = exp(x) / (1 + exp(x))
static string SIGMOIDBAR  = "Sigmoidbar"; ///< sigmoidbar = 1 - sigmoid(x)

#endif /* SCHEME_SCHEMEAUX_H_ */
