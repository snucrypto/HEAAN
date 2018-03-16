/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_RING2UTILS_H_
#define HEAAN_RING2UTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

using namespace NTL;

class Ring2Utils {
public:


	//----------------------------------------------------------------------------------
	//   MODULUS
	//----------------------------------------------------------------------------------


	/**
	 * modulus function
	 * @param[out] p in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void mod(ZZX& res, ZZX& p, ZZ& mod, const long degree);

	/**
	 * modulus function
	 * @param[in, out] p in Z[X] / (X^N + 1) -> p in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void modAndEqual(ZZX& p, ZZ& mod, const long degree);


	//----------------------------------------------------------------------------------
	//   ADDITION & SUBSTRACTION
	//----------------------------------------------------------------------------------


	/**
	 * addition in ring Z_q[X] / (X^N + 1)
	 * @param[out] p1 + p2 in Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void add(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * addition in ring Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 * @return p1 + p2 in Z_q[X] / (X^N + 1)
	 */
	static ZZX add(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * addition in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p1 -> p1 + p2 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void addAndEqual(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * substraction in ring Z_q[X] / (X^N + 1)
	 * @param[out] p1 - p2 in Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void sub(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * substraction in ring Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 * @result p1 - p2 in Z_q[X] / (X^N + 1)
	 */
	static ZZX sub(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * substraction in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p1 -> p1 - p2 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void subAndEqual(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * substraction in ring Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in, out] p2 -> p1 - p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void subAndEqual2(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);


	//----------------------------------------------------------------------------------
	//   MULTIPLICATION & SQUARING
	//----------------------------------------------------------------------------------


	/**
	 * multiplication in ring Z_q[X] / (X^N + 1)
	 * @param[out] p1 * p2 in Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void mult(ZZX& res, ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * multiplication in ring Z_q[X] / (X^N + 1)
	 * @param[in] p1 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 * @result p1 * p2 in Z_q[X] / (X^N + 1)
	 */
	static ZZX mult(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * multiplication in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p1 -> p1 * p2 in Z_q[X] / (X^N + 1)
	 * @param[in] p2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void multAndEqual(ZZX& p1, ZZX& p2, ZZ& mod, const long degree);

	/**
	 * square in ring Z_q[X] / (X^N + 1)
	 * @param[out] p^2 in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void square(ZZX& res, ZZX& p, ZZ& mod, const long degree);

	/**
	 * square in ring Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 * @result p^2 in Z_q[X] / (X^N + 1)
	 */
	static ZZX square(ZZX& p, ZZ& mod, const long degree);

	/**
	 * square in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p -> p^2 in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void squareAndEqual(ZZX& p, ZZ& mod, const long degree);

	/**
	 * multiplication by monomial in ring Z_q[X] / (X^N + 1)
	 * @param[out] p * X^d in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] monomial degree d
	 * @param[in] degree N
	 */
	static void multByMonomial(ZZX& res, ZZX& p, const long monomialDeg, const long degree);

	/**
	 * multiplication by monomial in ring Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] monomial degree d
	 * @param[in] degree N
	 * @result p * X^d in Z_q[X] / (X^N + 1)
	 */
	static ZZX multByMonomial(ZZX& p, const long monomialDeg, const long degree);

	/**
	 * multiplication by monomial in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p -> p * X^d in Z_q[X] / (X^N + 1)
	 * @param[in] monomial degree d
	 * @param[in] degree N
	 */
	static void multByMonomialAndEqual(ZZX& p, const long monomialDeg, const long degree);

	/**
	 * multiplication by constant in ring Z_q[X] / (X^N + 1)
	 * @param[out] p * c in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] constant c
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void multByConst(ZZX& res, ZZX& p, const ZZ& cnst, ZZ& mod, const long degree);

	/**
	 * multiplication by constant in ring Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] constant c
	 * @param[in] mod q
	 * @param[in] degree N
	 * @result p * c in Z_q[X] / (X^N + 1)
	 */
	static ZZX multByConst(ZZX& p, const ZZ& cnst, ZZ& mod, const long degree);

	/**
	 * multiplication by constant in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p -> p * c in Z_q[X] / (X^N + 1)
	 * @param[in] constant c
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void multByConstAndEqual(ZZX& p, const ZZ& cnst, ZZ& mod, const long degree);

	/**
	 * multiplication by 2^b in ring Z_q[X] / (X^N + 1)
	 * @param[out] p * 2^b in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] degree b
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void leftShift(ZZX& res, ZZX& p, const long bits, ZZ& mod, const long degree);

	/**
	 * multiplication by 2^b in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p -> p * 2^b in Z_q[X] / (X^N + 1)
	 * @param[in] degree b
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void leftShiftAndEqual(ZZX& p, const long bits, ZZ& mod, const long degree);

	/**
	 * multiplication by 2 in ring Z_q[X] / (X^N + 1)
	 * @param[in, out] p -> 2p in Z_q[X] / (X^N + 1)
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void doubleAndEqual(ZZX& p, ZZ& mod, const long degree);

	/**
	 * division by 2^b in ring Z_q[X] / (X^N + 1)
	 * @param[out] p / 2^b in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] degree b
	 * @param[in] degree N
	 */
	static void rightShift(ZZX& res, ZZX& p, const long bits, const long degree);

	/**
	 * division by 2^b in ring Z_q[X] / (X^N + 1)
	 * @param[in,out] p -> p / 2^b in Z_q[X] / (X^N + 1)
	 * @param[in] degree b
	 * @param[in] degree N
	 */
	static void rightShiftAndEqual(ZZX& p, const long bits, const long degree);


	//----------------------------------------------------------------------------------
	//   CONJUGATION ROTATION AND OTHER
	//----------------------------------------------------------------------------------


	/**
	 * conjugation
	 * @param[out] conj(p) in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] degree N
	 */
	static void conjugate(ZZX& res, ZZX& p, const long degree);

	/**
	 * conjugation
	 * @param[in, out] p -> conj(p) in Z_q[X] / (X^N + 1)
	 * @param[in] p in Z_q[X] / (X^N + 1)
	 * @param[in] degree N
	 */
	static void conjugateAndEqual(ZZX& p, const long degree);

	/**
	 * changing p(X) to p(X^pow) in Z_q[X] / (X^N + 1)
	 * @param[out] p(X^pow) in Z_q[X] / (X^N + 1)
	 * @param[in] p(X) in Z_q[X] / (X^N + 1)
	 * @param[in] power pow
	 * @param[in] mod q
	 * @param[in] degree N
	 */
	static void inpower(ZZX& res, ZZX& p, const long pow, ZZ& mod, const long degree);

	/**
	 * changing p(X) to p(X^pow) in Z_q[X] / (X^N + 1)
	 * @param[in] p(X) in Z_q[X] / (X^N + 1)
	 * @param[in] power pow
	 * @param[in] mod q
	 * @param[in] degree N
	 * @result p(X^pow) in Z_q[X] / (X^N + 1)
	 */
	static ZZX inpower(ZZX& p, const long pow, ZZ& mod, const long degree);

};

#endif
