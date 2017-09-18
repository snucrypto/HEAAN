#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class Params {
public:

	long logN; ///< N is a power of 2 that corresponds to the ring Z[X] / (X^N + 1)
	long logq; ///< q corresponds to the highest modulus
	double sigma; ///< sigma corresponds to standard deviation for error and secret key coefficients generation from Gaussian distribution
	long h; ///< hamming weight of secret key


	long N;
	long logqq; ///< qq = q * q

	ZZ q;
	ZZ qq;

	long* rotGroup; ///< auxiliary information about rotation group indexes for batch encoding


	/**
	 *@param[in] N is a power of 2 that corresponds to the ring Z[X] / (X^N + 1)
	 *@param[in] q is a power of 2 that corresponds to the highest modulus
	 *@param[in] sigma corresponds to standard deviation for error and secret key coefficients generation from Gaussian distribution
	 */
	Params(long logN, long logq, double sigma = 3.2, long h = 64);


	/**
	 * suggests logN value.
	 * @param[in] lambda - security parameter
	 * @param[in] logq value
	 */
	static long suggestlogN(long lambda, long logq);

};

#endif /* SCHEME_PARAMS_H_ */
