#ifndef SCHEME_SCHEME_H_
#define SCHEME_SCHEME_H_

#include <vector>

#include "Params.h"
#include "Cipher.h"
#include "SecKey.h"
#include "PubKey.h"

using namespace std;
using namespace NTL;

class Scheme {
private:

public:
	Params& params;
	SecKey& secretKey;
	PubKey& publicKey;

	Scheme(Params& params, SecKey& secretKey, PubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	void trueValue(CZZ& m, ZZ& qi);
	void rlweInstance(ZZX& c0, ZZX& c1);

	ZZX encode(Message& msg);
	Cipher encrypt(Message& msg);
	Message decrypt(Cipher& cipher);


	ZZX encodeAll(Message& msg);
	Cipher encryptAll(Message& msg);
	Message decryptAll(Cipher& cipher);

//--------------------------------

	Cipher add(Cipher& cipher1, Cipher& cipher2);
	void addAndEqual(Cipher& cipher1, Cipher& cipher2);

	Cipher addConst(Cipher& cipher, ZZ& cnst);
	Cipher addConst(Cipher& cipher, CZZ& cnst);
	void addConstAndEqual(Cipher& cipher, ZZ& cnst);

	Cipher sub(Cipher& cipher1, Cipher& cipher2);
	void subAndEqual(Cipher& cipher1, Cipher& cipher2);

	Cipher mult(Cipher& cipher1, Cipher& cipher2);
	void multAndEqual(Cipher& cipher1, Cipher& cipher2);

	Cipher square(Cipher& cipher);
	void squareAndEqual(Cipher& cipher);

	Cipher multByConst(Cipher& cipher, ZZ& cnst);
	void multByConstAndEqual(Cipher& cipher, ZZ& cnst);

	Cipher multByMonomial(Cipher& cipher, const long& degree);
	void multByMonomialAndEqual(Cipher& cipher, const long& degree);

	Cipher leftShift(Cipher& cipher, long& bits);
	void leftShiftAndEqual(Cipher& cipher, long& bits);

	Cipher modSwitch(Cipher& cipher, long newLevel);
	Cipher modSwitch(Cipher& cipher);

	void modSwitchAndEqual(Cipher& cipher, long newLevel);
	void modSwitchAndEqual(Cipher& cipher);

	Cipher modEmbed(Cipher& cipher, long newLevel);
	Cipher modEmbed(Cipher& cipher);

	void modEmbedAndEqual(Cipher& cipher, long newLevel);
	void modEmbedAndEqual(Cipher& cipher);

	Cipher multAndModSwitch(Cipher& cipher1, Cipher& cipher2);
	void multModSwitchAndEqual(Cipher& cipher1, Cipher& cipher2);
//-----------------------------------------

	ZZ getqi(long& level);
	ZZ getPqi(long& level);
	long getLogqi(long& level);

};

#endif /* SCHEME_SCHEME_H_ */
