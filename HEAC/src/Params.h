#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class Params {
public:

	//-----------------------------------------
	long logN; ///< N is a power of 2 that corresponds to the ring Z[X] / (X^N + 1)
	long logl; ///< logl corresponds to additional number of bits needed for correct decryption on last level
	long logp; ///< p is a power of 2 that corresponds to the base of the exponent and modulus switching
	long L; ///< L corresponds to number of levels
	//-----------------------------------------
	bool isGauss; ///< if distribution of secret key should be gaussian or from set {-1, 0, 1} with hamming weight h
	double sigma; ///< sigma corresponds to standard deviation for error and secret key coefficients generation from Gaussian distribution
	long h; ///< hamming weight of secret key
	//-----------------------------------------


	long N;
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

	/**
	 *@param[in] N is a power of 2 that corresponds to the ring Z[X] / (X^N + 1)
	 *@param[in] logl corresponds to additional number of bits needed for correct decryption on last level
	 *@param[in] p is a power of 2 that corresponds to the base of the exponent and modulus switching
	 *@param[in] L corresponds to number of levels
	 *@param[in] sigma corresponds to standard deviation for error and secret key coefficients generation from Gaussian distribution
	 */
	Params(long logN, long logl, long logp, long L, bool isGauss = true, double sigma = 3.2, long h = 64);

	/**
	 * suggests logl value. In reality however logl can be taken lower than suggested value
	 * @param[in] logp value
	 * @param[in] levels needed
	 * @param[in] supposed bits in values that are encrypted (logl increases fast if number of bits in values bigger than logp)
	 * @param[in] maximum number of additions in each level
	 * @return suggested logl value
	 */
	static long suggestlogl(long logp, long L, long msgbits, long maxLevelAdditions);

	/**
	 * suggests logN value. The formula is logN = log(2 * (logp * L + logl) / 3)
	 * @param[in] lambda - security parameter
	 * @param[in] logl value
	 * @param[in] logp value
	 * @param[in] levels needed
	 */
	static long suggestlogN(long lambda, long logl, long logp, long L);
	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_PARAMS_H_ */
