#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <vector>

#include "NTL/ZZ.h"
#include "../utils/KsiPows.h"
#include "../utils/TaylorPows.h"

using namespace std;
using namespace NTL;

class Params {
public:
	long M;
	long N;
	long L;
	long logN;
	long logl;
	long logp;
	long logq;
	long logP;
	long logPq;

	double sigma;
	double rho;
	long h;

	ZZ p;
	ZZ q;
	ZZ Pq;

	vector<ZZ> qi;
	vector<ZZ> Pqi;

	KsiPows ksiPows;
	TaylorPows taylorPows;

	Params(long logN, long logl, long logp, long L, double sigma = 3.2, double rho = 0.5, long h = 64);
};

#endif /* SCHEME_PARAMS_H_ */
