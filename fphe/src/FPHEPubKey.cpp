/*
 * FPHEPubKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "FPHEPubKey.h"
#include <NTL/ZZ.h>
#include "ZRingUtils.h"

FPHEPubKey::FPHEPubKey(FPHEParams& params, FPHESecKey& secretKey) {
	long i, j;

	for (i = 0; i < params.tau; ++i) {
		ZZX lwe0;
		ZZX lwe1;
		ZZX e;
		ZRingUtils::sampleUniform(lwe1, params.qL, params.phim);
		if(params.isGauss) {
			ZRingUtils::sampleGaussian(e, params.phim, params.stdev);
		} else {
			ZRingUtils::sampleUniform(e, params.B, params.phim);
		}
		ZRingUtils::mulRing(lwe0, secretKey.s, lwe1, params.qL, params.phi);
		ZRingUtils::subRing(lwe0, e, lwe0, params.qL, params.phi);

		A0.push_back(lwe0);
		A1.push_back(lwe1);
	}

	ZZX s2;
	ZRingUtils::mulRing(s2, secretKey.s, secretKey.s, params.qL, params.phi);

	ZZX ss[4] = {ZZX(1), secretKey.s, secretKey.s, s2};

	ZZ one = ZZ(1);
	for (j = 0; j < params.logQL; ++j) {
		for (i = 0; i < 4; ++i) {
			ZZ pow2;
			ZZX ssPow2;

			ZZX lwe0;
			ZZX lwe1;

			ZZX e;
			ZRingUtils::sampleUniform(lwe1, params.qL, params.phim);
			if(params.isGauss) {
				ZRingUtils::sampleGaussian(e, params.phim, params.stdev);
			} else {
				ZRingUtils::sampleUniform(e, params.B, params.phim);
			}

			LeftShift(pow2, one, j);

			ZRingUtils::mulByConstantRing(ssPow2, ss[i], pow2, params.qL, params.phi);
			ZRingUtils::mulRing(lwe0, secretKey.s, lwe1, params.qL, params.phi);
			ZRingUtils::subRing(lwe0, e, lwe0, params.qL, params.phi);
			ZRingUtils::addRing(lwe0, lwe0, ssPow2, params.qL, params.phi);

			AL0.push_back(lwe0);
			AL1.push_back(lwe1);
		}
	}

}

FPHEPubKey::~FPHEPubKey() {
	// TODO Auto-generated destructor stub
}

