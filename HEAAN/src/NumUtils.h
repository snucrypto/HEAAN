#ifndef UTILS_NUMUTILS_H_
#define UTILS_NUMUTILS_H_

#include <NTL/ZZX.h>
#include <NTL/RR.h>
#include "SchemeAux.h"
#include "CZZ.h"

using namespace NTL;

class NumUtils {
public:

	//-----------------------------------------

	/**
	 * samples polynomial with random Gaussians coefficients
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 * @param[in] standard deviation
	 */
	static void sampleGauss(ZZX& res, const long& size, const double& stdev);

	/**
	 * samples polynomial with random {-1,0,1} coefficients
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 * @param[in] number of nonzero coefficients
	 */
	static void sampleHWT(ZZX& res, const long& size, const long& h);

	/**
	 * samples polynomial with random {-1,0,1} coefficients
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 */
	static void sampleZO(ZZX& res, const long& size);

	/**
	 * samples polynomial with random {0,1} coefficients
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 * @param[in] number of nonzero coefficients
	 */
	static void sampleBinary(ZZX& res, const long& size, const long& h);

	/**
	 * samples polynomial with random {0,1} coefficients
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 */
	static void sampleBinary(ZZX& res, const long& size);

	/**
	 * samples polynomial with random uniform coefficients in [0, 2^logBnd-1]
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 * @param[in] long log(bound)
	 */
	static void sampleUniform2(ZZX& res, const long& size, const long& logBnd);

	//-----------------------------------------

	/**
	 * calculates pre fft in Z_q[X] / (X^N + 1)
	 * @param[in, out] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return pre fft
	 */
	static void fftRaw(CZZ*& vals, const long& size, SchemeAux& aux, const bool& isForward);

	/**
	 * calculates fft in Z_q[X] / (X^N + 1)
	 * @param[in, out] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 */
	static void fft(CZZ*& vals, const long& size, SchemeAux& aux);

	/**
	 * calculates fft inverse in Z_q[X] / (X^N + 1)
	 * @param[in, out] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 */
	static void fftInv(CZZ*& vals, const long& size, SchemeAux& aux);

	/**
	 * calculates fft inverse without last division by size in Z_q[X] / (X^N + 1)
	 * @param[in, out] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 */
	static void fftInvLazy(CZZ*& vals, const long& size, SchemeAux& aux);

	//-----------------------------------------

	/**
	 * calculates special fft in Z_q[X] / (X^N + 1) needed for encoding/decoding
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 */
	static void fftSpecial(CZZ*& vals, const long& size, SchemeAux& aux);

	/**
	 * calculates special fft inverse in Z_q[X] / (X^N + 1) needed for encoding/decoding
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 */
	static void fftSpecialInv(CZZ*& vals, const long& size, SchemeAux& aux);

	//-----------------------------------------
};

#endif /* UTILS_NUMUTILS_H_ */
