/*
 * FPHESecKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "FPHESecKey.h"

#include "ZRingUtils.h"

FPHESecKey::FPHESecKey(FPHEParams& params) {
	if(params.isGauss) {
		ZRingUtils::sampleGaussian(s, params.phim, params.stdev);
	} else {
		ZRingUtils::sampleUniform(s, params.B, params.phim);
	}
}

FPHESecKey::~FPHESecKey() {
	// TODO Auto-generated destructor stub
}


