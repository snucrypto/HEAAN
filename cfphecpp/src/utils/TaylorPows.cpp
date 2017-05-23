#include "TaylorPows.h"

#include <initializer_list>
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
		long p = (1 << logp);
		for (long i = 0; i < size; ++i) {
			pows[i] = to_ZZ(p * coeffs[i]);
		}
	} else {
		long tmp1 = (1 << 30);
		for (long i = 0; i < size; ++i) {
			pows[i] = to_ZZ(tmp1 * coeffs[i]);
			pows[i] <<= (logp - 30);
		}
	}

	powsMap.insert(pair<string, ZZ*>(name, pows));
}

void TaylorPows::precompute() {

	double* expCoeffs = new double[11];
	double* sigCoeffs = new double[11];

	expCoeffs[0] = (1.0 / 1.0);
	expCoeffs[1] = (1.0 / 1.0);
	expCoeffs[2] = (1.0 / 2.0);
	expCoeffs[3] = (1.0 / 6.0);
	expCoeffs[4] = (1.0 / 24.0);
	expCoeffs[5] = (1.0 / 120.0);
	expCoeffs[6] = (1.0 / 720.0);
	expCoeffs[7] = (1.0 / 5040.0);
	expCoeffs[8] = (1.0 / 40320.0);
	//-----------------------------------------
	expCoeffs[9] = (1.0 / 362880.0);
	expCoeffs[10] = (1.0 / 3628800.0);


	sigCoeffs[0] = (1.0 / 2.0);
	sigCoeffs[1] = (1.0 / 4.0);
	sigCoeffs[2] = (0.0);
	sigCoeffs[3] = (-1.0 / 48.0);
	sigCoeffs[4] = (0.0);
	sigCoeffs[5] = (1.0 / 480.0);
	sigCoeffs[6] = (0.0);
	sigCoeffs[7] = (-17.0 / 80640.0);
	sigCoeffs[8] = (0.0);
	//-----------------------------------------
	sigCoeffs[9] = (31.0 / 1451520.0);
	sigCoeffs[10] = (0.0);

	insert(EXPONENT, expCoeffs, 11);
	insert(SIGMOID, sigCoeffs, 11);
}


