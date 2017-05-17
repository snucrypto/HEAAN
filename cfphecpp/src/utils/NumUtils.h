#ifndef UTILS_NUMUTILS_H_
#define UTILS_NUMUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

#include "../czz/CZZ.h"
#include "../czz/CZZX.h"
#include "KsiPows.h"

using namespace NTL;

class NumUtils {
public:

	static void sampleGauss(vector<CZZ>& res, const long& dim, const double& stdev);
	static void sampleGauss(ZZX& res, const long& dim, const double& stdev);
	static void sampleZO(vector<CZZ>& res, const long& dim);
	static void sampleZO(ZZX& res, const long& dim);
	static void sampleUniform2(vector<CZZ>& res, const long& dim, const long& logBnd);
	static void sampleUniform2(ZZX& res, const long& dim, const long& logBnd);

	//-----------------------------------------

	static vector<CZZ> doubleConjugate(vector<CZZ>& vals);

	//-----------------------------------------

	static vector<CZZ> fftRaw(vector<CZZ>& vals, KsiPows& ksiPows, const bool& isForward);
	static vector<CZZ> fft(vector<CZZ>& vals, KsiPows& ksiPows);
	static vector<CZZ> fftInv(vector<CZZ>& vals, KsiPows& ksiPows);

	//-----------------------------------------

	static vector<CZZ> fftSpecial(vector<CZZ>& vals, KsiPows& ksiPows);
	static vector<CZZ> fftSpecialInv(vector<CZZ>& vals, KsiPows& ksiPows);

	//-----------------------------------------

	static vector<CZZ> bitReverse(vector<CZZ>& vals);
	static vector<CZZ> fftButterfly(vector<CZZ>& vals, KsiPows& ksiPows);
	static vector<CZZ> fftButterflyInv(vector<CZZ>& vals, KsiPows& ksiPows);


};

#endif /* UTILS_NUMUTILS_H_ */
