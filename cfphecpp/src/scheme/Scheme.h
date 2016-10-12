#ifndef SCHEME_SCHEME_H_
#define SCHEME_SCHEME_H_

#include <vector>

#include "../eval/Ksi.h"
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

	ZZ getQi(long& level);
	long getLogQi(long& level);
	long getLogTQi(long& level);

	Scheme(Params& params, SecKey& secretKey, PubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	Cipher encrypt(CZZ& m, ZZ& nu);
	CZZ decrypt(Cipher& cipher);
	void decrypt(vector<CZZ>& res, vector<Cipher>& ciphers);
//--------------------------------

	Cipher add(Cipher& cipher1, Cipher& cipher2);
	void addAndEqual(Cipher& cipher1, Cipher& cipher2);

	Cipher addConstant(Cipher& cipher, ZZ& cnst);
	Cipher addConstant(Cipher& cipher, CZZ& cnst);
	void addConstantAndEqual(Cipher& cipher, ZZ& cnst);

	Cipher sub(Cipher& cipher1, Cipher& cipher2);
	void subAndEqual(Cipher& cipher1, Cipher& cipher2);

	Cipher mult(Cipher& cipher1, Cipher& cipher2);
	void multAndEqual(Cipher& cipher1, Cipher& cipher2);
	Cipher multAndModSwitch(Cipher& cipher1, Cipher& cipher2);

	Cipher multByConstant(Cipher& cipher, ZZ& cnst);
	Cipher multByConstant(Cipher& cipher, CZZ& cnst);
	void multByConstantAndEqual(Cipher& cipher, ZZ& cnst);

	Cipher square(Cipher& cipher);
	void squareAndEqual(Cipher& cipher);
	Cipher squareAndModSwitch(Cipher& cipher);

	Cipher modSwitch(Cipher& cipher, long newLevel);
	void modSwitchAndEqual(Cipher& cipher, long newLevel);

	Cipher modEmbed(Cipher& cipher, long newLevel);
	void modEmbedAndEqual(Cipher& cipher, long newLevel);

//-----------------------------------------

};

#endif /* SCHEME_SCHEME_H_ */
