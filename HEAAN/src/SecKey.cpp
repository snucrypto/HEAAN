#include "SecKey.h"

#include "NumUtils.h"

SecKey::SecKey(Params& params) {
	NumUtils::sampleBinary(sx, params.N, params.h);
}
