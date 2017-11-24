#ifndef HEAAN_CONTEXT_H_
#define HEAAN_CONTEXT_H_

#include <NTL/RR.h>

#include "Common.h"
#include "Params.h"

using namespace std;
using namespace NTL;

static RR const Pi = ComputePi_RR();

static string LOGARITHM = "Logarithm"; ///< log(x)
static string EXPONENT  = "Exponent"; ///< exp(x)
static string SIGMOID   = "Sigmoid"; ///< sigmoid(x) = exp(x) / (1 + exp(x))

class Context {
public:

	long logN; ///< log of N
	long logQ; ///< log of Q
	double sigma; ///< standard deviation for Gaussian distribution
	long h; ///< parameter for HWT distribution

	long N; ///< N is a power-of-two that corresponds to the ring Z[X]/(X^N + 1)
	long M; ///< M = 2N
	long logPQ; ///< log of PQ

	ZZ Q; ///< Q corresponds to the highest modulus
	ZZ PQ; ///< PQ = Q * Q

	long* rotGroup; ///< auxiliary information about rotation group indexes for batch encoding
	RR* ksiPowsr; ///< storing ksi pows for fft calculation
	RR* ksiPowsi; ///< storing ksi pows for fft calculation
	map<string, double*> taylorCoeffsMap; ///< storing taylor coefficients for function calculation

	Context(Params& params);

	virtual ~Context();
};

#endif /* CONTEXT_H_ */
