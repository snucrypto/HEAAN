#ifndef UTILS_NUMUTILS_H_
#define UTILS_NUMUTILS_H_

#include <NTL/ZZX.h>

#include "../czz/CZZ.h"

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
	 * samples polynomial with random {1, 0, -1} coefficients
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 */
	static void sampleZO(ZZX& res, const long& size);


	/**
	 * samples polynomial with random uniform coefficients in [0, 2^logBnd-1]
	 * @param[out] ZZX polynomial
	 * @param[in] long polynomial degree
	 * @param[in] long log(bound)
	 */
	static void sampleUniform2(ZZX& res, const long& size, const long& logBnd);

	//-----------------------------------------

	static CZZ* fftRaw(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp, const bool& isForward);

	/**
	 * calculates fft in Z_q[X] / (X^N + 1)
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return fft
	 */
	static CZZ* fft(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);

	/**
	 * calculates fft inverse in Z_q[X] / (X^N + 1)
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return fft inverse
	 */
	static CZZ* fftInv(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);

	/**
	 * calculates fft inverse without last division by size in Z_q[X] / (X^N + 1)
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return fft inverse without last division by size
	 */
	static CZZ* fftInvLazy(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);

	/**
	 * calculates full fft flow in Z_q[X] / (X^N + 1)
	 * @param[in] arrays of vals
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return full fft flow
	 */
	static CZZ* fftFull(CZZ*& vals1, CZZ*& vals2, const long& size, CZZ**& ksiPows, const long& logp);

	/**
	 * calculates full fft flow without last division by size in Z_q[X] / (X^N + 1)
	 * @param[in] arrays of vals
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return full fft flow without last division by size
	 */
	static CZZ* fftFullLazy(CZZ*& vals1, CZZ*& vals2, const long& size, CZZ**& ksiPows, const long& logp);

	//-----------------------------------------

	/**
	 * calculates special fft in Z_q[X] / (X^N + 1) needed for encoding/decoding
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return special fft
	 */
	static CZZ* fftSpecial(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);

	/**
	 * calculates special fft inverse in Z_q[X] / (X^N + 1) needed for encoding/decoding
	 * @param[in] arrays of vals
	 * @param[in] size of array
	 * @param[in] auxiliary information
	 * @param[in] auxiliary information
	 * @return special fft inverse
	 */
	static CZZ* fftSpecialInv(CZZ*& vals, const long& size, CZZ**& ksiPows, const long& logp);

	//-----------------------------------------
};

#endif /* UTILS_NUMUTILS_H_ */
