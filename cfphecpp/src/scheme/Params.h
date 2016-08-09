#ifndef SCHEME_PARAMS_H_
#define SCHEME_PARAMS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

using namespace std;
using namespace NTL;

class PolyParams {
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

	PolyParams(long lambda, bool isGauss = false);
	virtual ~PolyParams();
};

#endif /* SCHEME_PARAMS_H_ */
