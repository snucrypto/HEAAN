/*
 * FPHEPubKey.cpp
 *
 *  Created on: May 15, 2016
 *      Author: kimandrik
 */

#include "FPHEFFTPubKey.h"

#include <NTL/ZZ.h>

#include "ZRingUtilsFFT.h"

FPHEFFTPubKey::FPHEFFTPubKey(FPHEFFTParams& params, FPHEFFTSecKey& secretKey) {
	long i;
	vector<ZZ> e;
	vector<ZZ> efft;
	vector<ZZ> lwe1;

	for (i = 0; i < params.tau; ++i) {
		ZRingUtilsFFT::sampleUniform(lwe1, params.qL, params.phim);
		if(params.isGauss) {
			ZRingUtilsFFT::sampleGaussian(e, params.phim, params.stdev);
		} else {
			ZRingUtilsFFT::sampleUniform(e, params.B, params.phim);
		}
		vector<ZZ> lwe1fft;
		vector<ZZ> lwe0fft;

		ZRingUtilsFFT::convertfft(lwe1fft, lwe1, params.fft, params.qL, params.phim);
		ZRingUtilsFFT::convertfft(efft, e, params.fft, params.qL, params.phim);

		ZRingUtilsFFT::mulFFTRing(lwe0fft, secretKey.sfft, lwe1fft, params.qL, params.phim);
		ZRingUtilsFFT::subFFTRing(lwe0fft, efft, lwe0fft, params.qL, params.phim);

		A0fft.push_back(lwe0fft);
		A1fft.push_back(lwe1fft);
	}

	vector<ZZ> s2fft;
	vector<ZZ> Ps2fft;

	ZRingUtilsFFT::mulFFTRing(s2fft, secretKey.sfft, secretKey.sfft, params.PqL, params.phim);

	ZRingUtilsFFT::mulFFTByConstantRing(Ps2fft, s2fft, params.P, params.PqL, params.phim);

	vector<ZZ> c1Star;
	ZRingUtilsFFT::sampleUniform(c1Star, params.PqL, params.phim);

	if(params.isGauss) {
		ZRingUtilsFFT::sampleGaussian(e, params.phim, params.stdev);
	} else {
		ZRingUtilsFFT::sampleUniform(e, params.B, params.phim);
	}
	ZRingUtilsFFT::convertfft(efft, e, params.fft, params.PqL, params.phim);
	ZRingUtilsFFT::convertfft(c1Starfft, c1Star, params.fft, params.PqL, params.phim);

	ZRingUtilsFFT::addFFTRing(efft, efft, Ps2fft, params.PqL, params.phim);
	ZRingUtilsFFT::mulFFTRing(c0Starfft, secretKey.sfft, c1Starfft, params.PqL, params.phim);
	ZRingUtilsFFT::subFFTRing(c0Starfft, efft, c0Starfft, params.PqL, params.phim);
}

FPHEFFTPubKey::~FPHEFFTPubKey() {
	// TODO Auto-generated destructor stub
}

