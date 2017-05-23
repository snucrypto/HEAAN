/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

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

	group3pows = new long[Nh];
	group3powsInv = new long[Nh];


	long val = 1;
	for (long i = 0; i < Nh; ++i) {
		group3pows[i] = val;
		group3powsInv[i] = M - group3pows[i];
		val *=3;
		val %=M;
	}

	ksiPows.setLogp(logp);
	ksiPows.precompute(logN + 2);
	taylorPows.setLogp(logp);
	taylorPows.precompute();

}
