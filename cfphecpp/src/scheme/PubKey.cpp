#include "PubKey.h"

#include "../czz/CZZX.h"
#include "../utils/CPolyRingUtils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	long i, j;
	CZZX e, s2, Ps2;

	CPolyRingUtils::sampleUniform2(a, params.logq, params.n);
	CPolyRingUtils::sampleGaussian(e, params.n, params.sigma);
	CPolyRingUtils::mulPolyRing2(b, secretKey.s, a, params.logq, params.n);
	CPolyRingUtils::subPolyRing2(b, e, b, params.logq, params.n);


	CPolyRingUtils::mulPolyRing2(s2, secretKey.s, secretKey.s, params.logq, params.n);
	CPolyRingUtils::leftShiftPolyRing2(Ps2, s2, params.logP, params.logPq, params.n);
	CPolyRingUtils::sampleUniform2(aStar, params.logPq, params.n);
	CPolyRingUtils::sampleGaussian(e, params.n, params.sigma);
	CPolyRingUtils::addPolyRing2(e, e, Ps2, params.logPq, params.n);
	CPolyRingUtils::mulPolyRing2(bStar, secretKey.s, aStar, params.logPq, params.n);
	CPolyRingUtils::subPolyRing2(bStar, e, bStar, params.logPq, params.n);
}

