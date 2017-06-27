#include "SecKey.h"

#include <sstream>
#include <string>

#include "NumUtils.h"

SecKey::SecKey(Params& params) {
	NumUtils::sampleGauss(sx, params.N, params.sigma);
}

string SecKey::toString() {
	stringstream ss;
	ss << "SecretKey: [sx[0] = ";
	ss << sx.rep[0];
	ss << "]";
	return ss.str();
}
