#include "SecKey.h"

#include "../utils/CPolyRingUtils.h"

SecKey::SecKey(Params& params) {
	if(params.isGauss) {
		CPolyRingUtils::sampleGaussian(s, params.phim, params.stdev);
	} else {
		CPolyRingUtils::sampleUniform(s, params.B, params.phim);
	}
}
