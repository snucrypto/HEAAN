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
	long i;
	vector<ZZ> e;
	vector<ZZ> efft;
	vector<ZZ> lwe1;

	for (i = 0; i < params.tau; ++i) {
		ZRingUtils::sampleUniform(lwe1, params.qL, params.phim);
		if(params.isGauss) {
			ZRingUtils::sampleGaussian(e, params.phim, params.stdev);
		} else {
			ZRingUtils::sampleUniform(e, params.B, params.phim);
		}
		vector<ZZ> lwe1fft;
		vector<ZZ> lwe0fft;

		ZRingUtils::convertfft(lwe1fft, lwe1, params.fft, params.qL, params.phim);
		ZRingUtils::convertfft(efft, e, params.fft, params.qL, params.phim);

		ZRingUtils::mulRing(lwe0fft, secretKey.sfft, lwe1fft, params.qL, params.phim);
		ZRingUtils::subRing(lwe0fft, efft, lwe0fft, params.qL, params.phim);

		A0fft.push_back(lwe0fft);
		A1fft.push_back(lwe1fft);
	}

	vector<ZZ> s2fft;
	vector<ZZ> Ps2fft;

	ZRingUtils::mulRing(s2fft, secretKey.sfft, secretKey.sfft, params.PqL, params.phim);

	ZRingUtils::mulByConstantRing(Ps2fft, s2fft, params.P, params.PqL, params.phim);

	vector<ZZ> c1Star;
	ZRingUtils::sampleUniform(c1Star, params.PqL, params.phim);

	if(params.isGauss) {
		ZRingUtils::sampleGaussian(e, params.phim, params.stdev);
	} else {
		ZRingUtils::sampleUniform(e, params.B, params.phim);
	}
	ZRingUtils::convertfft(efft, e, params.fft, params.PqL, params.phim);
	ZRingUtils::convertfft(c1Starfft, c1Star, params.fft, params.PqL, params.phim);

	ZRingUtils::addRing(efft, efft, Ps2fft, params.PqL, params.phim);
	ZRingUtils::mulRing(c0Starfft, secretKey.sfft, c1Starfft, params.PqL, params.phim);
	ZRingUtils::subRing(c0Starfft, efft, c0Starfft, params.PqL, params.phim);
}

FPHEPubKey::~FPHEPubKey() {
	// TODO Auto-generated destructor stub
}

