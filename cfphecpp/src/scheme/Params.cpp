#include "Params.h"

Params::Params(long logN, long logl, long logp, long L, double sigma, double rho, long h) :
			logN(logN), logl(logl), logp(logp), L(L), sigma(sigma), rho(rho), h(h), qi(), Pqi(), group3pows(), group3powsInv() {

	//-----------------------------------------

	N = 1 << logN;
	M = N << 1;
	Nh = N >> 1;
	logNh = logN - 1;
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

	group3pows = new long*[logN];
	group3powsInv = new long*[logN];

	for (long i = 0; i < logN; ++i) {
		long ipow = 1 << i;
		long ipow2 = ipow << 2;
		group3pows[i] = new long[ipow];
		group3powsInv[i] = new long[ipow];
		long val = 1;
		for (long j = 0; j < ipow; ++j) {
			group3pows[i][j] = val;
			group3powsInv[i][j] = ipow2 - val;
			val *= 3;
			val %= ipow2;
		}
	}
}
