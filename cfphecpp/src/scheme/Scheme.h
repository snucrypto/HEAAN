#ifndef SCHEME_SCHEME_H_
#define SCHEME_SCHEME_H_

#include "SecKey.h"
#include "Cipher.h"
#include "Message.h"
#include "PubKey.h"

using namespace std;
using namespace NTL;

class Scheme {
private:

public:
	Params& params;
	SecKey& secretKey;
	PubKey& publicKey;

	Scheme(Params& params, SecKey& secretKey, PubKey& publicKey) : params(params), secretKey(secretKey), publicKey(publicKey) {};

	//-----------------------------------------

	ZZ getqi(long& level);
	ZZ getPqi(long& level);
	long getLogqi(long& level);

	//-----------------------------------------

	void trueValue(CZZ& m, ZZ& qi);
	void trueValue(ZZ& m, ZZ& qi);

	//-----------------------------------------

	void rlweInstance(ZZX& c0, ZZX& c1, ZZ& qi);
	void rlweInstance(ZZX& c0, ZZX& c1);

	//-----------------------------------------

	CZZ* doubleConjugate(CZZ*& vals, long slots);
	CZZ* doubleConjugate(CZZ& val);
	CZZ* deConjugate(CZZ*& vals, long slots);

	//-----------------------------------------

	Message encode(CZZ*& vals, long size);
	Cipher encrypt(Message& msg, long level = 1);
	Cipher encryptFull(CZZ& val, long level = 1);
	Cipher encryptFull(CZZ*& vals, long slots, long level = 1);
	Cipher* encryptFullSingleArray(CZZ*& vals, long size);

	//-----------------------------------------

	Message decrypt(Cipher& cipher);
	CZZ* decode(Message& msg);
	CZZ* decryptFull(Cipher& cipher);
	CZZ decryptFullSingle(Cipher& cipher);
	CZZ* decryptFullSingleArray(Cipher*& ciphers, long size);

	//-----------------------------------------

	Cipher add(Cipher& cipher1, Cipher& cipher2);
	void addAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	Cipher addConst(Cipher& cipher, ZZ& cnst);
	Cipher addConst(Cipher& cipher, CZZ& cnst);
	void addConstAndEqual(Cipher& cipher, ZZ& cnst);

	//-----------------------------------------

	Cipher sub(Cipher& cipher1, Cipher& cipher2);
	void subAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	Cipher mult(Cipher& cipher1, Cipher& cipher2);
	void multAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	Cipher square(Cipher& cipher);
	void squareAndEqual(Cipher& cipher);

	//-----------------------------------------

	Cipher multByConst(Cipher& cipher, ZZ& cnst);
	void multByConstAndEqual(Cipher& cipher, ZZ& cnst);

	//-----------------------------------------

	Cipher multByMonomial(Cipher& cipher, const long& degree);
	void multByMonomialAndEqual(Cipher& cipher, const long& degree);

	//-----------------------------------------

	Cipher leftShift(Cipher& cipher, long& bits);
	void leftShiftAndEqual(Cipher& cipher, long& bits);

	//-----------------------------------------

	Cipher modSwitch(Cipher& cipher, long newLevel);
	Cipher modSwitch(Cipher& cipher);
	void modSwitchAndEqual(Cipher& cipher, long newLevel);
	void modSwitchAndEqual(Cipher& cipher);

	//-----------------------------------------

	Cipher modEmbed(Cipher& cipher, long newLevel);
	Cipher modEmbed(Cipher& cipher);
	void modEmbedAndEqual(Cipher& cipher, long newLevel);
	void modEmbedAndEqual(Cipher& cipher);

	//-----------------------------------------

	Cipher multAndModSwitch(Cipher& cipher1, Cipher& cipher2);
	void multModSwitchAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	Cipher rotate(Cipher& cipher, long& steps);
	void rotateAndEqual(Cipher& cipher, long& steps);

};

#endif /* SCHEME_SCHEME_H_ */
