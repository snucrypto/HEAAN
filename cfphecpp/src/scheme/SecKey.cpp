#include "SecKey.h"

#include "../utils/NumUtils.h"

SecKey::SecKey(Params& params) {
	NumUtils::sampleGauss(s, params.N, params.sigma);
}
