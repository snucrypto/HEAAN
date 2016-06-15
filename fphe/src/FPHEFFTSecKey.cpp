#include "FPHEFFTSecKey.h"

#include "ZRingUtilsFFT.h"

FPHEFFTSecKey::FPHEFFTSecKey(FPHEFFTParams& params) {
	if(params.isGauss) {
		ZRingUtilsFFT::sampleGaussian(s, params.phim, params.stdev);
	} else {
		ZRingUtilsFFT::sampleUniform(s, params.B, params.phim);
	}
	ZRingUtilsFFT::convertfft(sfft, s, params.fft, params.qL, params.phim);
}

FPHEFFTSecKey::~FPHEFFTSecKey() {
	// TODO Auto-generated destructor stub
}


