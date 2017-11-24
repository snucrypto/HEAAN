#include "Params.h"


Params::Params(long logN, long logQ, double sigma, long h) : logN(logN), logQ(logQ), sigma(sigma), h(h) {
	N = 1 << logN;
}

long Params::suggestlogN(long lambda, long logQ) {
	long NBnd = ceil(logQ * (lambda + 110) / 3.6);
	double logNBnd = log2((double)NBnd);
	return (long)ceil(logNBnd);
}
