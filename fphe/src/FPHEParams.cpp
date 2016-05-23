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
	levels = 5;
	tau = 10;
	m = 3133;
	phim = 2880;
	stdev = 3;

	Pbits = 100;
	GenPrime(p, 20);

	RandomBits(B, 5);

	qi = vector<ZZ>();
	Pqi = vector<ZZ>();
	for (int i = 1; i <= levels; ++i) {
		ZZ q = power(p, i);
		qi.push_back(q);
		Pqi.push_back(q << Pbits);
	}

	qL = qi[levels-1];
	Pq = Pqi[levels-1];
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
