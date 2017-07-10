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
	 * evaluates double values x.r/p, x.i/p
	 * @param[out] real part x.r/p
	 * @param[out] imaginary part x.i/p
	 * @param[in] shifting bits
	 */
	static void evaluateDoubleVal(double& xr, double& xi, CZZ& x, const long& logp);

	/**
	 * evaluates RR values x.r/p, x.i/p
	 * @param[out] real part x.r/p
	 * @param[out] imaginary part x.i/p
	 * @param[in] log(p)
	 */
	static void evaluateRRVal(RR& xr, RR& xi, CZZ& x, const long& logp);

	/**
	 * evaluates arrays of double values [xvec.r/p], [xvec.i/p]
	 * @param[out] real part x.r/p
	 * @param[out] imaginary part x.i/p
	 * @param[in] log(p)
	 */
	static void evaluateDoubleVals(double*& xvecr, double*& xveci, CZZ*& xvec, const long size, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi) * p
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value (xr + i * xi) * p
	 */
	static CZZ evaluateVal(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi) * p
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value (xr + i * xi) * p
	 */
	static CZZ evaluateVal(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates random Z[i] value with real and imaginary parts in [0, p - 1]
	 * @param[in] log(p)
	 * @return random Z[i] value with real and imaginary parts in [0, p - 1]
	 */
	static CZZ evaluateRandomVal(const long& logp);

	/**
	 * evaluates random Z[i] value with absolute value p
	 * @param[in] log(p)
	 * @return random Z[i] value with absolute value p
	 */
	static CZZ evaluateRandomCircleVal(const long& logp);

	/**
	 * evaluates array of random Z[i] values with real and imaginary parts in [0, p - 1]
	 * @param[in] size of array
	 * @param[in] log(p)
	 * @return array of random Z[i] values with real and imaginary parts in [0, p - 1]
	 */
	static CZZ* evaluateRandomVals(const long& size, const long& logp);

	//-----------------------------------------

	/**
	 * evaluates Z[i] value [(xr + i * xi)^degree * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] log(p)
	 * @return Z[i] value [(xr + i * xi)^degree * p]
	 */
	static CZZ evaluatePow(const double& xr, const double& xi, const long& degree, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi)^degree * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] log(p)
	 * @return Z[i] value [(xr + i * xi)^degree * p]
	 */
	static CZZ evaluatePow(const RR& xr, const RR& xi, const long& degree, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi)^degree * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(degree)
	 * @param[in] log(p)
	 * @return Z[i] value [(xr + i * xi)^degree * p]
	 */
	static CZZ evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates Z[i] value [(xr + i * xi)^degree * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(degree)
	 * @param[in] log(p)
	 * @return Z[i] value [(xr + i * xi)^degree * p]
	 */
	static CZZ evaluatePow2(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^j * p] for j=1,...,degree
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] log(p)
	 * @return array of Z[i] values [(xr + i * xi)^j * p] for j=1,...,degree
	 */
	static CZZ* evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& logp);

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^j * p] for j=1,...,degree
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] log(p)
	 * @return array of Z[i] values [(xr + i * xi)^j * p] for j=1,...,degree
	 */
	static CZZ* evaluatePowvec(const RR& xr, const RR& xi, const long& degree, const long& logp);

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^j * p] for j=1,2,2^2,...,degree
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(degree)
	 * @param[in] log(p)
	 * @return array of Z[i] values [(xr + i * xi)^j * p] for j=1,2,2^2,...,degree
	 */
	static CZZ* evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates array of Z[i] values [(xr + i * xi)^j * p] for j=1,2,2^2,...,degree
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(degree)
	 * @param[in] log(p)
	 * @return array of Z[i] values [(xr + i * xi)^j * p] for j=1,2,2^2,...,degree
	 */
	static CZZ* evaluatePow2vec(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	//-----------------------------------------

	/**
	 * evaluates Z[i] value [p^2 /(xr + i * xi)]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [p^2 /(xr + i * xi)]
	 */
	static CZZ evaluateInverse(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [p^2 /(xr + i * xi)]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [p^2 /(xr + i * xi)]
	 */
	static CZZ evaluateInverse(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z[i] value [log(xr + i * xi) * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [log(xr + i * xi) * p]
	 */
	static CZZ evaluateLogarithm(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [exp(xr + i * xi) * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [exp(xr + i * xi) * p]
	 */
	static CZZ evaluateExponent(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [exp(xr + i * xi) * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [exp(xr + i * xi) * p]
	 */
	static CZZ evaluateExponent(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z[i] value [sigmoid(xr + i * xi) * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [sigmoid(xr + i * xi) * p]
	 */
	static CZZ evaluateSigmoid(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value [sigmoid(xr + i * xi) * p]
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value [sigmoid(xr + i * xi) * p]
	 */
	static CZZ evaluateSigmoid(const RR& xr, const RR& xi, const long& logp);

	//-----------------------------------------

	/**
	 * multiplies array of values by p
	 * @param[in, out] array of values
	 * @param[in] array size
	 * @param[in] log(p)
	 */
	static void leftShiftAndEqual(CZZ*& vals, const long& size, const long& logp);

	//-----------------------------------------

	/**
	 * left indexes rotation of values
	 * @param[in, out] array of values
	 * @param[in] array size
	 * @param[in] rotation size
	 */
	static void leftRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize);

	//-----------------------------------------
};

#endif /* UTILS_EVALUATORUTILS_H_ */
