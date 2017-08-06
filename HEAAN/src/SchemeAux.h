#ifndef SCHEME_SCHEMEAUX_H_
#define SCHEME_SCHEMEAUX_H_

#include "CZZ.h"
#include "Params.h"
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <iostream>
#include <map>

using namespace std;
using namespace NTL;

class SchemeAux {
public:

	long logp;
	RR** ksiPowsr; ///< storing ksi pows for fft calculation
	RR** ksiPowsi; ///< storing ksi pows for fft calculation
	map<string, double*> taylorCoeffsMap; ///< storing taylor coefficients for function calculation
	map<string, ZZ*> taylorPowsMap; ///< storing taylor coefficients * p for function calculation

	ZZ poversqrt2; ///< p/sqrt(2) needed for i (imaginary unit) multiplication

	//-----------------------------------------

	SchemeAux(Params& params, bool computeTaylorPows = true);

	//-----------------------------------------

	/**
	 * precomputing information about taylor power series for logarithm, exponent sigmoid and (1 - sigmoid) functions
	 * this information needed for evaluating corresponding functions in encrypted state
	 */
	void precomputeTaylorPows();

	/**
	 * adding information about taylor power series for a function needed for evaluating this function in encrypted state
	 * @param[in] function name
	 * @param[in] array of taylor serires coefficients of a function
	 * @param[in] size of an array
	 */
	void insertTaylorPows(string& name, double*& coeffs, long size);
};

static string LOGARITHM = "Logarithm"; ///< log(x)
static string EXPONENT  = "Exponent"; ///< exp(x)
static string SIGMOID   = "Sigmoid"; ///< sigmoid(x) = exp(x) / (1 + exp(x))
static string SIGMOIDBAR  = "Sigmoidbar"; ///< sigmoidbar = 1 - sigmoid(x)

static string SIGMOIDGOOD = "SigmoidGood"; ///< sigmoid(x) = exp(x) / (1 + exp(x))
static string SIGMOIDBARGOOD = "SigmoidbarGood"; ///< sigmoidbar = 1 - sigmoid(x)
static string SIGMOIDPRIMEGOOD7 = "SigmoidprimeGood7"; ///< sigmoidprime = -1 / (1 + exp(x))
static string SIGMOIDPRIMEGOOD3 = "SigmoidprimeGood3"; ///< sigmoidprime = -1 / (1 + exp(x))

#endif /* SCHEME_SCHEMEAUX_H_ */
