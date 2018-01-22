#ifndef HEAAN_SCHEME_H_
#define HEAAN_SCHEME_H_

#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

#include "Common.h"
#include "CZZ.h"
#include "SecretKey.h"
#include "Ciphertext.h"
#include "Plaintext.h"
#include "Key.h"
#include "Context.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Params.h"
#include "Ring2Utils.h"

using namespace std;
using namespace NTL;

static long ENCRYPTION = 0;
static long MULTIPLICATION  = 1;
static long CONJUGATION = 2;

class Scheme {
private:
public:
	Context& context;

	map<long, Key> keyMap; ///< contain Encryption, Multiplication and Conjugation keys, if generated
	map<long, Key> leftRotKeyMap; ///< contain left rotation keys, if generated

	Scheme(SecretKey& secretKey, Context& context);


	//----------------------------------------------------------------------------------
	//   KEYS GENERATION
	//----------------------------------------------------------------------------------


	/**
	 * generates key for public encryption (key is stored in keyMap)
	 */
	void addEncKey(SecretKey& secretKey);

	/**
	 * generates key for conjugation (key is stored in keyMap)
	 */
	void addConjKey(SecretKey& secretKey);

	/**
	 * generates key for multiplication (key is stored in keyMap)
	 */
	void addMultKey(SecretKey& secretKey);

	/**
	 * generates key for left rotation (key is stored in leftRotKeyMap)
	 */
	void addLeftRotKey(SecretKey& secretKey, long rot);

	/**
	 * generates all keys for power-of-two left rotations (keys are stored in leftRotKeyMap)
	 */
	void addLeftRotKeys(SecretKey& secretKey);

	/**
	 * generates all keys for power-of-two right rotations (keys are stored in leftRotKeyMap)
	 */
	void addRightRotKeys(SecretKey& secretKey);

	/**
	 * generates keys for sorting (keys are stored in leftRotKeyMap)
	 */
	void addSortKeys(SecretKey& secretKey, long size);


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
	Plaintext encode(CZZ* vals, long slots, long logq, bool isComplex = true);

	/**
	 * decodes a ZZX polynomial into an array of CZZ values using special fft
	 * @param[in] msg: message
	 * @return array of CZZ values
	 */
	CZZ* decodeSmall(Plaintext& msg);

	/**
	 * encodes a single CZZ value into a ZZX polynomial using special fft inverse
	 * @param[in] val: CZZ value
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return message
	 */
	Plaintext encodeLargeSingle(CZZ& val, long logq, bool isComplex = true);

	/**
	 * decodes a ZZX polynomial into a single CZZ value using special fft
	 * @param[in] msg: message
	 * @return CZZ value
	 */
	CZZ decodeSmallSingle(Plaintext& msg);


	//----------------------------------------------------------------------------------
	//   ENCRYPTION & DECRYPTION
	//----------------------------------------------------------------------------------

	/**
	 * encrypts message into ciphertext using secret key information
	 * @param[in] msg: message
	 * @return ciphertext
	 */
	Ciphertext encryptMsgSK(SecretKey& secretKey, Plaintext& msg);

	/**
	 * encrypts message into ciphertext using public key encyption
	 * @param[in] msg: message
	 * @return ciphertext
	 */
	Ciphertext encryptLargeMsg(Plaintext& msg);

	/**
	 * decrypts ciphertext into message
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return message
	 */
	Plaintext decryptSmallMsg(SecretKey& secretKey, Ciphertext& cipher);

	/**
	 * encodes array of CZZ into message and then encrypts it into ciphertext using public key encyption
	 * @param[in] vals: array of CZZ values
	 * @param[in] slots: array size
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return ciphertext
	 */
	Ciphertext encryptSK(SecretKey& secretKey, CZZ* vals, long slots, long logq, bool isComplex = true);

	/**
	 * encodes array of CZZ into message and then encrypts it into ciphertext using public key encyption
	 * @param[in] vals: array of CZZ values
	 * @param[in] slots: array size
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return ciphertext
	 */
	Ciphertext encrypt(CZZ* vals, long slots, long logq, bool isComplex = true);

	/**
	 * decrypts ciphertext into message and then decodes it into array of CZZ values
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return array of CZZ values
	 */
	CZZ* decrypt(SecretKey& secretKey, Ciphertext& cipher);

	/**
	 * encodes single CZZ value into a message and then encrypts it into a ciphertext using public key encyption
	 * @param[in] val: CZZ value
	 * @param[in] logq: log of ciphertext modulus
	 * @param[in] isComplex: there is an option for encryption single real value
	 * @return ciphertext
	 */
	Ciphertext encryptSingle(CZZ& val, long logq, bool isComplex = true);

	/**
	 * decrypts ciphertext into message and then decodes it into a single CZZ value
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return CZZ value
	 */
	CZZ decryptSingle(SecretKey& secretKey, Ciphertext& cipher);


	//----------------------------------------------------------------------------------
	//   HOMOMORPHIC OPERATIONS
	//----------------------------------------------------------------------------------


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
	Ciphertext addConst(Ciphertext& cipher, ZZ& cnst);

	/**
	 * constant addition
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m + constant)
	 * @param[in] cnst: constant
	 */
	void addConstAndEqual(Ciphertext& cipher, ZZ& cnst);

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

	/**
	 * multiplication by i (imaginary unit) in ciphertext
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(i * m)
	 */
	void imultAndEqual(Ciphertext& cipher);

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
	Ciphertext multByConst(Ciphertext& cipher, ZZ& cnst);

	/**
	 * constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * constant)
	 * @param[in] cnst: constant
	 */
	void multByConstAndEqual(Ciphertext& cipher, ZZ& cnst);

	/**
	 * polynomial multiplication
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] poly: polynomial, encoding (constant)
	 * @return ciphertext(m * constant)
	 */
	Ciphertext multByPoly(Ciphertext& cipher, ZZX& poly);

	/**
	 * polynomial multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * constant)
	 * @param[in] poly: polynomial, encoding (constant)
	 */
	void multByPolyAndEqual(Ciphertext& cipher, ZZX& poly);

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

	/**
	 * rescaling procedure
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] bitsDown: rescaling bits
	 * @return ciphertext(m/2^bitsDown) with new modulus (q/2^bitsDown)
	 */
	Ciphertext reScaleBy(Ciphertext& cipher, long bitsDown);

	/**
	 * rescaling procedure
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] newlogq: log of new ciphertext modulus
	 * @return ciphertext(m*2^newlogq/q) with new modulus (newlogq)
	 */
	Ciphertext reScaleTo(Ciphertext& cipher, long newlogq);

	/**
	 * rescaling procedure
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m/2^bitsDown) with new modulus (q/2^bitsDown)
	 * @param[in] bitsDown: rescaling bits
	 */
	void reScaleByAndEqual(Ciphertext& cipher, long bitsDown);

	/**
	 * rescaling procedure
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m*2^newlogq/q) with new modulus (newlogq)
	 * @param[in] newlogq: log ofnew ciphertext modulus
	 */
	void reScaleToAndEqual(Ciphertext& cipher, long newlogq);

	/**
	 * modulus down procedure
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] bitsDown: modulus down bits
	 * @return ciphertext(m) with new modulus (q/2^bitsDown)
	 */
	Ciphertext modDownBy(Ciphertext& cipher, long bitsDown);

	/**
	 * modulus down procedure
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m) with new modulus (q/2^bitsDown)
	 * @param[in] bitsDown: modulus down bits
	 */
	void modDownByAndEqual(Ciphertext& cipher, long bitsDown);

	/**
	 * modulus down procedure
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] newlogq: log of new ciphertext modulus
	 * @return ciphertext(m) with new modulus (2^newlogq)
	 */
	Ciphertext modDownTo(Ciphertext& cipher, long newlogq);

	/**
	 * modulus down procedure
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m) with new modulus (2^newlogq)
	 * @param[in] newlogq: log of new ciphertext modulus
	 */
	void modDownToAndEqual(Ciphertext& cipher, long newlogq);


	//----------------------------------------------------------------------------------
	//   ROTATIONS & CONJUGATIONS
	//----------------------------------------------------------------------------------


	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in] cipher: ciphertext(m(v_1, v_2, ..., v_slots))
	 * @param[in] logRotSlots: log of rotation slots
	 * @return ciphertext(m(v_{1+rotSlots}, v_{2+rotSlots}, ..., v_{slots+rotSlots})
	 */
	Ciphertext leftRotateByPo2(Ciphertext& cipher, long logRotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in, out] cipher: ciphertext(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1+rotSlots}, v_{2+rotSlots}, ..., v_{slots+rotSlots})
	 * @param[in] logRotSlots: log of rotation slots
	 */
	void leftRotateByPo2AndEqual(Ciphertext& cipher, long logRotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in] cipher: ciphertext(m(v_1, v_2, ..., v_slots))
	 * @param[in] logRotSlots: log of rotation slots
	 * @return ciphertext(m(v_{1-rotSlots}, v_{2-rotSlots}, ..., v_{slots-rotSlots})
	 */
	Ciphertext rightRotateByPo2(Ciphertext& cipher, long logRotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in, out] cipher: ciphertext(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1-rotSlots}, v_{2-rotSlots}, ..., v_{slots-rotSlots})
	 * @param[in] logRotSlots: log of rotation slots
	 */
	void rightRotateByPo2AndEqual(Ciphertext& cipher, long logrotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in] cipher: ciphertext(m(v_1, v_2, ..., v_slots))
	 * @param[in] logRotSlots: log of rotation slots
	 * @return ciphertext(m(v_{1+rotSlots}, v_{2+rotSlots}, ..., v_{slots+rotSlots})
	 */
	Ciphertext leftRotateFast(Ciphertext& cipher, long rotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in, out] cipher: ciphertext(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1+rotSlots}, v_{2+rotSlots}, ..., v_{slots+rotSlots})
	 * @param[in] logRotSlots: log of rotation slots
	 */
	void leftRotateAndEqualFast(Ciphertext& cipher, long rotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in] cipher: ciphertext(m(v_1, v_2, ..., v_slots))
	 * @param[in] rotSlots: rotation slots
	 * @return ciphertext(m(v_{1+rotSlots}, v_{2+rotSlots}, ..., v_{slots+rotSlots})
	 */
	Ciphertext leftRotate(Ciphertext& cipher, long rotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in, out] cipher: ciphertext(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1+rotSlots}, v_{2+rotSlots}, ..., v_{slots+rotSlots})
	 * @param[in] rotSlots: rotation slots
	 */
	void leftRotateAndEqual(Ciphertext& cipher, long rotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in] cipher: ciphertext(m(v_1, v_2, ..., v_slots))
	 * @param[in] rotSlots: rotation slots
	 * @return ciphertext(m(v_{1-rotSlots}, v_{2-rotSlots}, ..., v_{slots-rotSlots})
	 */
	Ciphertext rightRotate(Ciphertext& cipher, long rotSlots);

	/**
	 * calculates ciphertext of array with rotated indexes
	 * @param[in, out] cipher: ciphertext(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1-rotSlots}, v_{2-rotSlots}, ..., v_{slots-rotSlots})
	 * @param[in] rotSlots: rotation slots
	 */
	void rightRotateAndEqual(Ciphertext& cipher, long rotSlots);

	/**
	 * calculates ciphertext of conjugations
	 * @param[in] cipher: ciphertext(m = x + iy)
	 * @return ciphertext(x - iy)
	 */
	Ciphertext conjugate(Ciphertext& cipher);

	/**
	 * calculates ciphertext of conjugations
	 * @param[in, out] cipher: ciphertext(m = x + iy) -> ciphertext(x - iy)
	 */
	void conjugateAndEqual(Ciphertext& cipher);

};

#endif
