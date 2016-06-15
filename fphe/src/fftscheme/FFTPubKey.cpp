/*
 * FPHEPubKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "FFTPubKey.h"

#include <NTL/ZZ.h>

#include "../utils/FFTRingUtils.h"

FFTPubKey::FFTPubKey(FFTParams& params, FFTSecKey& secretKey) {
	long i;
	vector<ZZ> e;
	vector<ZZ> efft;
	vector<ZZ> lwe1;

	for (i = 0; i < params.tau; ++i) {
		FFTRingUtils::sampleUniform(lwe1, params.qL, params.phim);
		if(params.isGauss) {
			FFTRingUtils::sampleGaussian(e, params.phim, params.stdev);
		} else {
			FFTRingUtils::sampleUniform(e, params.B, params.phim);
		}
		vector<ZZ> lwe1fft;
		vector<ZZ> lwe0fft;

		FFTRingUtils::convertfft(lwe1fft, lwe1, params.fft, params.qL, params.phim);
		FFTRingUtils::convertfft(efft, e, params.fft, params.qL, params.phim);

		FFTRingUtils::mulFFTRing(lwe0fft, secretKey.sfft, lwe1fft, params.qL, params.phim);
		FFTRingUtils::subFFTRing(lwe0fft, efft, lwe0fft, params.qL, params.phim);

		A0fft.push_back(lwe0fft);
		A1fft.push_back(lwe1fft);
	}

	vector<ZZ> s2fft;
	vector<ZZ> Ps2fft;

	FFTRingUtils::mulFFTRing(s2fft, secretKey.sfft, secretKey.sfft, params.PqL, params.phim);

	FFTRingUtils::mulFFTByConstantRing(Ps2fft, s2fft, params.P, params.PqL, params.phim);

	vector<ZZ> c1Star;
	FFTRingUtils::sampleUniform(c1Star, params.PqL, params.phim);

	if(params.isGauss) {
		FFTRingUtils::sampleGaussian(e, params.phim, params.stdev);
	} else {
		FFTRingUtils::sampleUniform(e, params.B, params.phim);
	}
	FFTRingUtils::convertfft(efft, e, params.fft, params.PqL, params.phim);
	FFTRingUtils::convertfft(c1Starfft, c1Star, params.fft, params.PqL, params.phim);

	FFTRingUtils::addFFTRing(efft, efft, Ps2fft, params.PqL, params.phim);
	FFTRingUtils::mulFFTRing(c0Starfft, secretKey.sfft, c1Starfft, params.PqL, params.phim);
	FFTRingUtils::subFFTRing(c0Starfft, efft, c0Starfft, params.PqL, params.phim);
}

FFTPubKey::~FFTPubKey() {
	// TODO Auto-generated destructor stub
}

