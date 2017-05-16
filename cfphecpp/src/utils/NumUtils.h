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

	static vector<CZZ> fftSpecialRaw(vector<CZZ>& vals, KsiPows& ksiPows, const bool& isForward);
	static vector<CZZ> fftSpecial(vector<CZZ>& vals, KsiPows& ksiPows);
	static vector<CZZ> fftSpecialInv(vector<CZZ>& vals, KsiPows& ksiPows);

	//-----------------------------------------

	static vector<CZZ> fftRaw(vector<CZZ>& vals, KsiPows& ksiPows,
			const bool& isForward) {
		long valsSize = vals.size();
		if (valsSize == 1) {
			return vals;
		}
		long logValsSize = log2(valsSize);
		long Nh = valsSize >> 1;
		vector<CZZ> res, tmp, sub1, sub2;
		for (long i = 0; i < valsSize; i = i + 2) {
			sub1.push_back(vals[i]);
			sub2.push_back(vals[i + 1]);
		}
		vector<CZZ> y1 = fftRaw(sub1, ksiPows, isForward);
		vector<CZZ> y2 = fftRaw(sub2, ksiPows, isForward);
		if (isForward) {
			for (long i = 0; i < Nh; ++i) {
				y2[i] *= ksiPows.pows[logValsSize][i];
				y2[i] >>= ksiPows.logp;
			}
		} else {
			for (long i = 0; i < Nh; ++i) {
				y2[i] *= ksiPows.pows[logValsSize][valsSize - i];
				y2[i] >>= ksiPows.logp;
			}
		}
		for (long i = 0; i < Nh; ++i) {
			CZZ sum = y1[i] + y2[i];
			CZZ diff = y1[i] - y2[i];
			res.push_back(sum);
			tmp.push_back(diff);
		}
		for (long i = 0; i < Nh; ++i) {
			res.push_back(tmp[i]);
		}
		return res;
	}
	static vector<CZZ> fft(vector<CZZ>& vals, KsiPows& ksiPows);
	static vector<CZZ> fftInv(vector<CZZ>& vals, KsiPows& ksiPows);

	//-----------------------------------------

	static vector<CZZ> bitReverse(vector<CZZ>& vals);
	static vector<CZZ> fftButterfly(vector<CZZ>& vals, KsiPows& ksiPows);
	static vector<CZZ> fftButterflyInv(vector<CZZ>& vals, KsiPows& ksiPows);


};

#endif /* UTILS_NUMUTILS_H_ */
