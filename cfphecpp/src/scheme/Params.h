#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <vector>

#include "NTL/ZZ.h"
#include "../utils/KsiPows.h"

using namespace std;
using namespace NTL;

class Params {
public:
	long logN;
	long N;
	long logl;
	long logp;
	long logP;
	long logPq;
	long L;
	double sigma;
	double rho;
	long h;

	long M;
	long logq;

	KsiPows ksiPows;

	ZZ p;
	ZZ q;
	ZZ Pq;
	vector<ZZ> qi;
	vector<ZZ> Pqi;

//	ZZ Bclean;
//	ZZ Bscale;
//	vector<ZZ> Bks;
//	vector<ZZ> Bmult;

	Params(long logN, long logl, long logp, long L, double sigma, double rho, long h);
};

#endif /* SCHEME_PARAMS_H_ */
