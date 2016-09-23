#ifndef EVAL_KSI_H_
#define EVAL_KSI_H_

#include <math.h>
#include <vector>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class Ksi {
public:

	long n;
	long logpow;

	vector<ZZ> powr;
	vector<ZZ> powi;

	Ksi(long n, long logpow);
};

#endif /* EVAL_KSI_H_ */
