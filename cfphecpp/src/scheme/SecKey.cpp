#include "SecKey.h"

#include "../utils/NumUtils.h"

SecKey::SecKey(Params& params) {
	NumUtils::sampleGauss(sx, params.N, params.sigma);
}
