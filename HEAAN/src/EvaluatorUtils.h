#ifndef HEAAN_EVALUATORUTILS_H_
#define HEAAN_EVALUATORUTILS_H_

#include <NTL/RR.h>
#include <NTL/ZZ.h>

#include "Context.h"
#include "CZZ.h"

using namespace NTL;

class EvaluatorUtils {
public:


	//----------------------------------------------------------------------------------
	//   ZZ & CZZ TO DOUBLE
	//----------------------------------------------------------------------------------


	/**
	 * evaluates double value (x >> logp)
	 * @param[in] x: ZZ scaled up value
	 * @param[in] logp: log of precision
	 * @return x >> logp
	 */
	static double evalReal(const ZZ& x, const long& logp);

	/**
	 * evaluates double value (x >> logp)
	 * @param[out] res: double value (x >> logp)
	 * @param[in] x: ZZ scaled up value
	 * @param[in] logp: log of precision
	 */
	static void evalReal(double& res, const ZZ& x, const long& logp);

	/**
	 * evaluates real part of (x >> logp)
	 * @param[in] x: CZZ scaled up value
	 * @param[in] logp: log of precision
	 * @return real part of (x >> logp)
	 */
	static double evalReal(const CZZ& x, const long& logp);

	/**
	 * evaluates real part of (x >> logp)
	 * @param[out] res: real part of (x >> logp)
	 * @param[in] x: CZZ scaled up value
	 * @param[in] logp: log of precision
	 */
	static void evalReal(double& res, const CZZ& x, const long& logp);

	/**
	 * evaluates real and imaginary part of (x >> logp)
	 * @param[out] resx: real part of (x >> logp)
	 * @param[out] resi: imaginary part of (x >> logp)
	 * @param[in] x: CZZ scaled up value
	 * @param[in] logp: scaled bits
	 */
	static void evalComplex(double& resx, double& resi, const CZZ& x, const long& logp);

	/**
	 * evaluates double array of values xarray[i] >> logp
	 * @param[out] res: double array
	 * @param[in] xarray: ZZ scaled up array
	 * @param[in] size: array size
	 * @param[in] logp: log of precision
	 */
	static void evalRealArray(double*& res, const ZZ*& xarray, const long& size, const long& logp);

	/**
	 * evaluates double array of values xarray[i] >> logp
	 * @param[in] xarray: ZZ scaled up array
	 * @param[in] size: array size
	 * @param[in] logp: log of precision
	 * @return double array of values xarray[i] >> logp
	 */
	static double* evalRealArray(const ZZ*& xarray, const long& size, const long& logp);

	/**
	 * evaluates double array of real part of values xarray[i] >> logp
	 * @param[out] res: double array of real part
	 * @param[in] xarray: CZZ scaled up array
	 * @param[in] size: array size
	 * @param[in] logp: log of precision
	 */
	static void evalRealArray(double*& res, const CZZ*& xarray, const long& size, const long& logp);

	/**
	 * evaluates double array of real part of values xarray[i] >> logp
	 * @param[in] xarray: CZZ scaled up array
	 * @param[in] size: array size
	 * @param[in] logp: log of precision
	 * @return double array of real part of values xarray[i] >> logp
	 */
	static double* evalRealArray(const CZZ*& xarray, const long& size, const long& logp);

	/**
	 * evaluates double array of real and imaginary parts of values xarray[i] >> logp
	 * @param[out] resx: double array of real part
	 * @param[out] resi: double array of imaginary part
	 * @param[in] xarray: CZZ scaled up array
	 * @param[in] size: array size
	 * @param[in] logp: log of precision
	 */
	static void evalComplexArray(double*& resx, double*& resi, const CZZ*& xarray, const long& size, const long& logp);


	//----------------------------------------------------------------------------------
	//   DOUBLE & RR TO ZZ & CZZ
	//----------------------------------------------------------------------------------


	/**
	 * evaluates value x << logp
	 * @param[in] x: double value
	 * @param[in] logp: log of precision
	 * @return x << logp
	 */
	static ZZ evalZZ(const double& x, const long& logp);

	/**
	 * evaluates value x << logp
	 * @param[in] x: double value
	 * @param[in] logp: log of precision
	 * @return x << logp
	 */
	static ZZ evalZZ(const RR& x, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi) << logp
	 * @param[in] xr: double real part
	 * @param[in] xi: double imaginary part
	 * @param[in] logp: log of precision
	 * @return Z[i] value (xr + i * xi) << logp
	 */
	static CZZ evalCZZ(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi) << logp
	 * @param[in] xr: RR real part
	 * @param[in] xi: RR imaginary part
	 * @param[in] logp: log of precision
	 * @return Z[i] value (xr + i * xi) << logp
	 */
	static CZZ evalCZZ(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z value xr << logp as CZZ value
	 * @param[in] xr: double real part
	 * @param[in] logp: log of precision
	 * @return Z value xr << logp
	 */
	static CZZ evalCZZ0(const double& xr, const long& logp);

	/**
	 * evaluates Z value xr << logp as CZZ value
	 * @param[in] xr: RR real part
	 * @param[in] logp: log of precision
	 * @return Z value xr << logp
	 */
	static CZZ evalCZZ0(const RR& xr, const long& logp);

	/**
	 * evaluates array of Z[i] values (xr + i * xi) << logp
	 * @param[in] xr: double array of real parts
	 * @param[in] xr: double array of imaginary parts
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of Z[i] values (xr + i * xi) << logp
	 */
	static CZZ* evalCZZArray(double*& xr, double*& xi, const long& size, const long& logp);

	/**
	 * evaluates array of Z[i] values (xr + i * xi) << logp
	 * @param[in] xr: RR array of real parts
	 * @param[in] xr: RR array of imaginary parts
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of Z[i] values (xr + i * xi) << logp
	 */
	static CZZ* evalCZZArray(RR*& xr, RR*& xi, const long& size, const long& logp);

	/**
	 * evaluates array of Z values xr << logp as CZZ array
	 * @param[in] xr: double array of real parts
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of Z values xr << logp as CZZ array
	 */
	static CZZ* evalCZZ0Array(double*& xr, const long& size, const long& logp);

	/**
	 * evaluates array of Z values xr << logp as CZZ array
	 * @param[in] xr: RR array of real parts
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of Z values xr << logp as CZZ array
	 */
	static CZZ* evalCZZ0Array(RR*& xr, const long& size, const long& logp);


	//----------------------------------------------------------------------------------
	//   RANDOM
	//----------------------------------------------------------------------------------


	/**
	 * evaluates random bits in Z[i]
	 * @param[in] logp: number of bits
	 * @return random bits in Z[i]
	 */
	static CZZ evalRandCZZ(const long& logp);

	/**
	 * evaluates random bits in Z as CZZ
	 * @param[in] logp: number of bits
	 * @return random bits in Z as CZZ
	 */
	static CZZ evalRandCZZ0(const long& logp);

	/**
	 * evaluates random bits in Z[i]
	 * @param[in] logp: number of bits
	 * @return random bits in Z[i]
	 */
	static CZZ evalRandCZZCircle(const long& logp);

	/**
	 * evaluates array of random bits in Z[i]
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of random bits in Z[i]
	 */
	static ZZ* evalRandZZArray(const long& size, const long& logp);

	/**
	 * evaluates array of random bits in Z[i]
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of random bits in Z[i]
	 */
	static CZZ* evalRandCZZArray(const long& size, const long& logp);

	/**
	 * evaluates array of random bits in Z as CZZ
	 * @param[in] size: array size
	 * @param[in] logp: number of bits
	 * @return array of random bits in Z as CZZ
	 */
	static CZZ* evalRandCZZ0Array(const long& size, const long& logp);


	//----------------------------------------------------------------------------------
	//   POWERS
	//----------------------------------------------------------------------------------


	/**
	 * evaluates Z[i] value (xr + i * xi)^d << logp
	 * @param[in] xr: real part
	 * @param[in] xi: imaginary part
	 * @param[in] degree: power degree
	 * @param[in] logp: log of precision
	 * @return Z[i] value (xr + i * xi)^d << logp
	 */
	static CZZ evalCZZPow(const double& xr, const double& xi, const long& degree, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << logp
	 * @param[in] xr: real part
	 * @param[in] xi: imaginary part
	 * @param[in] degree: power degree
	 * @param[in] logp: log of precision
	 * @return Z[i] value (xr + i * xi)^d << logp
	 */
	static CZZ evalCZZPow(const RR& xr, const RR& xi, const long& degree, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << logp
	 * @param[in] xr: real part
	 * @param[in] xi: imaginary part
	 * @param[in] logDegree: log of degree
	 * @param[in] logp: log of precision
	 * @return Z[i] value (xr + i * xi)^d << logp
	 */
	static CZZ evalCZZPow2(const double& xr, const double& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates Z[i] value (xr + i * xi)^d << logp
	 * @param[in] xr: real part
	 * @param[in] xi: imaginary part
	 * @param[in] logDegree: log(degree)
	 * @param[in] logp: log of precision
	 * @return Z[i] value (xr + i * xi)^d << logp
	 */
	static CZZ evalCZZPow2(const RR& xr, const RR& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << logp for j=1,...,d
	 * @param[in] xr: real part
	 * @param[in] xi: imaginary part
	 * @param[in] degree: power degree
	 * @param[in] logp: log of precision
	 * @return array of Z[i] values (xr + i * xi)^j << logp for j=1,...,d
	 */
	static CZZ* evalCZZPowArray(const double& xr, const double& xi, const long& degree, const long& logp);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << logp for j=1,...,d
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] degree: power degree
	 * @param[in] logp: log of precision
	 * @return array of Z[i] values (xr + i * xi)^j << logp for j=1,...,d
	 */
	static CZZ* evalCZZPowArray(const RR& xr, const RR& xi, const long& degree, const long& logp);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << logp for j=1,2,2^2,...,d
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logDegree: log of degree
	 * @param[in] logp: log of precision
	 * @return array of Z[i] values (xr + i * xi)^j << logp for j=1,2,2^2,...,d
	 */
	static CZZ* evalCZZPow2Array(const double& xr, const double& xi, const long& logDegree, const long& logp);

	/**
	 * evaluates array of Z[i] values (xr + i * xi)^j << logp for j=1,2,2^2,...,d
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logDegree: log of degree
	 * @param[in] logp: log of precision
	 * @return array of Z[i] values (xr + i * xi)^j << logp for j=1,2,2^2,...,d
	 */
	static CZZ* evalCZZPow2Array(const RR& xr, const RR& xi, const long& logDegree, const long& logp);


	//----------------------------------------------------------------------------------
	//   FUNCTIONS
	//----------------------------------------------------------------------------------


	/**
	 * evaluates Z[i] value 1 / (xr + i * xi) << (2 * logp)
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value 1 / (xr + i * xi) << (2 * logp)
	 */
	static CZZ evalCZZInv(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value 1 / (xr + i * xi) << (2 * logp)
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value 1 / (xr + i * xi) << (2 * logp)
	 */
	static CZZ evalCZZInv(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z[i] value log(xr + i * xi) << logp
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value log(xr + i * xi) << logp
	 */
	static CZZ evalCZZLog(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value exp(xr + i * xi) << logp
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value exp(xr + i * xi) << logp
	 */
	static CZZ evalCZZExp(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value exp(xr + i * xi) << logp
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value exp(xr + i * xi) << logp
	 */
	static CZZ evalCZZExp(const RR& xr, const RR& xi, const long& logp);

	/**
	 * evaluates Z[i] value sigmoid(xr + i * xi) << logp
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value sigmoid(xr + i * xi) << logp
	 */
	static CZZ evalCZZSigmoid(const double& xr, const double& xi, const long& logp);

	/**
	 * evaluates Z[i] value sigmoid(xr + i * xi) << logp
	 * @param[in] xr: real part of input
	 * @param[in] xi: imaginary part of input
	 * @param[in] logp: log of precision
	 * @return Z[i] value sigmoid(xr + i * xi) << logp
	 */
	static CZZ evalCZZSigmoid(const RR& xr, const RR& xi, const long& logp);

	/**
	 * left shift array of values by bits
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] bits: shift bits
	 */
	static void leftShiftAndEqual(CZZ*& vals, const long& size, const long& bits);


	//----------------------------------------------------------------------------------
	//   ROTATIONS
	//----------------------------------------------------------------------------------


	/**
	 * left indexes rotation of values
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] rotSize: rotation size
	 */
	static void leftRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize);

	/**
	 * right indexes rotation of values
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] rotSize: rotation size
	 */
	static void rightRotateAndEqual(CZZ*& vals, const long& size, const long& rotSize);

};

#endif
