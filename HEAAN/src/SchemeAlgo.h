/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_SCHEMEALGO_H_
#define HEAAN_SCHEMEALGO_H_

#include <NTL/BasicThreadPool.h>
#include <NTL/ZZ.h>

#include "Common.h"
#include "EvaluatorUtils.h"
#include "Plaintext.h"
#include "SecretKey.h"
#include "Ciphertext.h"
#include "Scheme.h"

class SchemeAlgo {
public:
	Scheme& scheme;

	SchemeAlgo(Scheme& scheme) : scheme(scheme) {};


	//----------------------------------------------------------------------------------
	//   ARRAY ENCRYPTION & DECRYPTION
	//----------------------------------------------------------------------------------


	/**
	 * encrypting array of values, each to one ciphertext
	 * @param[in] vals: [m_1, m_2,...,m_size]
	 * @param[in] size: array size
	 * @return [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 */
	Ciphertext* encryptSingleArray(complex<double>* vals, long size, long logp);

	/**
	 * decrypting array of ciphertexts with single value encrypted in each
	 * @param[in] secretKey: secret key
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] size: array size
	 * @return [m_1, m_2,...,m_size]
	 */
	complex<double>* decryptSingleArray(SecretKey& secretKey, Ciphertext* ciphers, long size);


	//----------------------------------------------------------------------------------
	//   POWERS & PRODUCTS
	//----------------------------------------------------------------------------------


	/**
	 * Calculating power of 2 ciphertext
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] logp: log of precision
	 * @param[in] logDegree: log of degree
	 * @return ciphertext(m^degree)
	 */
	Ciphertext powerOf2(Ciphertext& cipher, const long precisionBits, const long logDegree);

	/**
	 * Calculating and storing all power of 2 ciphertexts up to degree, degree is a power-of-two
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] logp: log of precision
	 * @param[in] logDegree: log of degree
	 * @return [ciphertext(m), ciphertext(m^2), ciphertext(m^4), ... , ciphertext(m^degree)]
	 */
	Ciphertext* powerOf2Extended(Ciphertext& cipher, const long logp, const long logDegree);

	/**
	 * Calculating power of ciphertext
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] logp: log of precision
	 * @param[in] degree: power degree
	 * @return ciphertext(m^degree)
	 */
	Ciphertext power(Ciphertext& cipher, const long logp, const long degree);

	/**
	 * Calculating and storing powers of ciphertext up to degree
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] logp: log of precision
	 * @param[in] degree: power degree
	 * @return [ciphertext(m), ciphertext(m^2), ... , ciphertext(m^degree)]
	 */
	Ciphertext* powerExtended(Ciphertext& cipher, const long logp, const long degree);

	/**
	 * Calculating product of ciphertexts, degree is a power-of-two
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2), ... ,ciphertext(m_{degree})]
	 * @param[in] logp: log of precision
	 * @param[in] logDegree: log of degree
	 * @return ciphertext(m_1 * m_2 *...*m_{degree})
	 */
	Ciphertext prodOfPo2(Ciphertext* ciphers, const long logp, const long logDegree);

	/**
	 * Calculating product of ciphertexts
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2), ... ,ciphertext(m_{degree})]
	 * @param[in] logp: log of precision
	 * @param[in] degree: array size
	 * @return ciphertext(m_1 * m_2 *...*m_{degree})
	 */
	Ciphertext prod(Ciphertext* ciphers, const long logp, const long degree);


	//----------------------------------------------------------------------------------
	//   METHODS ON ARRAYS OF CIPHERTEXTS
	//----------------------------------------------------------------------------------


	/**
	 * Calculating sum of ciphertexts
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] size: array size
	 * @return ciphertext(m_1 + m_2 + ... + m_size)
	 */
	Ciphertext sum(Ciphertext* ciphers, const long size);

	/**
	 * Calculating distance of vectors encrypted in ciphertexts
	 * @param[in] cipher1: ciphertext(m_1, m_2, ..., m_slots)
	 * @param[in] cipher2: ciphertext(m'_1, m'_2, ..., m'_slots)
	 * @return ciphertext(sum((m_i-m'_i)^2) >> logp)
	 */
	Ciphertext distance(Ciphertext& cipher1, Ciphertext& cipher2, const long logp);

	/**
	 * Pairwise ciphertexts multiplication
	 * @param[in] ciphers1: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] ciphers2: [ciphertext(n_1), ciphertext(n_2),...,ciphertext(n_size)]
	 * @param[in] size: array size
	 * @return [ciphertext(m_1 * n_1), ciphertext(m_2 * n_2),...,ciphertext(m_size * n_size)]
	 */
	Ciphertext* multVec(Ciphertext* ciphers1, Ciphertext* ciphers2, const long size);

	/**
	 * Pairwise ciphertexts multiplication
	 * @param[in, out] ciphers1: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)] -> [ciphertext(m_1 * n_1), ciphertext(m_2 * n_2),...,ciphertext(m_size * n_size)]
	 * @param[in] ciphers2: [ciphertext(n_1), ciphertext(n_2),...,ciphertext(n_size)]
	 * @param[in] size: array size
	 */
	void multAndEqualVec(Ciphertext* ciphers1, Ciphertext* ciphers2, const long size);

	/**
	 * Pairwise ciphers multiplication and modulus switching
	 * Pairwise ciphertexts multiplication and modulus switching
	 * @param[in] ciphers1: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] ciphers2: [ciphertext(n_1), ciphertext(n_2),...,ciphertext(n_size)]
	 * @param[in] logp: log of precision
	 * @param[in] size: array size
	 * @return [ciphertext(m_1 * n_1 / p), ciphertext(m_2 * n_2 / p),...,ciphertext(m_size * n_size / p)]
	 */
	Ciphertext* multAndModSwitchVec(Ciphertext* ciphers1, Ciphertext* ciphers2, const long logp, const long size);

	/**
	 * Pairwise ciphertexts multiplication and modulus switching
	 * @param[in, out] ciphers1: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)] -> [ciphertext(m_1 * n_1 / p), ciphertext(m_2 * n_2 / p),...,ciphertext(m_size * n_size / p)]
	 * @param[in] ciphers2: [ciphertext(n_1), ciphertext(n_2),...,ciphertext(n_size)]
	 * @param[in] logp: log of precision
	 * @param[in] size: array size
	 */
	void multModSwitchAndEqualVec(Ciphertext* ciphers1, Ciphertext* ciphers2, const long logp, const long size);

	/**
	 * Calculating inner product of ciphertexts
	 * @param[in] ciphers1: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] ciphers2: [ciphertext(n_1), ciphertext(n_2),...,ciphertext(n_size)]
	 * @param[in] logp: log of precision
	 * @param[in] size: array size
	 * @return ciphertext(m_1 * n_1 + m_2 * n_2 + ... + m_size * n_size)
	 */
	Ciphertext innerProd(Ciphertext* ciphers1, Ciphertext* ciphers2, const long logp, const long size);

	/**
	 * Calculating ciphertext of partial sums
	 * @param[in] cipher: ciphertext(m_1, m_2,..., m_size)
	 * @param[in] slots: number of slots summed in partial sums
	 * @return ciphertext(m_1 + ... + m_slots, m_2 + ... + m_{slots + 1},...,m_size + m_1 + ... + m_{slots - 1})
	 *
	 */
	Ciphertext partialSlotsSum(Ciphertext& cipher, const long slots);

	/**
	 * Calculating ciphertext of partial sums
	 * @param[in, out] cipher: ciphertext(m_1, m_2,..., m_size) -> ciphertext(m_1 + ... + m_slots, m_2 + ... + m_{slots + 1},...,m_size + m_1 + ... + m_{slots - 1})
	 * @param[in] slots: number of slots summed in partial sums
	 */
	void partialSlotsSumAndEqual(Ciphertext& cipher, const long slots);


	//----------------------------------------------------------------------------------
	//   FUNCTIONS
	//----------------------------------------------------------------------------------


	/**
	 * Calculating inverse of a ciphertext, using steps number of approximations
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] logp: log of precision
	 * @param[in] steps: number of steps in approximation
	 * @return cipher(1 / m << (2 * logp))
	 */
	Ciphertext inverse(Ciphertext& cipher, long logp, long steps);

	/**
	 * Calculating and storing inverse of a ciphertext at each step up to steps
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] logp: log of precision
	 * @param[in] steps: number of steps in approximation
	 * @return [cipher(1 / m << (2 * logp)), ... ,cipher(1 / m << (2 * logp))]
	 */
	Ciphertext* inverseExtended(Ciphertext& cipher, const long logp, const long steps);

	/**
	 * Calculating function using Taylor Series approximation
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] funcName: name of a function
	 * @param[in] logp: log of precision
	 * @param[in] degree: taylor approximation degree
	 * @return ciphertext(funcName(m))
	 */
	Ciphertext function(Ciphertext& cipher, string& funcName, const long logp, const long degree);

	/**
	 * Calculating function using Taylor Series approximation
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] funcName: name of a function
	 * @param[in] logp: log of precision
	 * @param[in] degree: taylor approximation degree
	 * @return ciphertext(funcName(m) * p), but saves level
	 */
	Ciphertext functionLazy(Ciphertext& cipher, string& funcName, const long logp, const long degree);

	/**
	 * Calculating function using Taylor Series approximation, and storing intermediate results
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] funcName: name of a function
	 * @param[in] logp: log of precision
	 * @param[in] degree: taylor approximation degree
	 * @return [ciphertext(funcName(m)), ... ,ciphertext(funcName(m))]
	 */
	Ciphertext* functionExtended(Ciphertext& cipher, string& funcName, const long logp, const long degree);


	//----------------------------------------------------------------------------------
	//   FFT & FFT INVERSE
	//----------------------------------------------------------------------------------


	/**
	 * Calculating pre fft of ciphertexts
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] size: array size (power-of-two)
	 * @param[in] isForward: switching between fft and fft inverse
	 * @return [ciphertext(fft_1), ... ,ciphertext(fft_size)]
	 */
	void fftRaw(Ciphertext* ciphers, const long size, const bool isForward);

	/**
	 * Calculating fft of ciphertexts
	 * @param[in] [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @return [ciphertext(fft_1), ... ,ciphertext(fft_size)]
	 */
	void DFT(Ciphertext* ciphers, const long size);

	/**
	 * Calculating fft inverse of ciphertexts
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] size: array size (power-of-two)
	 * @return [ciphertext(fftinv_1), ... ,ciphertext(fftinv_size)]
	 */
	void IDFT(Ciphertext* ciphers, const long size);

	/**
	 * Calculating fft inverse of ciphertexts
	 * @param[in] ciphers: [ciphertext(m_1), ciphertext(m_2),...,ciphertext(m_size)]
	 * @param[in] size: array size (power-of-two)
	 * @return [ciphertext(fftinv_1 * size), ... ,ciphertext(fftinv_size * size)] but saves level
	 */
	void IDFTLazy(Ciphertext* ciphers, const long size);

};

#endif
