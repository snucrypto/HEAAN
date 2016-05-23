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
	ZZX e;

	for (i = 0; i < params.tau; ++i) {
		ZZX lwe0;
		ZZX lwe1;
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

	ZZX s2, Ps2;

	ZRingUtils::mulRing(s2, secretKey.s, secretKey.s, params.qL, params.phi);
	ZRingUtils::mulByConstantRing(Ps2, s2, params.P, params.Pq, params.phi);
	cout << "QQQ" << endl;

	ZRingUtils::sampleUniform(aStar, params.Pq, params.phim);

	if(params.isGauss) {
		ZRingUtils::sampleGaussian(e, params.phim, params.stdev);
	} else {
		ZRingUtils::sampleUniform(e, params.B, params.phim);
	}

	ZRingUtils::addRing(e, e, Ps2, params.Pq, params.phi);
	ZRingUtils::mulRing(bStar, secretKey.s, aStar, params.Pq, params.phi);
	ZRingUtils::subRing(bStar, e, bStar, params.Pq, params.phi);
}

FPHEPubKey::~FPHEPubKey() {
	// TODO Auto-generated destructor stub
}

