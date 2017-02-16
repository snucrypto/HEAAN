#include "PubKey.h"

#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	ZZX e, s2;

	NumUtils::sampleUniform2(a, params.n, params.logq);
	NumUtils::sampleGauss(e, params.n, params.sigma);

	Ring2Utils::multNew(b, secretKey.s, a, params.logq, params.q, params.n);
	Ring2Utils::subNew(b, e, b, params.q, params.n);
	Ring2Utils::multNew(s2, secretKey.s, secretKey.s, params.logq, params.q, params.n);

	Ring2Utils::leftShiftAndEqual(s2, params.logq, params.logPq, params.n);
	NumUtils::sampleUniform2(aStar, params.n, params.logPq);
	NumUtils::sampleGauss(e, params.n, params.sigma);

	Ring2Utils::addAndEqualNew(e, s2, params.Pq, params.n);
	Ring2Utils::multNew(bStar, secretKey.s, aStar, params.logPq, params.Pq, params.n);
	Ring2Utils::subNew(bStar, e, bStar, params.Pq, params.n);
}
