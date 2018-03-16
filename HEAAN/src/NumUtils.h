/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_NUMUTILS_H_
#define HEAAN_NUMUTILS_H_

#include <NTL/ZZX.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>

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

};

#endif
