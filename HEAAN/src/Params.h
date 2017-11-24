#ifndef HEAAN_PARAMS_H_
#define HEAAN_PARAMS_H_

#include <NTL/ZZ.h>

#include "Common.h"

using namespace std;
using namespace NTL;

class Params {
public:

	long logN; ///< log of N
	long logQ; ///< Q corresponds to the highest modulus
	double sigma; ///< standard deviation for Gaussian distribution
	long h; ///< parameter for HWT distribution

	long N; ///< N is a power-of-two that corresponds to the ring Z[X]/(X^N + 1)

	/**
	 * @param[in] logN: log of N
	 * @param[in] logQ: log of highest ciphertext modulus
	 * @param[in] sigma: standard deviation for Gaussian distribution
	 * @param[in] h: parameter for HWT distribution
	 */
	Params(long logN, long logQ, double sigma = 3.2, long h = 64);

	/**
	 * suggests logN value.
	 * @param[in] lambda: security parameter
	 * @param[in] logQ: log of highest ciphertext modulus
	 */
	static long suggestlogN(long lambda, long logQ);

};

#endif
