#include "FFTSecKey.h"

#include "../utils/FFTRingUtils.h"

FFTSecKey::FFTSecKey(FFTParams& params) {
	if(params.isGauss) {
		FFTRingUtils::sampleGaussian(s, params.phim, params.stdev);
	} else {
		FFTRingUtils::sampleUniform(s, params.B, params.phim);
	}
	FFTRingUtils::convertfft(sfft, s, params.fft, params.qL, params.phim);
}

FFTSecKey::~FFTSecKey() {
	// TODO Auto-generated destructor stub
}


