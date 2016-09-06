/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */
#include "Params.h"

Params::Params(long lambda) : lambda(lambda) {
	levels = 5;
	sigma = 3;
	tau = 3;
	d = 2 << 14;
	n = 2 << 13;
	h = 64;
	logP = 20;
	logQ = logP * levels;
	logT = logP * levels;
	logTQ = logQ + logT;

	Bclean = (8.0 * sqrt(2.0) * sigma * n) + (6.0 * sigma * sqrt(n)) + (16.0 * sigma * sqrt(h * n * 1.0));
	Bscale = sqrt(3.0 * n) + 8 * sqrt(h * n / 3.0);
	for (long l = 0; l < levels; ++l) {
		ZZ Bkstemp;
		Bkstemp = 8 << (logP * (l+1));
		Bkstemp *= (sigma * n);
		Bkstemp /= sqrt(3.0);
		Bks.push_back(Bkstemp);

		ZZ Bmulttemp;
		Bmulttemp = Bkstemp >> logT;
		Bmulttemp += Bscale;
		Bks.push_back(Bmulttemp);
	}
}
