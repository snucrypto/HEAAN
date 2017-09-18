#include "PubKey.h"

#include "NumUtils.h"
#include "Params.h"
#include "Ring2Utils.h"

PubKey::PubKey(Params& params, SecKey& secretKey) {
	ZZX ex, sxsx;

	NumUtils::sampleUniform2(ax, params.N, params.logq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::mult(bx, secretKey.sx, ax, params.q, params.N);
	Ring2Utils::sub(bx, ex, bx, params.q, params.N);

	//-----------------------------------------

	axLeftRot = new ZZX[params.logN - 1];
	bxLeftRot = new ZZX[params.logN - 1];

	for (long i = 0; i < (params.logN - 1); ++i) {
		ZZX spow;
		long ipow = (1 << i);
		Ring2Utils::inpower(spow, secretKey.sx, params.rotGroup[ipow], params.q, params.N);
		Ring2Utils::leftShiftAndEqual(spow, params.logq, params.qq, params.N);
		NumUtils::sampleUniform2(axLeftRot[i], params.N, params.logqq);
		NumUtils::sampleGauss(ex, params.N, params.sigma);
		Ring2Utils::addAndEqual(ex, spow, params.qq, params.N);
		Ring2Utils::mult(bxLeftRot[i], secretKey.sx, axLeftRot[i], params.qq, params.N);
		Ring2Utils::sub(bxLeftRot[i], ex, bxLeftRot[i], params.qq, params.N);
	}

	axRightRot = new ZZX[params.logN - 1];
	bxRightRot = new ZZX[params.logN - 1];

	for (long i = 0; i < (params.logN - 1); ++i) {
		ZZX spow;
		long ipow = params.N/2 - (1 << i);
		Ring2Utils::inpower(spow, secretKey.sx, params.rotGroup[ipow], params.q, params.N);
		Ring2Utils::leftShiftAndEqual(spow, params.logq, params.qq, params.N);
		NumUtils::sampleUniform2(axRightRot[i], params.N, params.logqq);
		NumUtils::sampleGauss(ex, params.N, params.sigma);
		Ring2Utils::addAndEqual(ex, spow, params.qq, params.N);
		Ring2Utils::mult(bxRightRot[i], secretKey.sx, axRightRot[i], params.qq, params.N);
		Ring2Utils::sub(bxRightRot[i], ex, bxRightRot[i], params.qq, params.N);
	}

	//-----------------------------------------

	Ring2Utils::mult(sxsx, secretKey.sx, secretKey.sx, params.q, params.N);
	Ring2Utils::leftShiftAndEqual(sxsx, params.logq, params.qq, params.N);
	NumUtils::sampleUniform2(axStar, params.N, params.logqq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, sxsx, params.qq, params.N);
	Ring2Utils::mult(bxStar, secretKey.sx, axStar, params.qq, params.N);
	Ring2Utils::sub(bxStar, ex, bxStar, params.qq, params.N);

	ZZX sxconj;
	Ring2Utils::conjugate(sxconj, secretKey.sx, params.N);
	Ring2Utils::leftShiftAndEqual(sxconj, params.logq, params.qq, params.N);
	NumUtils::sampleUniform2(axConj, params.N, params.logqq);
	NumUtils::sampleGauss(ex, params.N, params.sigma);
	Ring2Utils::addAndEqual(ex, sxconj, params.qq, params.N);
	Ring2Utils::mult(bxConj, secretKey.sx, axConj, params.qq, params.N);
	Ring2Utils::sub(bxConj, ex, bxConj, params.qq, params.N);

	//-----------------------------------------
}
