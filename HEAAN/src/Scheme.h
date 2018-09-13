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

	bool isSerialized;

	map<long, Key*> keyMap; ///< contain Encryption, Multiplication and Conjugation keys, if generated
	map<long, Key*> leftRotKeyMap; ///< contain left rotation keys, if generated

	map<long, string> serKeyMap; ///< contain Encryption, Multiplication and Conjugation keys, if generated
	map<long, string> serLeftRotKeyMap; ///< contain left rotation keys, if generated

	Scheme(SecretKey& secretKey, Ring& ring, bool isSerialized = false);

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

	void serializeRotKey(long r, string path);

	void deserializeRotKey(long r, string path);


	//----------------------------------------------------------------------------------
	//   ENCODING & DECODING
	//----------------------------------------------------------------------------------


	Plaintext encode(complex<double>* vals, long n, long logp, long logq);

	Plaintext encode(double* vals, long n, long logp, long logq);

	complex<double>* decode(Plaintext& msg);

	Plaintext encodeSingle(complex<double> val, long logp, long logq);

	Plaintext encodeSingle(double val, long logp, long logq);

	complex<double> decodeSingle(Plaintext& msg);


	//----------------------------------------------------------------------------------
	//   ENCRYPTION & DECRYPTION
	//----------------------------------------------------------------------------------


	Ciphertext encryptMsg(Plaintext& msg);

	Plaintext decryptMsg(SecretKey& secretKey, Ciphertext& cipher);

	Ciphertext encrypt(complex<double>* vals, long n, long logp, long logq);

	Ciphertext encrypt(double* vals, long n, long logp, long logq);

	Ciphertext encryptZeros(long n, long logp, long logq);

	complex<double>* decrypt(SecretKey& secretKey, Ciphertext& cipher);

	Ciphertext encryptSingle(complex<double> val, long logp, long logq);

	Ciphertext encryptSingle(double val, long logp, long logq);

	complex<double> decryptSingle(SecretKey& secretKey, Ciphertext& cipher);


	//----------------------------------------------------------------------------------
	//   HOMOMORPHIC OPERATIONS
	//----------------------------------------------------------------------------------

	Ciphertext negate(Ciphertext& cipher);

	void negateAndEqual(Ciphertext& cipher);

	Ciphertext add(Ciphertext& cipher1, Ciphertext& cipher2);

	void addAndEqual(Ciphertext& cipher1, Ciphertext& cipher2);

	Ciphertext addConst(Ciphertext& cipher, double cnst, long logp);

	Ciphertext addConst(Ciphertext& cipher, RR& cnst, long logp);

	Ciphertext addConst(Ciphertext& cipher, complex<double> cnst, long logp);

	void addConstAndEqual(Ciphertext& cipher, double cnst, long logp);

	void addConstAndEqual(Ciphertext& cipher, RR& cnst, long logp);

	void addConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp);

	Ciphertext sub(Ciphertext& cipher1, Ciphertext& cipher2);

	void subAndEqual(Ciphertext& cipher1, Ciphertext& cipher2);

	void subAndEqual2(Ciphertext& cipher1, Ciphertext& cipher2);

	Ciphertext imult(Ciphertext& cipher);

	Ciphertext idiv(Ciphertext& cipher);

	void imultAndEqual(Ciphertext& cipher);

	void idivAndEqual(Ciphertext& cipher);

	Ciphertext mult(Ciphertext& cipher1, Ciphertext& cipher2);

	void multAndEqual(Ciphertext& cipher1, Ciphertext& cipher2);

	Ciphertext square(Ciphertext& cipher);

	void squareAndEqual(Ciphertext& cipher);

	Ciphertext multByConst(Ciphertext& cipher, double cnst, long logp);

	Ciphertext multByConst(Ciphertext& cipher, complex<double> cnst, long logp);

	Ciphertext multByConstVec(Ciphertext& cipher, complex<double>* cnstVec, long logp);

	void multByConstAndEqual(Ciphertext& cipher, double cnst, long logp);

	void multByConstAndEqual(Ciphertext& cipher, RR& cnst, long logp);

	void multByConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp);

	Ciphertext multByPoly(Ciphertext& cipher, ZZ* poly, long logp);

	Ciphertext multByPolyNTT(Ciphertext& cipher, uint64_t* rpoly, long bnd, long logp);

	void multByPolyAndEqual(Ciphertext& cipher, ZZ* poly, long logp);

	void multByPolyNTTAndEqual(Ciphertext& cipher, uint64_t* rpoly, long bnd, long logp);

	Ciphertext multByMonomial(Ciphertext& cipher, const long degree);

	void multByMonomialAndEqual(Ciphertext& cipher, const long degree);

	Ciphertext leftShift(Ciphertext& cipher, long bits);

	void leftShiftAndEqual(Ciphertext& cipher, long bits);

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
