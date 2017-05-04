#include "PubKey.h"

#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	ZZX e, s2;

	NumUtils::sampleUniform2(a, params.n, params.logq);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::mult(b, secretKey.s, a, params.q, params.n);
	Ring2Utils::sub(b, e, b, params.q, params.n);
	Ring2Utils::mult(s2, secretKey.s, secretKey.s, params.q, params.n);

	Ring2Utils::leftShiftAndEqual(s2, params.logq, params.logPq, params.n);
	NumUtils::sampleUniform2(aStar, params.n, params.logPq);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::addAndEqual(e, s2, params.Pq, params.n);
	Ring2Utils::mult(bStar, secretKey.s, aStar, params.Pq, params.n);
	Ring2Utils::sub(bStar, e, bStar, params.Pq, params.n);
}
