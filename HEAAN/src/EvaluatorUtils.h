#ifndef UTILS_EVALUATORUTILS_H_
#define UTILS_EVALUATORUTILS_H_

#include <NTL/RR.h>

#include "SchemeAux.h"
#include "CZZ.h"

using namespace NTL;

class EvaluatorUtils {
public:

	/**
	 * evaluates RR values x.r >> bits, x.i >> bits
	 * @param[out] real part x.r >> bits
	 * @param[out] imaginary part x.i >> bits
	 * @param[in] shifting bits
	 */
	static void evaluateRRVal(RR& xr, RR& xi, CZZ& x, const long& bits);

	/**
	 * evaluates value xr << bits
	 * @param[in] x
	 * @param[in] bits
	 * @return x << bits
	 */
	static ZZ evaluateVal(const double& x, const long& bits);

	/**
	 * evaluates value xr << bits
	 * @param[in] x
	 * @param[in] bits
	 * @return x << bits
	 */
	static ZZ evaluateVal(const RR& x, const long& bits);

	/**
	 * evaluates Z[i] value (xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value (xr + i * xi) << bits
	 */
	static CZZ evaluateVal(const double& xr, const double& xi, const long& bits);

	/**
	 * evaluates Z[i] value (xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value (xr + i * xi) << bits
	 */
	static CZZ evaluateVal(const RR& xr, const RR& xi, const long& bits);

	/**
	 * evaluates random bits in Z[i]
	 * @param[in] bits
	 * @return random bits in Z[i]
	 */
	static CZZ evaluateRandomVal(const long& bits);

	/**
	 * evaluates random bits in Z[i]
	 * @param[in] bits
	 * @return random bits in Z[i]
	 */
	static CZZ evaluateRandomCircleVal(const long& bits);

	/**
	 * evaluates array of random bits in Z[i]
	 * @param[in] size of array
	 * @param[in] bits
	 * @return array of random bits in Z[i]
	 */
	static CZZ* evaluateRandomVals(const long& size, const long& bits);

	static CZZ* evaluateRandomZZVals(const long& size, const long& bits);
	//-----------------------------------------

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] bits
	 * @return Z[i] value (xr + i * xi)^d << bits
	 */
	static CZZ evaluatePow(const double& xr, const double& xi, const long& degree, const long& bits);

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] bits
	 * @return Z[i] value (xr + i * xi)^d << bits
	 */
	static CZZ evaluatePow(const RR& xr, const RR& xi, const long& degree, const long& bits);

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(degree)
	 * @param[in] bits
	 * @return Z[i] value (xr + i * xi)^d << bits
	 */
	static CZZ evaluatePow2(const double& xr, const double& xi, const long& logDegree, const long& bits);

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(degree)
	 * @param[in] bits
	 * @return Z[i] value (xr + i * xi)^d << bits
	 */
	static CZZ evaluatePow2(const RR& xr, const RR& xi, const long& logDegree, const long& bits);

	//-----------------------------------------

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << bits for j=1,...,d
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] bits
	 * @return array of Z[i] values (xr + i * xi)^j << bits for j=1,...,d
	 */
	static CZZ* evaluatePowvec(const double& xr, const double& xi, const long& degree, const long& bits);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << bits for j=1,...,d
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] power degree
	 * @param[in] bits
	 * @return array of Z[i] values (xr + i * xi)^j << bits for j=1,...,d
	 */
	static CZZ* evaluatePowvec(const RR& xr, const RR& xi, const long& degree, const long& bits);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << bits for j=1,2,2^2,...,d
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(d)
	 * @param[in] bits
	 * @return array of Z[i] values (xr + i * xi)^j << bits for j=1,2,2^2,...,d
	 */
	static CZZ* evaluatePow2vec(const double& xr, const double& xi, const long& logDegree, const long& bits);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << bits for j=1,2,2^2,...,d
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(d)
	 * @param[in] bits
	 * @return array of Z[i] values (xr + i * xi)^j << bits for j=1,2,2^2,...,d
	 */
	static CZZ* evaluatePow2vec(const RR& xr, const RR& xi, const long& logDegree, const long& bits);

	//-----------------------------------------

	/**
	 * evaluates Z[i] value 1 / (xr + i * xi) << (2 * bits)
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value 1 / (xr + i * xi) << (2 * bits)
	 */
	static CZZ evaluateInverse(const double& xr, const double& xi, const long& bits);

	/**
	 * evaluates Z[i] value 1 / (xr + i * xi) << (2 * bits)
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] log(p)
	 * @return Z[i] value 1 / (xr + i * xi) << (2 * bits)
	 */
	static CZZ evaluateInverse(const RR& xr, const RR& xi, const long& bits);

	/**
	 * evaluates Z[i] value log(xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value log(xr + i * xi) << bits
	 */
	static CZZ evaluateLogarithm(const double& xr, const double& xi, const long& bits);

	/**
	 * evaluates Z[i] value exp(xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value exp(xr + i * xi) << bits
	 */
	static CZZ evaluateExponent(const double& xr, const double& xi, const long& bits);

	/**
	 * evaluates Z[i] value exp(xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value exp(xr + i * xi) << bits
	 */
	static CZZ evaluateExponent(const RR& xr, const RR& xi, const long& bits);

	/**
	 * evaluates Z[i] value sigmoid(xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value sigmoid(xr + i * xi) << bits
	 */
	static CZZ evaluateSigmoid(const double& xr, const double& xi, const long& bits);

	/**
	 * evaluates Z[i] value sigmoid(xr + i * xi) << bits
	 * @param[in] real part
	 * @param[in] imaginary part
	 * @param[in] bits
	 * @return Z[i] value sigmoid(xr + i * xi) << bits
	 */
	static CZZ evaluateSigmoid(const RR& xr, const RR& xi, const long& bits);

	//-----------------------------------------

	/**
	 * left shift array of values by bits
	 * @param[in, out] array of values
	 * @param[in] array size
	 * @param[in] bits
	 */
	static void leftShiftAndEqual(CZZ*& vals, const long& size, const long& bits);

	//-----------------------------------------

	/**
	 * left indexes rotation of values
	 * @param[in, out] array of values
	 * @param[in] array size
	 * @param[in] rotation size
	 */
	static void leftRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize);

	/**
	 * right indexes rotation of values
	 * @param[in, out] array of values
	 * @param[in] array size
	 * @param[in] rotation size
	 */
	static void rightRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize);
	//-----------------------------------------
};

#endif /* UTILS_EVALUATORUTILS_H_ */
