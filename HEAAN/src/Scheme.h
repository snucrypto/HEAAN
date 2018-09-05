/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_SCHEME_H_
#define HEAAN_SCHEME_H_

#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <complex>

#include "BootContext.h"
#include "Common.h"
#include "SecretKey.h"
#include "Ciphertext.h"
#include "Plaintext.h"
#include "Key.h"
#include "EvaluatorUtils.h"
#include "Ring.h"

using namespace std;
using namespace NTL;

static long ENCRYPTION = 0;
static long MULTIPLICATION  = 1;
static long CONJUGATION = 2;

class Scheme {
private:
public:
	Ring& ring;

	map<long, Key> keyMap; ///< contain Encryption, Multiplication and Conjugation keys, if generated
	map<long, Key> leftRotKeyMap; ///< contain left rotation keys, if generated
	map<long, BootContext> bootKeyMap;

	Scheme(SecretKey& secretKey, Ring& ring);

	//----------------------------------------------------------------------------------
	//   KEYS GENERATION
	//----------------------------------------------------------------------------------


	void addEncKey(SecretKey& secretKey);

	void addMultKey(SecretKey& secretKey);

	void addConjKey(SecretKey& secretKey);

	void addLeftRotKey(SecretKey& secretKey, long r);

	void addRightRotKey(SecretKey& secretKey, long r);

	void addLeftRotKeys(SecretKey& secretKey);

	void addRightRotKeys(SecretKey& secretKey);

	void addBootKey(SecretKey& secretKey, long logl, long logp);


	//----------------------------------------------------------------------------------
	//   ENCODING & DECODING
	//----------------------------------------------------------------------------------


	/**
	 * encodes an array of CZZ values into a ZZX polynomial using special fft inverse
	 * @param[in] vals: array of values
	 * @param[in] slots: size of an array
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return message
	 */
	Plaintext encode(complex<double>* vals, long slots, long logp, long logq);

	Plaintext encode(double* vals, long slots, long logp, long logq);

	/**
	 * decodes a ZZX polynomial into an array of CZZ values using special fft
	 * @param[in] msg: message
	 * @return array of CZZ values
	 */
	complex<double>* decode(Plaintext& msg);

	/**
	 * encodes a single CZZ value into a ZZX polynomial using special fft inverse
	 * @param[in] val: CZZ value
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return message
	 */
	Plaintext encodeSingle(complex<double> val, long logp, long logq);

	Plaintext encodeSingle(double val, long logp, long logq);

	/**
	 * decodes a ZZX polynomial into a single CZZ value using special fft
	 * @param[in] msg: message
	 * @return CZZ value
	 */
	complex<double> decodeSingle(Plaintext& msg);


	//----------------------------------------------------------------------------------
	//   ENCRYPTION & DECRYPTION
	//----------------------------------------------------------------------------------


	/**
	 * encrypts message into ciphertext using public key encyption
	 * @param[in] msg: message
	 * @return ciphertext
	 */
	Ciphertext encryptMsg(Plaintext& msg);

	/**
	 * decrypts ciphertext into message
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return message
	 */
	Plaintext decryptMsg(SecretKey& secretKey, Ciphertext& cipher);

	/**
	 * encodes array of CZZ into message and then encrypts it into ciphertext using public key encyption
	 * @param[in] vals: array of CZZ values
	 * @param[in] slots: array size
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return ciphertext
	 */
	Ciphertext encrypt(complex<double>* vals, long slots, long logp, long logq);

	Ciphertext encrypt(double* vals, long slots, long logp, long logq);

	/**
	 * decrypts ciphertext into message and then decodes it into array of CZZ values
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return array of CZZ values
	 */
	complex<double>* decrypt(SecretKey& secretKey, Ciphertext& cipher);

	/**
	 * encodes single CZZ value into a message and then encrypts it into a ciphertext using public key encyption
	 * @param[in] val: CZZ value
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return ciphertext
	 */
	Ciphertext encryptSingle(complex<double> val, long logp, long logq);

	Ciphertext encryptSingle(double val, long logp, long logq);

	/**
	 * decrypts ciphertext into message and then decodes it into a single CZZ value
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return CZZ value
	 */
	complex<double> decryptSingle(SecretKey& secretKey, Ciphertext& cipher);


	//----------------------------------------------------------------------------------
	//   HOMOMORPHIC OPERATIONS
	//----------------------------------------------------------------------------------

	/**
	 * negate the ciphertext
	 * @param[in] cipher: ciphertext(m)
	 * @return ciphertext(-m)
	 */
	Ciphertext negate(Ciphertext& cipher);

	/**
	 * negate the ciphertext
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(-m)
	 */
	void negateAndEqual(Ciphertext& cipher);

	/**
	 * addition of ciphertexts
	 * @param[in] cipher1: ciphertext(m1)
	 * @param[in] cipher2: ciphertext(m2)
	 * @return ciphertext(m1 + m2)
	 */
	Ciphertext add(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * addition of ciphertexts
	 * @param[in, out] cipher1: ciphertext(m1) -> ciphertext(m1 + m2)
	 * @param[in] cipher2: ciphertext(m2)
	 */
	void addAndEqual(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * constant addition
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @return ciphertext(m + constant)
	 */
	Ciphertext addConst(Ciphertext& cipher, double cnst, long logp);

	Ciphertext addConst(Ciphertext& cipher, RR& cnst, long logp);

	Ciphertext addConst(Ciphertext& cipher, complex<double> cnst, long logp);

	/**
	 * constant addition
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m + constant)
	 * @param[in] cnst: constant
	 */
	void addConstAndEqual(Ciphertext& cipher, double cnst, long logp);

	void addConstAndEqual(Ciphertext& cipher, RR& cnst, long logp);

	void addConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp);

	/**
	 * substraction of ciphertexts
	 * @param[in] cipher1: ciphertext(m1)
	 * @param[in] cipher2: ciphertext(m2)
	 * @return ciphertext(m1 - m2)
	 */
	Ciphertext sub(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * substraction of ciphertexts
	 * @param[in, out] cipher1: ciphertext(m1) -> ciphertext(m1 - m2)
	 * @param[in] cipher2: ciphertext(m2)
	 */
	void subAndEqual(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * substraction of ciphertexts
	 * @param[in] cipher1: ciphertext(m1)
	 * @param[in, out] cipher2: ciphertext(m2) -> ciphertext(m1 - m2)
	 */
	void subAndEqual2(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * multiplication by i (imaginary unit) in ciphertext
	 * @param[in] cipher: ciphertext(m)
	 * @return ciphertext(i * m)
	 */
	Ciphertext imult(Ciphertext& cipher);

	Ciphertext idiv(Ciphertext& cipher);

	/**
	 * multiplication by i (imaginary unit) in ciphertext
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(i * m)
	 */
	void imultAndEqual(Ciphertext& cipher);

	void idivAndEqual(Ciphertext& cipher);

	/**
	 * multiplication of ciphertexts. This algorithm contain relinearization.
	 * To manage the noise we usually need rescaling method after multiplication
	 * @param[in] cipher1: ciphertext(m1)
	 * @param[in] cipher2: ciphertext(m2)
	 * @return ciphertext(m1 * m2)
	 */
	Ciphertext mult(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * multiplication of ciphertexts. This algorithm contain relinearization.
	 * To manage the noise we usually need rescaling method after multiplication
	 * @param[in, out] cipher1: ciphertext(m1) -> ciphertext(m1 * m2)
	 * @param[in] cipher2: ciphertext(m2)
	 */
	void multAndEqual(Ciphertext& cipher1, Ciphertext& cipher2);

	/**
	 * squaring a ciphertext. This algorithm contain relinearization.
	 * To manage the noise we usually need resclaing method after squaring
	 * @param[in] cipher: ciphertext(m)
	 * @return ciphertext(m^2)
	 */
	Ciphertext square(Ciphertext& cipher);

	/**
	 * squaring a ciphertext. This algorithm contain relinearization.
	 * To manage the noise we usually need resclaing method after squaring
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m^2)
	 */
	void squareAndEqual(Ciphertext& cipher);

	/**
	 * constant multiplication
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @return ciphertext(m * constant)
	 */
	Ciphertext multByConst(Ciphertext& cipher, double cnst, long logp);

	Ciphertext multByConst(Ciphertext& cipher, complex<double> cnst, long logp);

	Ciphertext multByConstVec(Ciphertext& cipher, complex<double>* cnstVec, long logp);

	/**
	 * constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * constant)
	 * @param[in] cnst: constant
	 */
	void multByConstAndEqual(Ciphertext& cipher, double cnst, long logp);

	void multByConstAndEqual(Ciphertext& cipher, RR& cnst, long logp);

	void multByConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp);

	/**
	 * polynomial multiplication
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] poly: polynomial, encoding (constant)
	 * @return ciphertext(m * constant)
	 */
	Ciphertext multByPoly(Ciphertext& cipher, ZZ* poly, long logp);

	Ciphertext multByPolyNTT(Ciphertext& cipher, uint64_t* rpoly, long bnd, long logp);

	/**
	 * polynomial multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * constant)
	 * @param[in] poly: polynomial, encoding (constant)
	 */
	void multByPolyAndEqual(Ciphertext& cipher, ZZ* poly, long logp);

	void multByPolyNTTAndEqual(Ciphertext& cipher, uint64_t* rpoly, long bnd, long logp);

	/**
	 * multiplication by monomial X^degree
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] degree: monomial degree
	 * @return ciphertext(m) * X^degree
	 */
	Ciphertext multByMonomial(Ciphertext& cipher, const long degree);

	/**
	 * multiplication by monomial X^degree
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m) * X^degree
	 * @param[in] degree: monomial degree
	 */
	void multByMonomialAndEqual(Ciphertext& cipher, const long degree);

	/**
	 * multiplication by 2^bits
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] bits: shift bits
	 * @return ciphertext(m*2^bits)
	 */
	Ciphertext leftShift(Ciphertext& cipher, long bits);

	/**
	 * multiplication by 2^bits
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m*2^bits)
	 * @param[in] bits: shift bits
	 */
	void leftShiftAndEqual(Ciphertext& cipher, long bits);

	/**
	 * doubles
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(2m)
	 */
	void doubleAndEqual(Ciphertext& cipher);

	Ciphertext divByPo2(Ciphertext& cipher, long bits);

	void divByPo2AndEqual(Ciphertext& cipher, long bits);


	//----------------------------------------------------------------------------------
	//   RESCALING
	//----------------------------------------------------------------------------------

	Ciphertext reScaleBy(Ciphertext& cipher, long dlogq);

	Ciphertext reScaleTo(Ciphertext& cipher, long logq);

	void reScaleByAndEqual(Ciphertext& cipher, long dlogq);

	void reScaleToAndEqual(Ciphertext& cipher, long logq);

	Ciphertext modDownBy(Ciphertext& cipher, long dlogq);

	void modDownByAndEqual(Ciphertext& cipher, long dlogq);

	Ciphertext modDownTo(Ciphertext& cipher, long logq);

	void modDownToAndEqual(Ciphertext& cipher, long logq);


	//----------------------------------------------------------------------------------
	//   ROTATIONS & CONJUGATIONS
	//----------------------------------------------------------------------------------


	Ciphertext leftRotateFast(Ciphertext& cipher, long r);
	Ciphertext rightRotateFast(Ciphertext& cipher, long r);

	void leftRotateFastAndEqual(Ciphertext& cipher, long r);
	void rightRotateFastAndEqual(Ciphertext& cipher, long r);

	Ciphertext leftRotate(Ciphertext& cipher, long r);
	Ciphertext rightRotate(Ciphertext& cipher, long r);

	void leftRotateAndEqual(Ciphertext& cipher, long r);
	void rightRotateAndEqual(Ciphertext& cipher, long r);

	Ciphertext conjugate(Ciphertext& cipher);

	void conjugateAndEqual(Ciphertext& cipher);


	//----------------------------------------------------------------------------------
	//   BOOTSTRAPPING
	//----------------------------------------------------------------------------------


	void normalizeAndEqual(Ciphertext& cipher);

	void coeffToSlotAndEqual(Ciphertext& cipher);

	void slotToCoeffAndEqual(Ciphertext& cipher);

	void exp2piAndEqual(Ciphertext& cipher, long logp);

	void evalExpAndEqual(Ciphertext& cipher, long logT, long logI = 4);

	void bootstrapAndEqual(Ciphertext& cipher, long logq, long logQ, long logT, long logI = 4);

	Ciphertext bootstrap(Ciphertext& cipher, long logq, long logQ, long logT, long logI = 4);

};

#endif
