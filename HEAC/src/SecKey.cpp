#include "SecKey.h"

#include "NumUtils.h"

SecKey::SecKey(Params& params) {
	if(params.isGauss) {
		NumUtils::sampleGauss(sx, params.N, params.sigma);
	} else {
		NumUtils::sampleZO(sx, params.N, params.h);
	}
}
