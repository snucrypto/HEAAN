#include "FFTSecKey.h"

#include "ZRingUtilsFFT.h"

FFTSecKey::FFTSecKey(FFTParams& params) {
	if(params.isGauss) {
		ZRingUtilsFFT::sampleGaussian(s, params.phim, params.stdev);
	} else {
		ZRingUtilsFFT::sampleUniform(s, params.B, params.phim);
	}
	ZRingUtilsFFT::convertfft(sfft, s, params.fft, params.qL, params.phim);
}

FFTSecKey::~FFTSecKey() {
	// TODO Auto-generated destructor stub
}


