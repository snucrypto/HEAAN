#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>
#include <vector>

using namespace std;
using namespace NTL;

class Params {
public:
	long lambda;
	long L;
	double rho;
	double sigma;
	long logp;
	long d;
	long n;
	long h;
	long logP;
	long logq;
	long logPq;

	ZZ Bclean;
	ZZ Bscale;
	vector<ZZ> Bks;
	vector<ZZ> Bmult;

	Params(long lambda);
};

#endif /* SCHEME_PARAMS_H_ */
