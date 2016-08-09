#include "PubKey.h"

#include "../czz/CZZX.h"
#include "../utils/CPolyRingUtils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	long i, j;
	CZZX e;

	for (i = 0; i < params.tau; ++i) {
		CZZX lwe0;
		CZZX lwe1;
		CPolyRingUtils::sampleUniform2(lwe1, params.logQ, params.phim);
		if(params.isGauss) {
			CPolyRingUtils::sampleGaussian(e, params.phim, params.stdev);
		} else {
			CPolyRingUtils::sampleUniform(e, params.B, params.phim);
		}
		CPolyRingUtils::mulPolyRing2(lwe0, secretKey.s, lwe1, params.logQ, params.phim);
		CPolyRingUtils::subPolyRing2(lwe0, e, lwe0, params.logQ, params.phim);

		A0.push_back(lwe0);
		A1.push_back(lwe1);
	}

	CZZX s2, Ps2;

	CPolyRingUtils::mulPolyRing2(s2, secretKey.s, secretKey.s, params.logQ, params.phim);
	CPolyRingUtils::leftShiftPolyRing2(Ps2, s2, params.logT, params.logTQ, params.phim);
	CPolyRingUtils::sampleUniform2(aStar, params.logTQ, params.phim);

	if(params.isGauss) {
		CPolyRingUtils::sampleGaussian(e, params.phim, params.stdev);
	} else {
		CPolyRingUtils::sampleUniform(e, params.B, params.phim);
	}

	CPolyRingUtils::addPolyRing2(e, e, Ps2, params.logTQ, params.phim);
	CPolyRingUtils::mulPolyRing2(bStar, secretKey.s, aStar, params.logTQ, params.phim);
	CPolyRingUtils::subPolyRing2(bStar, e, bStar, params.logTQ, params.phim);
}

