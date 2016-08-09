#ifndef SCHEME_SCHEME_H_
#define SCHEME_SCHEME_H_

#include "../scheme/PolyParams.h"
#include "../scheme/PolyPubKey.h"
#include "../scheme/PolySecKey.h"


class PolyCipher;


using namespace std;
using namespace NTL;

class PolyScheme {
private:

public:
	PolyParams& params;
	PolySecKey& secretKey;
	PolyPubKey& publicKey;

	long getLogQi(long& level);
	long getLogTQi(long& level);

	PolyScheme(PolyParams& params, PolySecKey& secretKey, PolyPubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	virtual ~PolyScheme() {};

	PolyCipher encrypt(ZZ& m);
	ZZ decrypt(PolyCipher& cipher);

	PolyCipher add(PolyCipher& cipher1, PolyCipher& cipher2);
	PolyCipher sub(PolyCipher& cipher1, PolyCipher& cipher2);
	PolyCipher mul(PolyCipher& cipher1, PolyCipher& cipher2);
	PolyCipher addConstant(PolyCipher& cipher, ZZ& cnst);
	PolyCipher mulByConstant(PolyCipher& cipher, ZZ& cnst);
	PolyCipher modSwitch(PolyCipher& cipher, long newLevel);
	PolyCipher modEmbed(PolyCipher& cipher, long newLevel);

	void modEmbedAndEqual(PolyCipher& cipher, long newLevel);
	void modSwitchAndEqual(PolyCipher& cipher, long newLevel);
	void addAndEqual(PolyCipher& cipher1, PolyCipher& cipher2);
	void subAndEqual(PolyCipher& cipher1, PolyCipher& cipher2);
	void mulAndEqual(PolyCipher& cipher1, PolyCipher& cipher2);
	void addConstantAndEqual(PolyCipher& cipher, ZZ& cnst);
	void mulByConstantAndEqual(PolyCipher& cipher, ZZ& cnst);


};

#endif /* SCHEME_SCHEME_H_ */
