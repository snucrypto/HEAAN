/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "Params.h"


Params::Params(long logN, long logl, long logp, long L, double sigma, double rho, long h) : logN(logN), logl(logl), logp(logp), L(L), sigma(sigma), rho(rho), h(h) {
	N = 1 << logN;
	M = N << 1;
	logq = logl + logp * L;
	logP = logq;
	logPq = logP + logq;
	power(p, 2, logp);
	power(q, 2, logq);
	power(Pq, 2, logPq);

	for (long i = 0; i < L; ++i) {
		ZZ ql, Pql;
		long logql = logl + logp * (i + 1);
		power(ql, 2, logql);
		qi.push_back(ql);
		power(Pql, 2, logql + logP);
		Pqi.push_back(Pql);
	}

	ksiPows.setLogp(logp);
	ksiPows.precompute(logN + 2);
	taylorPows.setLogp(logp);
	taylorPows.precompute();
}
