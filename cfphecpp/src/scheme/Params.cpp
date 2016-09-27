/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "Params.h"

#include <cmath>

Params::Params(long n, long logp, long L, double sigma, double rho, long h) : n(n), logp(logp), L(L), sigma(sigma), rho(rho), h(h) {
	d = n << 1;
	logq = logp * L;
	logP = logp * L;
	logPq = logq + logP;

	power(p, 2, logp);

	Bclean = (8.0 * sqrt(2.0) * sigma * n) + (6.0 * sigma * sqrt(n)) + (16.0 * sigma * sqrt(h * n * 1.0));

	Bscale = sqrt(3.0 * n) + 8 * sqrt(h * n / 3.0);

	for (long l = 0; l < L; ++l) {
		ZZ ql;
		power(ql, 2, logp * (l+1));
		qi.push_back(ql);

		ZZ Bkstemp;
		Bkstemp = 8 << (logp * (l+1));
		Bkstemp *= (sigma * n);
		Bkstemp /= sqrt(3.0);
		Bks.push_back(Bkstemp);

		ZZ Bmulttemp;
		Bmulttemp = Bkstemp >> logP;
		Bmulttemp += Bscale;
		Bks.push_back(Bmulttemp);
	}
}
