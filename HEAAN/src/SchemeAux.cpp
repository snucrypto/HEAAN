#include "SchemeAux.h"

#include <NTL/RR.h>
SchemeAux::SchemeAux(Params& params, long precisionBits): precisionBits(precisionBits) {
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

	taylorCoeffsMap.insert(pair<string, double*>(LOGARITHM, new double[11]{0,1,-0.5,1./3,-1./4,1./5,-1./6,1./7,-1./8,1./9,-1./10}));
	taylorCoeffsMap.insert(pair<string, double*>(EXPONENT, new double[11]{1,1,0.5,1./6,1./24,1./120,1./720,1./5040, 1./40320,1./362880,1./3628800}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOID, new double[11]{1./2,1./4,0,-1./48,0,1./480,0,-17./80640,0,31./1451520,0}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDBAR, new double[11]{1./2,-1./4,0,1./48,0,-1./480,0,17./80640,0,-31./1451520,0}));

	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDGOOD, new double[8]{0.5,0.216884,0,-0.00819276,0,0.000165861,0,-0.00000119581}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDBARGOOD, new double[8]{0.5,-0.216884,0,0.00819276,0,-0.000165861,0,0.00000119581}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDPRIMEGOOD7, new double[8]{-0.5,0.216884,0,-0.00819276,0,0.000165861,0,-0.00000119581}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDPRIMEGOOD5, new double[6]{-0.5,0.2166,0,-0.0077,0,0.00011}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDPRIMEGOOD3, new double[4]{-0.5,0.19,0,-0.0035}));
}
