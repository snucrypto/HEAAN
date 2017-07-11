#include "SchemeAux.h"

#include <NTL/RR.h>
SchemeAux::SchemeAux(Params& params, bool computeTaylorPows): logp(params.logp) {
	ksiPows = new CZZ*[params.logN + 2];
	for (long i = 0; i < params.logN + 2; ++i) {
		long ipow = (1 << i);
		CZZ* temp = new CZZ[ipow + 1];

		for (long j = 0; j < ipow; ++j) {
			RR pi = ComputePi_RR();
			RR angle = 2.0 * pi * j / ipow;
			RR cosp = cos(angle);
			RR sinp = sin(angle);
			cosp.e += logp;
			sinp.e += logp;
			temp[j] = CZZ(RoundToZZ(cosp), RoundToZZ(sinp));
		}
		temp[ipow] = temp[0];
		ksiPows[i] = temp;
	}

	RR sqrt2 = sqrt(to_RR(2.0));
	sqrt2.e += logp - 1;
	psqrt2 = to_ZZ(sqrt2);

	if(computeTaylorPows) {
		precomputeTaylorPows();
	}
}

void SchemeAux::precomputeTaylorPows() {
	double* logCoeffs  = new double[11]{   0,     1, -1./2,   1./3, -1./4,    1./5,  -1./6,       1./7,    -1./8,         1./9,     -1./10};
	double* expCoeffs  = new double[11]{   1,     1,  1./2,   1./6, 1./24,  1./120, 1./720,    1./5040, 1./40320,    1./362880, 1./3628800};
	double* sigCoeffs  = new double[11]{1./2,  1./4,     0, -1./48,     0,  1./480,      0, -17./80640,        0,  31./1451520,          0};
	double* sigbarCoeffs = new double[11]{1./2, -1./4,     0,  1./48,     0, -1./480,      0,  17./80640,        0, -31./1451520,          0};

	insertTaylorPows(  LOGARITHM,    logCoeffs, 11);
	insertTaylorPows(   EXPONENT,    expCoeffs, 11);
	insertTaylorPows(    SIGMOID,    sigCoeffs, 11);
	insertTaylorPows( SIGMOIDBAR, sigbarCoeffs, 11);
}

void SchemeAux::insertTaylorPows(string& name, double*& coeffs, long size) {
	taylorCoeffsMap.insert(pair<string, double*>(name, coeffs));
	ZZ* pows = new ZZ[size];
	for (long i = 0; i < size; ++i) {
		RR coeffsp = to_RR(coeffs[i]);
		coeffsp.e += logp;
		pows[i] = RoundToZZ(coeffsp);
	}

	taylorPowsMap.insert(pair<string, ZZ*>(name, pows));
}
