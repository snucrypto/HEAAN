#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>

using namespace NTL;

class Params {
public:
	long lambda;
	long levels;
	long tau;

	bool isGauss;
	double stdev;

	long logP;
	long m;
	long phim;
	long logT;
	long logQ;
	long logTQ;

	ZZ B;
	ZZ nu;
	Params(long lambda, bool isGauss = false);
};

#endif /* SCHEME_PARAMS_H_ */
