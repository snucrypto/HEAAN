/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "Params.h"

#include <cmath>

Params::Params(long lambda) : lambda(lambda) {
	L = 10;
	sigma = 3;
	rho = 0.5;
	d = 1 << 14;
	n = 1 << 13;
	h = 64;
	logp = 30;
	logq = logp * L;
	logP = logp * L;
	logPq = logq + logP;

	Bclean = (8.0 * sqrt(2.0) * sigma * n) + (6.0 * sigma * sqrt(n)) + (16.0 * sigma * sqrt(h * n * 1.0));
	Bscale = sqrt(3.0 * n) + 8 * sqrt(h * n / 3.0);
	for (long l = 0; l < L; ++l) {
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
