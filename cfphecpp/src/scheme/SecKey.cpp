/*
 * FPHESecKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "SecKey.h"

#include "../utils/PolyRingUtils.h"

PolySecKey::PolySecKey(PolyParams& params) {
	if(params.isGauss) {
		PolyRingUtils::sampleGaussian(s, params.phim, params.stdev);
	} else {
		PolyRingUtils::sampleUniform(s, params.B, params.phim);
	}
}

PolySecKey::~PolySecKey() {
	// TODO Auto-generated destructor stub
}


