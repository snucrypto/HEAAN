#include "TaylorPows.h"

#include <utility>

TaylorPows::TaylorPows() {
	logp = 0;
}

void TaylorPows::setLogp(long logp) {
	this->logp = logp;
}

void TaylorPows::insert(string& name, double*& coeffs, long size) {
	coeffsMap.insert(pair<string, double*>(name, coeffs));
	ZZ* pows = new ZZ[size];
	if(logp < 31) {
		for (long i = 0; i < size; ++i) {
			pows[i] = to_ZZ((1 << logp) * coeffs[i]);
		}
	} else {
		for (long i = 0; i < size; ++i) {
			pows[i] = to_ZZ((1 << 30) * coeffs[i]) << (logp - 30);
		}
	}

	powsMap.insert(pair<string, ZZ*>(name, pows));
}

void TaylorPows::precompute() {
	double* logCoeffs = new double[11]{0, 1, -1./2, 1./3, -1./4, 1./5, -1./6, 1./7, -1./8, 1./9, -1./10};
	double* expCoeffs = new double[11]{1./1, 1./1, 1./2, 1./6, 1./24, 1./120, 1./720, 1./5040, 1./40320, 1./362880, 1./3628800};
	double* sigCoeffs = new double[11]{1./2, 1./4, 0, -1./48, 0, 1./480, 0, -17./80640, 0, 31./1451520, 0};

	insert(LOGARITHM, logCoeffs, 11);
	insert(EXPONENT, expCoeffs, 11);
	insert(SIGMOID, sigCoeffs, 11);
}


