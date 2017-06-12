#include "SchemeAux.h"

SchemeAux::SchemeAux(long logp, long logKsiSize): logp(logp), ksiPows() {
	precomputeKsiPows(logKsiSize);
	precomputeTaylorPows();
}

void SchemeAux::precomputeKsiPows(long logSize) {
	ksiPows = new CZZ*[logSize];
	if(logp < 31) {
		long p = (1 << logp);
		for (long i = 0; i < logSize; ++i) {
			long ipow = (1 << i);
			CZZ* temp = new CZZ[ipow + 1];

			for (long j = 0; j < ipow; ++j) {
				double angle = 2.0 * M_PI * j / ipow;
				temp[j] = CZZ(to_ZZ(cos(angle) * p), to_ZZ(sin(angle) * p));
			}
			temp[ipow] = temp[0];
			ksiPows[i] = temp;
		}
	} else {
		long tmp = (1 << 30);
		for (long i = 0; i < logSize; ++i) {
			long ipow = (1 << i);
			CZZ* temp = new CZZ[ipow + 1];
			for (long j = 0; j < ipow; ++j) {
				double angle = 2.0 * M_PI * j / ipow;
				ZZ rx = to_ZZ((long)(cos(angle) * tmp)) << (logp - 30);
				ZZ ix = to_ZZ((long)(sin(angle) * tmp)) << (logp - 30);
				temp[j] = CZZ(rx, ix);
			}
			temp[ipow] = temp[0];
			ksiPows[i] = temp;
		}
	}
}

void SchemeAux::precomputeTaylorPows() {
	double* logCoeffs  = new double[11]{   0,     1, -1./2,   1./3, -1./4,    1./5,  -1./6,       1./7,    -1./8,         1./9,     -1./10};
	double* expCoeffs  = new double[11]{   1,     1,  1./2,   1./6, 1./24,  1./120, 1./720,    1./5040, 1./40320,    1./362880, 1./3628800};
	double* sigCoeffs  = new double[11]{1./2,  1./4,     0, -1./48,     0,  1./480,      0, -17./80640,        0,  31./1451520,          0};
	double* sig1Coeffs = new double[11]{1./2, -1./4,     0,  1./48,     0, -1./480,      0,  17./80640,        0, -31./1451520,          0};

	insertTaylorPows(LOGARITHM,  logCoeffs, 11);
	insertTaylorPows( EXPONENT,  expCoeffs, 11);
	insertTaylorPows(  SIGMOID,  sigCoeffs, 11);
	insertTaylorPows( SIGMOID1, sig1Coeffs, 11);
}

void SchemeAux::insertTaylorPows(string& name, double*& coeffs, long size) {
	taylorCoeffsMap.insert(pair<string, double*>(name, coeffs));
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

	taylorPowsMap.insert(pair<string, ZZ*>(name, pows));
}
