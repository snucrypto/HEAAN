#include "Params.h"

#include <cmath>

Params::Params(long logN, long logq, double sigma, long h) :
			logN(logN), logq(logq), sigma(sigma), h(h) {
	//-----------------------------------------
	N = 1 << logN;
	logqq = 2 * logq;
	q = power2_ZZ(logq);
	qq = power2_ZZ(logqq);

	rotGroup = new long*[logN];
	rotGroupInv = new long*[logN];

	for (long i = 0; i < logN; ++i) {
		long ipow = 1 << i;
		long ipow2 = ipow << 2;
		rotGroup[i] = new long[ipow];
		rotGroupInv[i] = new long[ipow];
		long val = 1;
		for (long j = 0; j < ipow; ++j) {
			rotGroup[i][j] = val;
			rotGroupInv[i][j] = ipow2 - val;
			val *= 3;
			val %= ipow2;
		}
	}
}

long Params::suggestlogN(long lambda, long logq) {
	long res = 2 * logq * (lambda + 110) / 7.2;
	double logres = log2(res);
	return (long)ceil(logres);
}
