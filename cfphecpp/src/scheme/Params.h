#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class Params {
public:
	long M;
	long N;
	long Nh;
	long logN;
	long logNh;
	long logl;
	long logp;
	long logq;
	long logP;
	long logPq;

	long L;
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

	string toString();
};

#endif /* SCHEME_PARAMS_H_ */
