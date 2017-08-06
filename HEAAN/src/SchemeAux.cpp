#include "SchemeAux.h"

#include <NTL/RR.h>
SchemeAux::SchemeAux(Params& params, bool computeTaylorPows): logp(params.logp) {
	ksiPowsr = new RR*[params.logN + 2];
	ksiPowsi = new RR*[params.logN + 2];
	for (long i = 0; i < params.logN + 2; ++i) {
		long ipow = (1 << i);
		RR* tmpr = new RR[ipow + 1];
		RR* tmpi = new RR[ipow + 1];

		for (long j = 0; j < ipow; ++j) {
			RR pi = ComputePi_RR();
			RR angle = 2.0 * pi * j / ipow;
			RR cosp = cos(angle);
			RR sinp = sin(angle);
			tmpr[j] = cosp;
			tmpi[j] = sinp;
		}
		tmpr[ipow] = tmpr[0];
		tmpi[ipow] = tmpi[0];
		ksiPowsr[i] = tmpr;
		ksiPowsi[i] = tmpi;
	}

	RR sqrt2 = sqrt(to_RR(2.0));
	sqrt2.e += logp - 1;
	poversqrt2 = to_ZZ(sqrt2);

	if(computeTaylorPows) {
		precomputeTaylorPows();
	}
}

void SchemeAux::precomputeTaylorPows() {
	double* logCoeffs = new double[11]{0,1,-0.5,1./3,-1./4,1./5,-1./6,1./7,-1./8,1./9,-1./10};
	double* expCoeffs = new double[11]{1,1,0.5,1./6,1./24,1./120,1./720,1./5040, 1./40320,1./362880,1./3628800};
	double* sigCoeffs = new double[11]{1./2,1./4,0,-1./48,0,1./480,0,-17./80640,0,31./1451520,0};
	double* sigbarCoeffs = new double[11]{1./2,-1./4,0,1./48,0,-1./480,0,17./80640,0,-31./1451520,0};

	double* sigGoodCoeffs = new double[8]{0.5,0.216884,0,0.00819276,0,0.000165861,0,-0.00000119581};
	double* sigbarGoodCoeffs = new double[8]{0.5,-0.216884,0,-0.00819276,0,-0.000165861,0,0.00000119581};
	double* sigprimeGoodCoeffs7 = new double[8]{-0.5,0.216884,0,0.00819276,0,0.000165861,0,-0.00000119581};
	double* sigprimeGoodCoeffs3 = new double[4]{-0.5,0.18,0,-0.003};

	insertTaylorPows(LOGARITHM, logCoeffs, 11);
	insertTaylorPows(EXPONENT, expCoeffs, 11);
	insertTaylorPows(SIGMOID, sigCoeffs, 11);
	insertTaylorPows(SIGMOIDBAR, sigbarCoeffs, 11);

	insertTaylorPows(SIGMOIDGOOD, sigGoodCoeffs, 8);
	insertTaylorPows(SIGMOIDBARGOOD, sigbarGoodCoeffs, 8);
	insertTaylorPows(SIGMOIDPRIMEGOOD7, sigprimeGoodCoeffs7, 8);
	insertTaylorPows(SIGMOIDPRIMEGOOD3, sigprimeGoodCoeffs3, 4);
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
