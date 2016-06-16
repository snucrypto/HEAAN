/*
 * FPHEPubKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "PolyPubKey.h"

#include <NTL/ZZ.h>

#include "../utils/PolyRingUtils.h"

PolyPubKey::PolyPubKey(PolyParams& params, PolySecKey& secretKey) {
	long i, j;
	ZZX e;

	for (i = 0; i < params.tau; ++i) {
		ZZX lwe0;
		ZZX lwe1;
		PolyRingUtils::sampleUniform(lwe1, params.qL, params.phim);
		if(params.isGauss) {
			PolyRingUtils::sampleGaussian(e, params.phim, params.stdev);
		} else {
			PolyRingUtils::sampleUniform(e, params.B, params.phim);
		}
		PolyRingUtils::mulPolyRing(lwe0, secretKey.s, lwe1, params.qL, params.phim);
		PolyRingUtils::subPolyRing(lwe0, e, lwe0, params.qL, params.phim);

		A0.push_back(lwe0);
		A1.push_back(lwe1);
	}

	ZZX s2, Ps2;

	PolyRingUtils::mulPolyRing(s2, secretKey.s, secretKey.s, params.qL, params.phim);
	PolyRingUtils::leftShiftPolyRing(Ps2, s2, params.Pbits, params.Pq, params.phim);
	PolyRingUtils::sampleUniform(aStar, params.Pq, params.phim);

	if(params.isGauss) {
		PolyRingUtils::sampleGaussian(e, params.phim, params.stdev);
	} else {
		PolyRingUtils::sampleUniform(e, params.B, params.phim);
	}

	PolyRingUtils::addPolyRing(e, e, Ps2, params.Pq, params.phim);
	PolyRingUtils::mulPolyRing(bStar, secretKey.s, aStar, params.Pq, params.phim);
	PolyRingUtils::subPolyRing(bStar, e, bStar, params.Pq, params.phim);
}

PolyPubKey::~PolyPubKey() {
	// TODO Auto-generated destructor stub
}

