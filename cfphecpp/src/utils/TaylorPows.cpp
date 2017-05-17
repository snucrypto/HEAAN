#include "TaylorPows.h"


TaylorPows::TaylorPows() {
	logp = 0;
}

void TaylorPows::setLogp(long logp) {
	this->logp = logp;
}

void TaylorPows::precompute() {
	expCoeffs.push_back(1.0 / 1.0);
	expCoeffs.push_back(1.0 / 1.0);
	expCoeffs.push_back(1.0 / 2.0);
	expCoeffs.push_back(1.0 / 6.0);
	expCoeffs.push_back(1.0 / 24.0);
	expCoeffs.push_back(1.0 / 120.0);
	expCoeffs.push_back(1.0 / 720.0);
	expCoeffs.push_back(1.0 / 5040.0);
	expCoeffs.push_back(1.0 / 40320.0);
	expCoeffs.push_back(1.0 / 362880.0);
	expCoeffs.push_back(1.0 / 3628800.0);

	sigmoidCoeffs.push_back(1.0 / 2.0);
	sigmoidCoeffs.push_back(1.0 / 4.0);
	sigmoidCoeffs.push_back(0.0);
	sigmoidCoeffs.push_back(-1.0 / 48.0);
	sigmoidCoeffs.push_back(0.0);
	sigmoidCoeffs.push_back(1.0 / 480.0);
	sigmoidCoeffs.push_back(0.0);
	sigmoidCoeffs.push_back(-17.0 / 80640.0);
	sigmoidCoeffs.push_back(0.0);
	sigmoidCoeffs.push_back(31.0 / 1451520.0);
	sigmoidCoeffs.push_back(0.0);

	if(logp < 31) {
		long p = (1 << logp);
		for (long i = 0; i < expCoeffs.size(); ++i) {
			ZZ tmp = to_ZZ(p * expCoeffs[i]);
			expPows.push_back(tmp);
		}
		for (long i = 0; i < sigmoidCoeffs.size(); ++i) {
			ZZ tmp = to_ZZ(p * sigmoidCoeffs[i]);
			sigmoidPows.push_back(tmp);
		}
	} else {
		long tmp1 = (1 << 30);
		for (long i = 0; i < expCoeffs.size(); ++i) {
			ZZ tmp = to_ZZ(tmp1 * expCoeffs[i]);
			tmp <<= (logp - 30);
			expPows.push_back(tmp);
		}
		for (long i = 0; i < sigmoidCoeffs.size(); ++i) {
			ZZ tmp = to_ZZ(tmp1 * sigmoidCoeffs[i]);
			tmp <<= (logp - 30);
			sigmoidPows.push_back(tmp);
		}
	}
}


