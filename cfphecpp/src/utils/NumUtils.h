#ifndef UTILS_NUMUTILS_H_
#define UTILS_NUMUTILS_H_

#include <NTL/ZZX.h>

#include "../czz/CZZ.h"
#include "KsiPows.h"

using namespace NTL;

class NumUtils {
public:

	static void sampleGauss(ZZX& res, const long& size, const double& stdev);
	static void sampleZO(ZZX& res, const long& size);
	static void sampleUniform2(ZZX& res, const long& size, const long& logBnd);

	//-----------------------------------------

	static CZZ* fftRaw(CZZ*& vals, const long& size, KsiPows& ksiPows, const bool& isForward);
	static CZZ* fft(CZZ*& vals, const long& size, KsiPows& ksiPows);
	static CZZ* fftInv(CZZ*& vals, const long& size, KsiPows& ksiPows);
	static CZZ* fftInvLazy(CZZ*& vals, const long& size, KsiPows& ksiPows);

	static CZZ* fftFull(CZZ*& vals1, CZZ*& vals2, const long& size, KsiPows& ksiPows);
	static CZZ* fftFullLazy(CZZ*& vals1, CZZ*& vals2, const long& size, KsiPows& ksiPows);

	//-----------------------------------------

	static CZZ* fftSpecial(CZZ*& vals, const long& size, KsiPows& ksiPows);
	static CZZ* fftSpecialInv(CZZ*& vals, const long& size, KsiPows& ksiPows);

	//-----------------------------------------
};

#endif /* UTILS_NUMUTILS_H_ */
