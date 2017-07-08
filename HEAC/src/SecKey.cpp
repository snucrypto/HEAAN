#include "SecKey.h"

#include <sstream>
#include <string>

#include "NumUtils.h"

SecKey::SecKey(Params& params) {
	if(params.isGauss) {
		NumUtils::sampleGauss(sx, params.N, params.sigma);
	} else {
		NumUtils::sampleZO(sx, params.N, params.h);
	}
}

string SecKey::toString() {
	stringstream ss;
	ss << "SecretKey: [sx[0] = ";
	ss << sx.rep[0];
	ss << "]";
	return ss.str();
}
