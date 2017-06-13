#ifndef UTILS_EVALUATORUTILS_H_
#define UTILS_EVALUATORUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/RR.h>

#include "../czz/CZZ.h"

using namespace NTL;

static RR const Pi = ComputePi_RR();

class EvaluatorUtils {
public:

	//-----------------------------------------

	static CZZ evaluateVal(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateVal(const RR& xr, const RR& xi, const long& logp);

	static CZZ evaluateRandomVal(const long& logp);
	static CZZ evaluateRandomCircleVal(const long& logp);
	static CZZ* evaluateRandomVals(const long& size, const long& logp);

	//-----------------------------------------

	static CZZ evaluatePow(const double& xr, const double& xi, const long& degree, const long& logp);
	static CZZ evaluatePow(const RR& xr, const RR& xi, const long& degree, const long& logp);
	static CZZ evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& logp);
	static CZZ evaluatePow2(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	static CZZ* evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp);
	static CZZ* evaluatePowvec(const RR& xr, const RR& xi, const long& degree, const long& logp);
	static CZZ* evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp);
	static CZZ* evaluatePow2vec(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	static CZZ evaluateInverse(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateInverse(const RR& xr, const RR& xi, const long& logp);
	static CZZ evaluateLogarithm(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateExponent(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateExponent(const RR& xr, const RR& xi, const long& logp);
	static CZZ evaluateSigmoid(const double& xr, const double& xi, const long& logp);
	static CZZ evaluateSigmoid(const RR& xr, const RR& xi, const long& logp);

	//-----------------------------------------

	static void leftShift(CZZ*& vals, const long& size, const long& logp);

	//-----------------------------------------

	static void idxShift(CZZ*& vals, const long& size, const long& shift);

	//-----------------------------------------
};

#endif /* UTILS_EVALUATORUTILS_H_ */
