/*
 * FPHESecKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "PolySecKey.h"

#include "../polyscheme/ZRingUtils.h"

PolySecKey::PolySecKey(PolyParams& params) {
	if(params.isGauss) {
		ZRingUtils::sampleGaussian(s, params.phim, params.stdev);
	} else {
		ZRingUtils::sampleUniform(s, params.B, params.phim);
	}
}

PolySecKey::~PolySecKey() {
	// TODO Auto-generated destructor stub
}


