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

	Scheme(Params& params, SecKey& secretKey, PubKey& publicKey);

	ZZ getqi(long& level);
	ZZ getPqi(long& level);
	long getLogqi(long& level);

	void trueValue(CZZ& m, ZZ& qi);
	void trueValue(ZZ& m, ZZ& qi);

	void rlweInstance(ZZX& c0, ZZX& c1, ZZ& qi);
	void rlweInstance(ZZX& c0, ZZX& c1);

	//-----------------------------------------

	vector<CZZ> doubleConjugate(vector<CZZ>& vals);
	vector<CZZ> doubleConjugate(CZZ& val);

	Message encode(vector<CZZ>& vals);
	Cipher encrypt(Message& msg, long& level);
	Cipher encrypt(Message& msg);

	Message decrypt(Cipher& cipher);
	vector<CZZ> decode(Message& msg);

	vector<CZZ> deConjugate(vector<CZZ>& vals);

	//-----------------------------------------

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

};

#endif /* SCHEME_SCHEME_H_ */
