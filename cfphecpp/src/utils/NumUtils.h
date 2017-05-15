#ifndef UTILS_NUMUTILS_H_
#define UTILS_NUMUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

#include "CKsi.h"
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"

using namespace NTL;

class NumUtils {
public:

	static void sampleGauss(vector<CZZ>& res, const long& d, const double& stdev);
	static void sampleGauss(ZZX& res, const long& d, const double& stdev);
	static void sampleZO(vector<CZZ>& res, const long& d);
	static void sampleZO(ZZX& res, const long& d);
	static void sampleUniform2(vector<CZZ>& res, const long& d, const long& logBnd);
	static void sampleUniform2(ZZX& res, const long& d, const long& logBnd);

	static vector<CZZ> fftHalfRaw(vector<CZZ>& coeffs, CKsi& cksi, const bool& isForward);
	static vector<CZZ> fftHalf(vector<CZZ>& coeffs, CKsi& cksi);
	static vector<CZZ> fftHalfInv(vector<CZZ>& coeffs, CKsi& cksi);

	static vector<CZZ> fftRaw(vector<CZZ>& coeffs, CKsi& cksi, const bool& isForward);
	static vector<CZZ> fft(vector<CZZ>& coeffs, CKsi& cksi);
	static vector<CZZ> fftInv(vector<CZZ>& coeffs, CKsi& cksi);

	static vector<CZZ> bitReverse(vector<CZZ>& coeffs);
	static vector<CZZ> fftbutterfly(vector<CZZ>& coeffs, CKsi& cksi);
	static vector<CZZ> fftbutterflyInv(vector<CZZ>& coeffs, CKsi& cksi);


};

#endif /* UTILS_NUMUTILS_H_ */
