#ifndef HEAAN_NUMUTILS_H_
#define HEAAN_NUMUTILS_H_

#include <NTL/ZZX.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include "CZZ.h"

#include "Common.h"
using namespace NTL;

class NumUtils {
public:


	//----------------------------------------------------------------------------------
	//   SAMPLING
	//----------------------------------------------------------------------------------


	/**
	 * samples polynomial with random Gaussians coefficients
	 * @param[out] res: ZZX polynomial
	 * @param[in] size: polynomial degree
	 * @param[in] stdev: standard deviation
	 */
	static void sampleGauss(ZZX& res, const long size, const double stdev);

	/**
	 * samples polynomial with random {-1,0,1} coefficients
	 * @param[out] res: ZZX polynomial
	 * @param[in] size: polynomial degree
	 * @param[in] h: number of nonzero coefficients
	 */
	static void sampleHWT(ZZX& res, const long size, const long h);

	/**
	 * samples polynomial with random {-1,0,1} coefficients
	 * @param[out] res: ZZX polynomial
	 * @param[in] size: polynomial degree
	 */
	static void sampleZO(ZZX& res, const long size);

	/**
	 * samples polynomial with random {0,1} coefficients
	 * @param[out] res: ZZX polynomial
	 * @param[in] size: polynomial degree
	 * @param[in] h: number of nonzero coefficients
	 */
	static void sampleBinary(ZZX& res, const long size, const long h);

	/**
	 * samples polynomial with random {0,1} coefficients
	 * @param[out] res: ZZX polynomial
	 * @param[in] size: polynomial degree
	 */
	static void sampleBinary(ZZX& res, const long size);

	/**
	 * samples polynomial with random uniform coefficients in [0, 2^bits-1]
	 * @param[out] res: ZZX polynomial
	 * @param[in] size: polynomial degree
	 * @param[in] bits: number of bits
	 */
	static void sampleUniform2(ZZX& res, const long size, const long bits);


	//----------------------------------------------------------------------------------
	//   FFT & FFT INVERSE
	//----------------------------------------------------------------------------------


	/**
	 * calculates pre fft in Z_Q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] ksiPowsr: precomputed powers of units of unity
	 * @param[in] ksiPowsi: precomputed powers of units of unity
	 * @param[in] M: M = 2N
	 */
	static void fftRaw(CZZ* vals, const long size, const RR* ksiPowsr, const RR* ksiPowsi, const long M, const bool isForward);

	/**
	 * calculates fft in Z_q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] ksiPowsr: precomputed powers of units of unity
	 * @param[in] ksiPowsi: precomputed powers of units of unity
	 * @param[in] M: M = 2N
	 */
	static void fft(CZZ* vals, const long size, const RR* ksiPowsr, const RR* ksiPowsi, const long M);

	/**
	 * calculates fft inverse in Z_q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] ksiPowsr: precomputed powers of units of unity
	 * @param[in] ksiPowsi: precomputed powers of units of unity
	 * @param[in] M: M = 2N
	 */
	static void fftInv(CZZ* vals, const long size, const RR* ksiPowsr, const RR* ksiPowsi, const long M);

	/**
	 * calculates fft inverse without last division by size in Z_q[X] / (X^N + 1)
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] ksiPowsr: precomputed powers of units of unity
	 * @param[in] ksiPowsi: precomputed powers of units of unity
	 * @param[in] M: M = 2N
	 */
	static void fftInvLazy(CZZ* vals, const long size, const RR* ksiPowsr, const RR* ksiPowsi, const long M);

	//-----------------------------------------

	/**
	 * calculates special fft in Z_q[X] / (X^N + 1) for encoding/decoding
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] ksiPowsr: precomputed powers of units of unity
	 * @param[in] ksiPowsi: precomputed powers of units of unity
	 * @param[in] M: M = 2N
	 */
	static void fftSpecial(CZZ* vals, const long size, const RR* ksiPowsr, const RR* ksiPowsi, const long M);

	/**
	 * calculates special fft inverse in Z_q[X] / (X^N + 1) for encoding/decoding
	 * @param[in, out] vals: array of values
	 * @param[in] size: array size
	 * @param[in] ksiPowsr: precomputed powers of units of unity
	 * @param[in] ksiPowsi: precomputed powers of units of unity
	 * @param[in] M: M = 2N
	 */
	static void fftSpecialInv(CZZ* vals, const long size, const RR* ksiPowsr, const RR* ksiPowsi, const long M);

	//-----------------------------------------
};

#endif
