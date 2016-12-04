#include "PubKey.h"

#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	ZZX e, s2, Ps2;

	NumUtils::sampleUniform2(a, params.n, params.logq);
	NumUtils::sampleGauss(e, params.n, params.sigma);

	Ring2Utils::mult(b, secretKey.s, a, params.logq, params.n);
	Ring2Utils::sub(b, e, b, params.logq, params.n);
	Ring2Utils::mult(s2, secretKey.s, secretKey.s, params.logq, params.n);

	long i;
	for (i = 0; i < params.L; ++i) {
		long logqi = params.logq - params.logp * i;
		long logTqi = logqi << 1;

		ZZX aStar;
		ZZX bStar;
		Ring2Utils::leftShift(Ps2, s2, logqi, logTqi, params.n);
		NumUtils::sampleUniform2(aStar, params.n, logTqi);
		NumUtils::sampleGauss(e, params.n, params.sigma);

		Ring2Utils::add(e, e, Ps2, logTqi, params.n);
		Ring2Utils::mult(bStar, secretKey.s, aStar, logTqi, params.n);
		Ring2Utils::sub(bStar, e, bStar, logTqi, params.n);
		aStars.push_back(aStar);
		bStars.push_back(bStar);
	}
}

