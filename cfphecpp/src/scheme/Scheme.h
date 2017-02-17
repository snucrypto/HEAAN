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
	void rlweInstance(CZZX& c0, CZZX& c1);

	Cipher encrypt(CZZ& m, ZZ& nu);
	CZZ decrypt(Cipher& cipher);
	void decrypt(vector<CZZ>& res, vector<Cipher>& ciphers);

	CZZX encode(long& logSlots, vector<CZZ>& mvec);
	Cipher encrypt(long& logSlots, vector<CZZ>& mvec, ZZ& nu);
	vector<CZZ> decrypt(long& logSlots, Cipher& cipher);

	CZZX encodeAll(vector<CZZ>& mvec);
	Cipher encryptAll(vector<CZZ>& mvec, ZZ& nu);
	vector<CZZ> decryptAll(Cipher& cipher);

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
	Cipher multByConst(Cipher& cipher, CZZ& cnst);
	void multByConstAndEqual(Cipher& cipher, ZZ& cnst);
	void multByConstAndEqual(Cipher& cipher, CZZ& cnst);

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
