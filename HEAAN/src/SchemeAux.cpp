#include "SchemeAux.h"

SchemeAux::SchemeAux(long& logN) {
	M = 1 << (logN + 1);

	ksiPowsr = new RR[M + 1];
	ksiPowsi = new RR[M + 1];

	for (long j = 0; j < M; ++j) {
		RR angle = 2.0 * Pi * j / M;
		ksiPowsr[j] = cos(angle);
		ksiPowsi[j] = sin(angle);
	}

	ksiPowsr[M] = ksiPowsr[0];
	ksiPowsi[M] = ksiPowsi[0];

	taylorCoeffsMap.insert(pair<string, double*>(LOGARITHM, new double[11]{0,1,-0.5,1./3,-1./4,1./5,-1./6,1./7,-1./8,1./9,-1./10}));
	taylorCoeffsMap.insert(pair<string, double*>(EXPONENT, new double[11]{1,1,0.5,1./6,1./24,1./120,1./720,1./5040, 1./40320,1./362880,1./3628800}));
	taylorCoeffsMap.insert(pair<string, double*>(SIGMOID, new double[11]{1./2,1./4,0,-1./48,0,1./480,0,-17./80640,0,31./1451520,0}));

//	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDBAR, new double[11]{1./2,-1./4,0,1./48,0,-1./480,0,17./80640,0,-31./1451520,0}));
//	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDGOOD, new double[8]{0.5,0.216884,0,-0.00819276,0,0.000165861,0,-0.00000119581}));
//	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDBARGOOD, new double[8]{0.5,-0.216884,0,0.00819276,0,-0.000165861,0,0.00000119581}));
//	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDPRIMEGOOD7, new double[8]{-0.5,0.216884,0,-0.00819276,0,0.000165861,0,-0.00000119581}));
//	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDPRIMEGOOD5, new double[6]{-0.5,0.2166,0,-0.0077,0,0.00011}));
//	taylorCoeffsMap.insert(pair<string, double*>(SIGMOIDPRIMEGOOD3, new double[4]{-0.5,0.19,0,-0.0035}));
}
