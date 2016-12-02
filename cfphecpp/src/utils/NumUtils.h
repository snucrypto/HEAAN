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
	static void sampleGauss(ZZX& res, const long& d, const double& stdev);
	static void sampleGauss(CZZX& res, const long& d, const double& stdev);

	static void sampleZO(ZZX& res, const long& d, const double& rho);
	static void sampleZO(CZZX& res, const long& d, const double& rho);

	static void sampleUniform(ZZX& res, const long& d, const ZZ& bnd);
	static void sampleUniform(CZZX& res, const long& d, const ZZ& bnd);

	static void sampleUniform2(ZZX& res, const long& d, const long& logBnd);
	static void sampleUniform2(CZZX& res, const long& d, const long& logBnd);

	static vector<CZZ> fftRaw(vector<CZZ>& coeffs, CKsi& cksi, const bool& isForward);
	static vector<CZZ> fft(vector<CZZ>& coeffs, CKsi& cksi);
	static vector<CZZ> fftInv(vector<CZZ>& coeffs, CKsi& cksi);
};

#endif /* UTILS_NUMUTILS_H_ */
