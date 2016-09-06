#include "SecKey.h"

#include "../utils/CPolyRingUtils.h"

SecKey::SecKey(Params& params) {
	CPolyRingUtils::sampleGaussian(s, params.n, params.sigma);
}
