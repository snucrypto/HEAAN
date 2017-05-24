#include "PubKey.h"

#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) : aKeySwitch(), bKeySwitch() {
	ZZX e, s2;

	NumUtils::sampleUniform2(a, params.N, params.logq);
	NumUtils::sampleGauss(e, params.N, params.sigma);
	Ring2Utils::mult(b, secretKey.s, a, params.q, params.N);
	Ring2Utils::sub(b, e, b, params.q, params.N);

	//-----------------------------------------

	aKeySwitch = new ZZX[params.logNh];
	bKeySwitch = new ZZX[params.logNh];

	for (long i = 0; i < params.logNh; ++i) {
		ZZX spow;
		long ipow = (1 << i);
		Ring2Utils::inpower(spow, secretKey.s, params.group3pows[params.logNh][ipow], params.N);
		Ring2Utils::leftShiftAndEqual(spow, params.logP, params.logPq, params.N);
		NumUtils::sampleUniform2(aKeySwitch[i], params.N, params.logPq);
		NumUtils::sampleGauss(e, params.N, params.sigma);
		Ring2Utils::addAndEqual(e, spow, params.Pq, params.N);
		Ring2Utils::mult(bKeySwitch[i], secretKey.s, aKeySwitch[i], params.Pq, params.N);
		Ring2Utils::sub(bKeySwitch[i], e, bKeySwitch[i], params.Pq, params.N);
	}

	//-----------------------------------------

	Ring2Utils::mult(s2, secretKey.s, secretKey.s, params.q, params.N);
	Ring2Utils::leftShiftAndEqual(s2, params.logP, params.logPq, params.N);
	NumUtils::sampleUniform2(aStar, params.N, params.logPq);
	NumUtils::sampleGauss(e, params.N, params.sigma);
	Ring2Utils::addAndEqual(e, s2, params.Pq, params.N);
	Ring2Utils::mult(bStar, secretKey.s, aStar, params.Pq, params.N);
	Ring2Utils::sub(bStar, e, bStar, params.Pq, params.N);

	//-----------------------------------------
}
