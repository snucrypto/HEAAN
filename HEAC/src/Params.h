#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

/**
 * Input params: logN, logl, logp, L, sigma
 * p is a power of 2 that corresponds to the base of the exponent and modulus switching
 * N is a power of 2 that corresponds to the ring Z[X] / (X^N + 1)
 * logl corresponds to additional number of bits needed for correct decryption on last level
 * L corresponds to number of levels
 * sigma corresponds to standard deviation for error and secret key coefficients generation from Gaussian distribution
 */
class Params {
public:

	//-----------------------------------------
	long logN; ///< N is a power of 2 that corresponds to the ring Z[X] / (X^N + 1)
	long logl; ///< logl corresponds to additional number of bits needed for correct decryption on last level
	long logp; ///< p is a power of 2 that corresponds to the base of the exponent and modulus switching
	long L; ///< L corresponds to number of levels
	//-----------------------------------------
	double sigma; ///< sigma corresponds to standard deviation for error and secret key coefficients generation from Gaussian distribution
	//-----------------------------------------

	long M; ///< M = 2 * N
	long N;
	long Nh; ///< Nh = N / 2
	long logNh;
	long logq; ///< q corresponds to highest modulus and q = p^L * 2^logl
	long logP; ///< P = q corresponds to big modulus for linearization procedure
	long logPq; ///< Pq = P * q


	ZZ p;
	ZZ q;
	ZZ Pq;

	ZZ* qi; ///< array of modulus used in scheme [q_0 = p * 2^logl, q_1 = p^2 * 2^logl, ..., q_{L-1} = q = p^L * 2^logl]
	ZZ* Pqi; ///< array of modulus used in linearization procedure [Pq_0 = P * q_0, ..., Pq_{L-1} = P * q_{L-1}]

	long** rotGroup; ///< auxiliary information about rotation group indexes for batch encoding
	long** rotGroupInv; ///< auxiliary information about rotation group indexes for batch encoding

	//-----------------------------------------

	Params(long logN, long logl, long logp, long L, double sigma = 3.2);

	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_PARAMS_H_ */
