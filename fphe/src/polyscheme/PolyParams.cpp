/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "PolyParams.h"

#include <sstream>
#include <string>

#include "../utils/PolyRingUtils.h"

PolyParams::PolyParams(long lambda, bool isGauss) : lambda(lambda), isGauss(isGauss) {
	levels = 10;
	stdev = 3;
	tau = 3;
	m = 14;
	phim = 13;
	logP = 25;
	logQ = logP * levels;
	logT = logP * levels;
	logTQ = logQ + logT;
	B = 100;
}

PolyParams::~PolyParams() {
	// TODO Auto-generated destructor stub
}
