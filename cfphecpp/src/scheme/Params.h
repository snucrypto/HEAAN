#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <vector>

#include "../utils/CKsi.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

class Params {
public:
	long logn;
	long n;
	long logp;
	long logP;
	long logPq;
	long L;
	double sigma;
	double rho;
	long h;

	long d;
	long logq;

	CKsi cksi;

	ZZ p;
	ZZ Bclean;
	ZZ Bscale;
	vector<ZZ> qi;
	vector<ZZ> Bks;
	vector<ZZ> Bmult;

	Params(long logn, long logp, long L, double sigma, double rho, long h);
};

#endif /* SCHEME_PARAMS_H_ */
