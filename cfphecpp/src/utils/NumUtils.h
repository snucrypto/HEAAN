#ifndef UTILS_NUMUTILS_H_
#define UTILS_NUMUTILS_H_

#include <NTL/ZZX.h>

#include "../czz/CZZ.h"

using namespace NTL;

class NumUtils {
public:

	//-----------------------------------------

	static void sampleGauss(ZZX& res, const long& size, const double& stdev);
	static void sampleZO(ZZX& res, const long& size);
	static void sampleUniform2(ZZX& res, const long& size, const long& logBnd);

	//-----------------------------------------

	static CZZ* fftRaw(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp, const bool& isForward);
	static CZZ* fft(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);
	static CZZ* fftInv(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);
	static CZZ* fftInvLazy(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);
	static CZZ* fftFull(CZZ*& vals1, CZZ*& vals2, const long& size, CZZ**& ksiPows, const long& logp);
	static CZZ* fftFullLazy(CZZ*& vals1, CZZ*& vals2, const long& size, CZZ**& ksiPows, const long& logp);

	//-----------------------------------------

	static CZZ* fftSpecial(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);
	static CZZ* fftSpecialInv(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);

	//-----------------------------------------
};

#endif /* UTILS_NUMUTILS_H_ */
