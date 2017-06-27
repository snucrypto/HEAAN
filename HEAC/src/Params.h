#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class Params {
public:

	long M; ///< M - power of two
	long N; ///< N = phi(M) = M/2 - degree of RLWE
	long Nh;
	long logN;
	long logNh;
	long logl; ///< additional amount of bits (normally small) needed for correct decoding
	long logp; ///< power of 2, modulus
	long logq; ///< Highest Modulus
	long logP; ///< Modulus needed in KeySwitching procedure
	long logPq;

	long L; ///< number of levels
	double sigma;
	double rho;
	long h;

	ZZ p;
	ZZ q;
	ZZ Pq;

	ZZ* qi;
	ZZ* Pqi;

	/**
	 * rotation group for rotating messages withing slots
	 */
	long** rotGroup;
	long** rotGroupInv;

	//-----------------------------------------

	Params(long logN, long logl, long logp, long L, double sigma = 3.2, double rho = 0.5, long h = 64);

	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_PARAMS_H_ */
