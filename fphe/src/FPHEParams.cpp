/*
 * FPHEParams.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "FPHEParams.h"

#include <sstream>
#include <string>

#include "ZRingUtils.h"

FPHEParams::FPHEParams(long lambda, bool isGauss) : lambda(lambda), isGauss(isGauss) {
	levels = 7;
	tau = 10;
	m = 1247;
	phim = 1176;
	stdev = 3;

	GenPrime(p, 50);
	RandomBits(B, 5);

	qi = vector<ZZ>();
	logQi = vector<long>();

	for (int i = 1; i <= levels; ++i) {
		ZZ q = power(p, i);
		qi.push_back(q);
		logQi.push_back(NumBits(q));
	}

	qL = qi[levels-1];
	logQL = logQi[levels-1];
	phi = ZRingUtils::Cyclotomic(m);
}

ZZ& FPHEParams::getModulo(long level) {
	return qi[levels - level];
}

FPHEParams::~FPHEParams() {
	// TODO Auto-generated destructor stub
}

string FPHEParams::toString() {
	stringstream ss;

	ss << "Params: ";
	ss << "lambda=";
	ss << lambda;
	ss << ", p=";
	ss << p;
	ss << ", qL=";
	ss << qL;
	ss << ", levels=";
	ss << levels;
	ss << ", tau=";
	ss << tau;
	ss << ", m=";
	ss << m;
	ss << ", phim=";
	ss << phim;
	ss << ", isGauss=";
	ss << isGauss;
	ss << ", stdev=";
	ss << stdev;
	ss << ", B=";
	ss << B;
	ss << ", phi=";
	ss << phi;

	return ss.str();
}
