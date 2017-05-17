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
	ksiPows.setLogp(logp);
	ksiPows.precompute(logN + 2);
	taylorPows.setLogp(logp);
	taylorPows.precompute();
	power(p, 2, logp);
	power(q, 2, logq);
	power(Pq, 2, logPq);

//	Bclean = (8.0 * sqrt(2.0) * sigma * n) + (6.0 * sigma * sqrt(n)) + (16.0 * sigma * sqrt(h * n * 1.0));
//	Bscale = sqrt(3.0 * n) + 8 * sqrt(h * n / 3.0);

	for (long i = 0; i < L; ++i) {
		ZZ ql, Pql;
		long logql = logl + logp * (i + 1);
		power(ql, 2, logql);
		qi.push_back(ql);
		power(Pql, 2, logql + logP);
		Pqi.push_back(Pql);

//		ZZ Bkstemp;
//		Bkstemp = 8 << (logp * (i+1));
//		Bkstemp *= (sigma * n);
//		Bkstemp /= sqrt(3.0);
//		Bks.push_back(Bkstemp);

//		ZZ Bmulttemp;
//		Bmulttemp = Bkstemp >> logq;
//		Bmulttemp += Bscale;
//		Bks.push_back(Bmulttemp);
	}

}
