#include "Params.h"

#include <cmath>

Params::Params(long logN, long logq, double sigma, long h) :
			logN(logN), logq(logq), sigma(sigma), h(h) {
	//-----------------------------------------
	N = 1 << logN;
	logqq = 2 * logq;
	q = power2_ZZ(logq);
	qq = power2_ZZ(logqq);

	rotGroup = new long[N / 2];

	long val = 1;
	long N2 = N << 1;
	for (long i = 0; i < N / 2; ++i) {
		rotGroup[i] = val;
		val *= 5;
		val %= N2;
	}
}

long Params::suggestlogN(long lambda, long logq) {
	long res = 2 * logq * (lambda + 110) / 7.2;
	double logres = log2(res);
	return (long)ceil(logres);
}
