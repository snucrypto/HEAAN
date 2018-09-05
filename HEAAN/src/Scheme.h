/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_SCHEME_H_
#define HEAAN_SCHEME_H_

#include "Common.h"
#include "Ciphertext.h"
#include "Context.h"
#include "Key.h"
#include "Plaintext.h"
#include "SecretKey.h"

#include <complex>

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

	Scheme(Context& context);

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
	 * generates key for bootstrapping (keys are stored in leftRotKeyMap and bootKeyMap)
	 */
	void addBootKey(SecretKey& secretKey, long logl, long logp);

	/**
	 * generates keys for sorting (keys are stored in leftRotKeyMap)
	 */
	void addSortKeys(SecretKey& secretKey, long size);


	//----------------------------------------------------------------------------------
	//   ENCODING & DECODING
	//----------------------------------------------------------------------------------

	/**
	 * encodes an array of double values into a ZZX polynomial using special fft inverse
	 * @param[in] vals: array double of values
	 * @param[in] slots: size of an array
	 * @param[in] logp: log of message quantize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return message
	 */
	Plaintext encode(double* vals, long slots, long logp, long logq);

	/**
	 * encodes an array of complex values into a ZZX polynomial using special fft inverse
	 * @param[in] vals: array complex of values
	 * @param[in] slots: size of an array
	 * @param[in] logp: log of message quantize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return message
	 */
	Plaintext encode(complex<double>* vals, long slots, long logp, long logq);

	/**
	 * decodes a ZZX polynomial into an array of complex values using special fft
	 * @param[in] msg: message
	 * @return decoded array of complex values
	 */
	complex<double>* decode(Plaintext& msg);

	/**
	 * encodes a single double value into a ZZX polynomial using special fft inverse
	 * @param[in] val: double value
	 * @param[in] logp: log of message quantize bits
	 * @param[in] logq: log of ciphertext modulus
	 * @return message
	 */
	Plaintext encodeSingle(double val, long logp, long logq);

	/**
	 * encodes a single complex value into a ZZX polynomial using special fft inverse
	 * @param[in] val: complex value
	 * @param[in] logp: log of message quantize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return message
	 */
	Plaintext encodeSingle(complex<double> val, long logp, long logq);

	/**
	 * decodes a ZZX polynomial into a single complex value using special fft
	 * @param[in] msg: message
	 * @return decoded complex value
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
	 * encodes an array of double values into message and then encrypts it into ciphertext using public key encyption
	 * @param[in] vals: array of double values
	 * @param[in] slots: array size
	 * @param[in] logp: log of message quantize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return ciphertext
	 */
	Ciphertext encrypt(double* vals, long slots, long logp, long logq);

	/**
	 * encodes an array of complex values into message and then encrypts it into ciphertext using public key encyption
	 * @param[in] vals: array of complex values
	 * @param[in] slots: array size
	 * @param[in] logp: log of message quantize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return ciphertext
	 */
	Ciphertext encrypt(complex<double>* vals, long slots, long logp, long logq);

	/**
	 * encodes an array of zeros into message and then encrypts it into ciphertext using public key encyption
	 * @param[in] slots: array size
	 * @param[in] logp: log of message quantize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return ciphertext
	 */
	Ciphertext encryptZeros(long slots, long logp, long logq);

	/**
	 * decrypts ciphertext into message and then decodes it into array of complex values
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return decrypted array of complex values
	 */
	complex<double>* decrypt(SecretKey& secretKey, Ciphertext& cipher);

	/**
	 * encodes single double value into a message and then encrypts it into a ciphertext using public key encyption
	 * @param[in] val: double value
	 * @param[in] logq: log of message quntize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return ciphertext
	 */
	Ciphertext encryptSingle(double val, long logp, long logq);

	/**
	 * encodes a single complex value into a message and then encrypts it into a ciphertext using public key encyption
	 * @param[in] val: complex value
	 * @param[in] logq: log of message quntize value
	 * @param[in] logq: log of ciphertext modulus
	 * @return ciphertext
	 */
	Ciphertext encryptSingle(complex<double> val, long logp, long logq);

	/**
	 * decrypts ciphertext into message and then decodes it into a single complex value
	 * @param[in] secretKey: secret key
	 * @param[in] cipher: ciphertext
	 * @return decrypted complex value
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
	 * quantized constant addition
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits. If logp < 0 used cipher.logp, else logp
	 * @return ciphertext(m + cnst * 2^logp)
	 */
	Ciphertext addConst(Ciphertext& cipher, double cnst, long logp = -1);

	/**
	 * quantized constant addition
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits. If logp < 0 used cipher.logp, else logp
	 * @return ciphertext(m + cnst * 2^logp)
	 */
	Ciphertext addConst(Ciphertext& cipher, RR& cnst, long logp = -1);

	/**
	 * quantized constant addition
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits. If logp < 0 used cipher.logp, else logp
	 * @return ciphertext(m + cnst * 2^logp)
	 */
	Ciphertext addConst(Ciphertext& cipher, complex<double> cnst, long logp = -1);

	/**
	 * quantized constant addition
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m + cnst * 2^logp)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits. If logp < 0 used cipher.logp, else logp
	 */
	void addConstAndEqual(Ciphertext& cipher, double cnst, long logp = -1);

	/**
	 * quantized constant addition
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m + cnst * 2^logp)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits. If logp < 0 used cipher.logp, else logp
	 */
	void addConstAndEqual(Ciphertext& cipher, RR& cnst, long logp = -1);

	/**
	 * quantized constant addition
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m + cnst * 2^logp)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits. If logp < 0 used cipher.logp, else logp
	 */
	void addConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp = -1);

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
	 * division by i (imaginary unit) in ciphertext
	 * @param[in] cipher: ciphertext(m)
	 * @return ciphertext(m / i)
	 */
	Ciphertext idiv(Ciphertext& cipher);

	/**
	 * multiplication by i (imaginary unit) in ciphertext
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(i * m)
	 */
	void imultAndEqual(Ciphertext& cipher);

	/**
	 * division by i (imaginary unit) in ciphertext
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m / i)
	 */
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
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 * @return ciphertext(m * (cnst * 2^logp))
	 */
	Ciphertext multByConst(Ciphertext& cipher, double cnst, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 * @return ciphertext(m * (cnst * 2^logp))
	 */
	Ciphertext multByConst(Ciphertext& cipher, RR& cnst, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m)
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 * @return ciphertext(m * (cnst * 2^logp))
	 */
	Ciphertext multByConst(Ciphertext& cipher, complex<double> cnst, long logp);

	/**
	 * quantized constant vector multiplication
	 * @param[in, out] cipher: ciphertext(m)
	 * @param[in] cnstVec: constant vector
	 * @param[in] slots: cnstVec size
	 * @param[in] logp: number of quantized bits
	 * @return ciphertext(m * (cnst * 2^logp))
	 */
	Ciphertext multByConstVec(Ciphertext& cipher, double* cnstVec, long slots, long logp);

	/**
	 * quantized constant vector multiplication
	 * @param[in, out] cipher: ciphertext(m)
	 * @param[in] cnstVec: constant vector
	 * @param[in] slots: cnstVec size
	 * @param[in] logp: number of quantized bits
	 * @return ciphertext(m * (cnst * 2^logp))
	 */
	Ciphertext multByConstVec(Ciphertext& cipher, complex<double>* cnstVec, long slots, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * (cnst * 2^logp))
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 */
	void multByConstAndEqual(Ciphertext& cipher, double cnst, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * (cnst * 2^logp))
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 */
	void multByConstAndEqual(Ciphertext& cipher, RR& cnst, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * (cnst * 2^logp))
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 */
	void multByConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * (cnst * 2^logp))
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 */
	void multByConstVecAndEqual(Ciphertext& cipher, double* cnst, long slots, long logp);

	/**
	 * quantized constant multiplication
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m * (cnst * 2^logp))
	 * @param[in] cnst: constant
	 * @param[in] logp: number of quantized bits
	 */
	void multByConstVecAndEqual(Ciphertext& cipher, complex<double>* cnst, long slots, long logp);

	/**
	 * polynomial multiplication
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] poly: polynomial - encoding(cnst)
	 * @param[in] logp: number of quantized bits
	 * @return ciphertext(m * cnst)
	 */
	Ciphertext multByPoly(Ciphertext& cipher, ZZX& poly, long logp);

	/**
	 * polynomial multiplication
	 * @param[in] cipher: ciphertext(m) -> ciphertext(m * cnst)
	 * @param[in] poly: polynomial - encoding(cnst)
	 * @param[in] logp: number of quantized bits
	 */
	void multByPolyAndEqual(Ciphertext& cipher, ZZX& poly, long logp);

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
	 * multiplication by 2^degree
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] degree: power degree
	 * @return ciphertext(m*2^degree)
	 */
	Ciphertext multByPo2(Ciphertext& cipher, long degree);

	/**
	 * multiplication by 2^degree
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m*2^degree)
	 * @param[in] degree: power degree
	 */
	void multByPo2AndEqual(Ciphertext& cipher, long degree);

	/**
	 * doubles
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(2m)
	 */
	void multBy2AndEqual(Ciphertext& cipher);

	/**
	 * division by 2^degree
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] degree: power degree
	 * @return ciphertext(m / 2^degree)
	 */
	Ciphertext divByPo2(Ciphertext& cipher, long degree);

	/**
	 * division by 2^degree
	 * @param[in] cipher: ciphertext(m) -> ciphertext(m / 2^degree)
	 * @param[in] degree: power degree
	 */
	void divByPo2AndEqual(Ciphertext& cipher, long degree);


	//----------------------------------------------------------------------------------
	//   RESCALING & MODULUS DOWN
	//----------------------------------------------------------------------------------


	/**
	 * rescaling procedure
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] bitsDown: rescaling bits
	 * @return ciphertext(m / 2^bitsDown) with new modulus (q / 2^bitsDown)
	 */
	Ciphertext reScaleBy(Ciphertext& cipher, long bitsDown);

	/**
	 * rescaling procedure
	 * @param[in] cipher: ciphertext(m)
	 * @param[in] newlogq: log of new ciphertext modulus
	 * @return ciphertext(m / 2^(logq - newlogq)) with new modulus (2^newlogq)
	 */
	Ciphertext reScaleTo(Ciphertext& cipher, long newlogq);

	/**
	 * rescaling procedure
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m / 2^bitsDown) with new modulus (q / 2^bitsDown)
	 * @param[in] bitsDown: rescaling bits
	 */
	void reScaleByAndEqual(Ciphertext& cipher, long bitsDown);

	/**
	 * rescaling procedure
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(m / 2^(logq - newlogq)) with new modulus (2^newlogq)
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


	//----------------------------------------------------------------------------------
	//   ADDITIONAL METHODS FOR BOOTSTRAPPING
	//----------------------------------------------------------------------------------


	/**
	 * part of bootstrapping procedure: normalizes coefficients of ax and bx in ciphertext
	 * @param[in, out] cipher: ciphertext with ax, bx -> ciphertext with normalized ax, bx
	 */
	void normalizeAndEqual(Ciphertext& cipher);

	/**
	 * part of bootstrapping procedure: calculates special fft in encrypted form
	 * @param[in, out] cipher: ciphertext(vecm) -> ciphertext(special fft of vecm)
	 */
	void coeffToSlotAndEqual(Ciphertext& cipher);

	/**
	 * part of bootstrapping procedure: calculates special fft inverse in encrypted form
	 * @param[in, out] cipher: ciphertext(vecm) -> ciphertext(special fft inverse of vecm)
	 */
	void slotToCoeffAndEqual(Ciphertext& cipher);

	/**
	 * part of bootstrapping procedure: calculates exponent of ciphertext
	 * @param[in, out] cipher: ciphertext(m) -> ciphertext(exp(2pim))
	 */
	void exp2piAndEqual(Ciphertext& cipher, long logp);

	/**
	 * part of bootstrapping procedure: removes qI parts from cipher
	 * @param[in, out] cipher: ciphertext(x + qI + i(y + qJ)) -> ciphertext(x + iy)
	 */
	void evalExpAndEqual(Ciphertext& cipher, long logT, long logI = 4);

	/**
	 * full bootstrapping procedure
	 * @param[in, out] cipher: ciphertext(x) in mod q-> ciphertext(x) in mod qq where Q > qq > q
	 * @param[in] logq: log of q
	 * @param[in] logQ: log of Q - max possible secure modulus
	 * @param[in] logT: number of squaring steps in remove I part
	 * @param[in] logI: for h = 64, logI by experiments is 4
	 */
	void bootstrapAndEqual(Ciphertext& cipher, long logq, long logQ, long logT, long logI = 4);

};

#endif
