#include "FPHEFFTSecKey.h"

#include "ZRingUtils.h"

FPHEFFTSecKey::FPHEFFTSecKey(FPHEFFTParams& params) {
	if(params.isGauss) {
		ZRingUtils::sampleGaussian(s, params.phim, params.stdev);
	} else {
		ZRingUtils::sampleUniform(s, params.B, params.phim);
	}
	ZRingUtils::convertfft(sfft, s, params.fft, params.qL, params.phim);
}

FPHEFFTSecKey::~FPHEFFTSecKey() {
	// TODO Auto-generated destructor stub
}


