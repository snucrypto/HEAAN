#ifndef SCHEME_SCHEMEALGO_H_
#define SCHEME_SCHEMEALGO_H_

#include <iostream>

#include "Cipher.h"
#include "Scheme.h"

class SchemeAlgo {
public:
	Scheme& scheme;

	//-----------------------------------------

	SchemeAlgo(Scheme& scheme) : scheme(scheme) {};

	//-----------------------------------------

	/**
	 * Calculating power of 2 cipher
	 * @param[in] cipher(m)
	 * @param[in] logdeg
	 * @return cipher(m^2^logdeg)
	 */
	Cipher powerOf2(Cipher& cipher, const long& logDegree);

	/**
	 * Calculating and storing all power of 2 ciphers up to 2^logdeg
	 * @param[in] cipher(m)
	 * @param[in] logdeg
	 * @return [cipher(m), cipher(m^2), cipher(m^4), ... , cipher(m^2^logdeg)]
	 */
	Cipher* powerOf2Extended(Cipher& cipher, const long& logDegree);

	//-----------------------------------------

	/**
	 * Calculating power of cipher
	 * @param[in] cipher(m)
	 * @param[in] deg
	 * @return cipher(m^deg)
	 */
	Cipher power(Cipher& cipher, const long& degree);
	/**
	 * Calculating and storing powers of cipher up to deg
	 * @param[in] cipher(m)
	 * @param[in] deg
	 * @return [cipher(m), cipher(m^2), ... , cipher(m^deg)]
	 */
	Cipher* powerExtended(Cipher& cipher, const long& degree);

	//-----------------------------------------

	/**
	 * Calculating product of ciphers, number of ciphers here is power of 2
	 * @param[in] [cipher(m_1), cipher(m_2), ... ,cipher(m_{2^logdeg})]
	 * @param[in] logdeg
	 * @return cipher(m_1 * m_2 *...*m_{2^logdeg})
	 */
	Cipher prod2(Cipher*& ciphers, const long& logDegree);

	//-----------------------------------------

	/**
	 * Calculating sum of ciphers
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] size
	 * @return cipher(m_1 + m_2 + ... + m_size)
	 */
	Cipher sum(Cipher*& ciphers, const long& size);

	Cipher delta(Cipher& cipher1, Cipher& cipher2);
	//-----------------------------------------

	/**
	 * Pairwise ciphers multiplication
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] [cipher(n_1), cipher(n_2),...,cipher(n_size)]
	 * @param[in] size
	 * @return [cipher(m_1 * n_1), cipher(m_2 * n_2),...,cipher(m_size * n_size)]
	 */
	Cipher* multVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	/**
	 * Pairwise ciphers multiplication
	 * @param[in, out] [cipher(m_1), cipher(m_2),...,cipher(m_size)] -> [cipher(m_1 * n_1), cipher(m_2 * n_2),...,cipher(m_size * n_size)]
	 * @param[in] [cipher(n_1), cipher(n_2),...,cipher(n_size)]
	 * @param[in] size
	 */
	void multAndEqualVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	/**
	 * Pairwise ciphers multiplication and modulus switching
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] [cipher(n_1), cipher(n_2),...,cipher(n_size)]
	 * @param[in] size
	 * @return [cipher(m_1 * n_1 / p), cipher(m_2 * n_2 / p),...,cipher(m_size * n_size / p)] one level higher
	 */
	Cipher* multAndModSwitchVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	/**
	 * Pairwise ciphers multiplication and modulus switching
	 * @param[in, out] [cipher(m_1), cipher(m_2),...,cipher(m_size)] -> [cipher(m_1 * n_1 / p), cipher(m_2 * n_2 / p),...,cipher(m_size * n_size / p)]
	 * @param[in] [cipher(n_1), cipher(n_2),...,cipher(n_size)]
	 * @param[in] size
	 */
	void multModSwitchAndEqualVec(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	//-----------------------------------------

	/**
	 * encrypting array of vals, each to one cipher
	 * @param[in] [m_1, m_2,...,m_size]
	 * @param[in] size
	 * @return [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 */
	Cipher* encryptSingleArray(CZZ*& vals, long size);

	/**
	 * decrypting array of ciphers with single val encrypted in each
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] size
	 * @return [m_1, m_2,...,m_size]
	 */
	CZZ* decryptSingleArray(SecKey& secretKey, Cipher*& ciphers, long size);

	//-----------------------------------------

	/**
	 * Calculating inner product of ciphers
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] [cipher(n_1), cipher(n_2),...,cipher(n_size)]
	 * @param[in] size
	 * @return cipher(m_1 * n_1 + m_2 * n_2 + ... + m_size * n_size)
	 */
	Cipher innerProd(Cipher*& ciphers1, Cipher*& ciphers2, const long& size);

	//-----------------------------------------

	/**
	 * Calculating inverse of a cipher, using steps number of approximations
	 * @param[in] cipher(m)
	 * @param[in] steps
	 * @return cipher(p^2 / m)
	 */
	Cipher inverse(Cipher& cipher, const long& stepsNum);

	/**
	 * Calculating and storing inverse of a cipher at each step up to stepsNum
	 * @param[in] cipher(m)
	 * @param[in] steps
	 * @return [cipher(p^2 / m), ... ,cipher(p^2 / m)]
	 */
	Cipher* inverseExtended(Cipher& cipher, const long& stepsNum);

	//-----------------------------------------

	/**
	 * Calculating function using Taylor Series approximation, more information in SchemeAux
	 * @param[in] cipher(m)
	 * @param[in] funcName
	 * @param[in] degee
	 * @return cipher(funcName(m))
	 */
	Cipher function(Cipher& cipher, string& funcName, const long& degree);

	/**
	 * Calculating function using Taylor Series approximation, more information in SchemeAux
	 * @param[in] cipher(m)
	 * @param[in] funcName
	 * @param[in] degee
	 * @return cipher(funcName(m) * p), but saves one level
	 */
	Cipher functionLazy(Cipher& cipher, string& funcName, const long& degree);

	/**
	 * Calculating function using Taylor Series approximation, and storing intermediate results
	 * @param[in] cipher(m)
	 * @param[in] funcName
	 * @param[in] degee
	 * @return [cipher(funcName(m)), ... ,cipher(funcName(m))]
	 */
	Cipher* functionExtended(Cipher& cipher, string& funcName, const long& degree);

	//-----------------------------------------

	/**
	 * Calculating fft of ciphers
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] size is a power of 2
	 * @param[in] boolean is forward?
	 * @return [cipher(fft_1), ... ,cipher(fft_size)]
	 */
	Cipher* fftRaw(Cipher*& ciphers, const long& size, const bool& isForward);

	/**
	 * Calculating fft of ciphers
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] size is a power of 2
	 * @return [cipher(fft_1), ... ,cipher(fft_size)]
	 */
	Cipher* fft(Cipher*& ciphers, const long& size);

	/**
	 * Calculating fft inverse of ciphers
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] size is a power of 2
	 * @return [cipher(fftinv_1), ... ,cipher(fftinv_size)]
	 */
	Cipher* fftInv(Cipher*& ciphers, const long& size);

	/**
	 * Calculating fft inverse of ciphers
	 * @param[in] [cipher(m_1), cipher(m_2),...,cipher(m_size)]
	 * @param[in] size is a power of 2
	 * @return [cipher(fftinv_1 * size), ... ,cipher(fftinv_size * size)] but saves level
	 */
	Cipher* fftInvLazy(Cipher*& ciphers, const long& size);

	/**
	 * auxiliary operation for multithreading
	 */
	void butOperation(Cipher& res1, Cipher& res2, Cipher& y1, Cipher& y2, long shift);

	/**
	 * auxiliary operation for multithreading
	 */
	void rescale(Cipher& c, long& bits);

	//-----------------------------------------

	Cipher partialSlotsSum(Cipher& cipher, const long& slots);
	void partialSlotsSumAndEqual(Cipher& cipher, const long& slots);

	//-----------------------------------------

};

#endif /* SCHEME_SCHEMEALGO_H_ */
