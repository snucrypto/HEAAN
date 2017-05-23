#ifndef UTILS_EVALUATORUTILS_H_
#define UTILS_EVALUATORUTILS_H_

#include <NTL/ZZ.h>
#include <vector>

#include "../czz/CZZ.h"

using namespace NTL;

static double const Pi = 4.0 * atan(1.0);

class EvaluatorUtils {
public:

	static CZZ evaluateVal(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateRandomVal(const long& logp);
	static CZZ evaluateRandomCircleVal(const long& logp);
	static CZZ* evaluateRandomVals(const long& size, const long& logp);

	//-----------------------------------------

	static CZZ evaluatePow(const double& xr, const double& xi, const long& degree, const long& logp);
	static CZZ evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	static CZZ* evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp);
	static CZZ* evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	static CZZ evaluateInverse(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateExponent(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateSigmoid(const double& xr, const double& xi, const long& logp);

	//-----------------------------------------

	static void leftShift(CZZ*& vals, const long& size, const long& logp);
};

#endif /* UTILS_EVALUATORUTILS_H_ */
