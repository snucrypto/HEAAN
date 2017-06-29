#ifndef UTILS_EVALUATORUTILS_H_
#define UTILS_EVALUATORUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/RR.h>

#include "CZZ.h"

using namespace NTL;

static RR const Pi = ComputePi_RR();

class EvaluatorUtils {
public:

	//-----------------------------------------

	/**
	 * evaluates Z[i] value [(xr + i * xi) * p]
	 */
	static CZZ evaluateVal(const double& xr, const double& xi, const long& logp);

	static void evaluateDoubleVal(double& xr, double& xi, CZZ& x, const long& logp);

	static void evaluateDoubleVals(double*& xvecr, double*& xveci, CZZ*& xvec, const long size, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi) * p]
	 */
	static CZZ evaluateVal(const RR& xr, const RR& xi, const long& logp);

	static void evaluateRRVal(RR& xr, RR& xi, CZZ& x, const long& logp);
	/**
	 * evaluates random Z[i] value with real and imaginary parts in [0, 2^logp - 1]
	 */
	static CZZ evaluateRandomVal(const long& logp);

	/**
	 * evaluates random Z[i] value with absolute value p
	 */
	static CZZ evaluateRandomCircleVal(const long& logp);

	/**
	 * evaluates random Z[i] values with real and imaginary parts in [0, 2^logp - 1]
	 */
	static CZZ* evaluateRandomVals(const long& size, const long& logp);

	//-----------------------------------------

	/**
	 * evaluates Z[i] value [(xr + i * xi)^degree * p]
	 */
	static CZZ evaluatePow(const double& xr, const double& xi, const long& degree, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi)^degree * p]
	 */
	static CZZ evaluatePow(const RR& xr, const RR& xi, const long& degree, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi)^2^logDegree * p]
	 */
	static CZZ evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi)^2^logDegree * p]
	 */
	static CZZ evaluatePow2(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^j * p] for j=1,...,degree
	 */
	static CZZ* evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp);

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^j * p] for j=1,...,degree
	 */
	static CZZ* evaluatePowvec(const RR& xr, const RR& xi, const long& degree, const long& logp);

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^2^j * p] for j=1,...,logDegree
	 */
	static CZZ* evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^2^j * p] for j=1,...,logDegree
	 */
	static CZZ* evaluatePow2vec(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	/**
	 * evaluates Z[i] value [p^2 /(xr + i * xi)]
	 */
	static CZZ evaluateInverse(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [p^2 /(xr + i * xi)]
	 */
	static CZZ evaluateInverse(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z[i] value [log(xr + i * xi) * p]
	 */
	static CZZ evaluateLogarithm(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [exp(xr + i * xi) * p]
	 */
	static CZZ evaluateExponent(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [exp(xr + i * xi) * p]
	 */
	static CZZ evaluateExponent(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z[i] value [sigmoid(xr + i * xi) * p]
	 */
	static CZZ evaluateSigmoid(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [sigmoid(xr + i * xi) * p]
	 */
	static CZZ evaluateSigmoid(const RR& xr, const RR& xi, const long& logp);

	//-----------------------------------------

	/**
	 * multiplies vals by p
	 */
	static void leftShift(CZZ*& vals, const long& size, const long& logp);

	//-----------------------------------------

	/**
	 * rotates indexes of vals by shift
	 */
	static void idxShift(CZZ*& vals, const long& size, const long& shift);

	//-----------------------------------------
};

#endif /* UTILS_EVALUATORUTILS_H_ */
