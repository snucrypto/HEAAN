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
	static void sampleGauss(ZZX& res, long& d, double& stdev);
	static void sampleGauss(CZZX& res, long& d, double& stdev);

	static void sampleZO(ZZX& res, long& d, double& rho);
	static void sampleZO(CZZX& res, long& d, double& rho);

	static void sampleUniform(ZZX& res, long& d, ZZ& bnd);
	static void sampleUniform(CZZX& res, long& d, ZZ& bnd);

	static void sampleUniform2(ZZX& res, long& d, long& logBnd);
	static void sampleUniform2(CZZX& res, long& d, long& logBnd);

	static vector<CZZ> fft(vector<CZZ>& coeffs, CKsi& cksi);
	static vector<CZZ> fftInv(vector<CZZ>& coeffs, CKsi& cksi);

};

#endif /* UTILS_NUMUTILS_H_ */
