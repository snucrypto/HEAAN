#include "SecKey.h"

#include "NumUtils.h"

SecKey::SecKey(Params& params) {
	NumUtils::sampleHWT(sx, params.N, params.h);
}
