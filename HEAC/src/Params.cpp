#include "Params.h"

#include <NTL/tools.h>
#include <cmath>
#include <sstream>

Params::Params(long logN, long logl, long logp, long L, bool isGauss, double sigma, long h) :
			logN(logN), logl(logl), logp(logp), L(L), isGauss(isGauss), sigma(sigma), h(h) {
	//-----------------------------------------
	N = 1 << logN;
	logq = logl + logp * L;
	logP = logq;
	logPq = logP + logq;
	power(p, 2, logp);
	power(q, 2, logq);
	power(Pq, 2, logPq);

	qi = new ZZ[L];
	Pqi = new ZZ[L];

	for (long i = 0; i < L; ++i) {
		long logql = logl + logp * (i + 1);
		power(qi[i], 2, logql);
		power(Pqi[i], 2, logql + logP);
	}

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

long Params::suggestlogl(long logp, long L, long msgbits, long maxLevelAdditions) {
	double exbits = max(msgbits - logp, 0);
	double logadd = log2(maxLevelAdditions);
	double res = 0;
	for (long i = 0; i < L-1; ++i) {
		res += (logadd + exbits) * (logadd + exbits);
		exbits = res;
	}
	res += (logadd + exbits);
	return (long)ceil(res);
}

long Params::suggestlogN(long lambda, long logl, long logp, long L) {
	long logq = logp * L + logl;
	long res = 2 * logq * (lambda + 110) / 7.2;
	double logres = log2(res);
	return (long)ceil(logres);
}

string Params::toString() {
	stringstream ss;
	ss << "Params: [logN = " << logN;
	ss << " , logl = " << logl;
	ss << " , logp = " << logp;
	ss << " , L = " << L;
	ss <<"]";
	return ss.str();
}
