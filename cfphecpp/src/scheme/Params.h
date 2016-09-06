#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>
#include <vector>

using namespace std;
using namespace NTL;

class Params {
public:
	long lambda;
	long levels;
	long tau;

	double sigma;

	long logP;
	long d;
	long n;
	long h;
	long logT;
	long logQ;
	long logTQ;

	ZZ Bclean;
	ZZ Bscale;
	vector<ZZ> Bks;
	vector<ZZ> Bmult;

	Params(long lambda);
};

#endif /* SCHEME_PARAMS_H_ */
