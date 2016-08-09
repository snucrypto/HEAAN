/*
 * FPHEPubKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "PubKey.h"

#include <NTL/ZZ.h>

#include "../utils/PolyRingUtils.h"

PolyPubKey::PolyPubKey(PolyParams& params, PolySecKey& secretKey) {
	long i, j;
	ZZX e;

	for (i = 0; i < params.tau; ++i) {
		ZZX lwe0;
		ZZX lwe1;
		PolyRingUtils::sampleUniform2(lwe1, params.logQ, params.phim);
		if(params.isGauss) {
			PolyRingUtils::sampleGaussian(e, params.phim, params.stdev);
		} else {
			PolyRingUtils::sampleUniform(e, params.B, params.phim);
		}
		PolyRingUtils::mulPolyRing2(lwe0, secretKey.s, lwe1, params.logQ, params.phim);
		PolyRingUtils::subPolyRing2(lwe0, e, lwe0, params.logQ, params.phim);

		A0.push_back(lwe0);
		A1.push_back(lwe1);
	}

	ZZX s2, Ps2;

	PolyRingUtils::mulPolyRing2(s2, secretKey.s, secretKey.s, params.logQ, params.phim);
	PolyRingUtils::leftShiftPolyRing2(Ps2, s2, params.logT, params.logTQ, params.phim);
	PolyRingUtils::sampleUniform2(aStar, params.logTQ, params.phim);

	if(params.isGauss) {
		PolyRingUtils::sampleGaussian(e, params.phim, params.stdev);
	} else {
		PolyRingUtils::sampleUniform(e, params.B, params.phim);
	}

	PolyRingUtils::addPolyRing2(e, e, Ps2, params.logTQ, params.phim);
	PolyRingUtils::mulPolyRing2(bStar, secretKey.s, aStar, params.logTQ, params.phim);
	PolyRingUtils::subPolyRing2(bStar, e, bStar, params.logTQ, params.phim);
}

PolyPubKey::~PolyPubKey() {
	// TODO Auto-generated destructor stub
}

