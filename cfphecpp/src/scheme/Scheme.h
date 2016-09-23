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

	long getLogQi(long& level);
	long getLogTQi(long& level);

	Scheme(Params& params, SecKey& secretKey, PubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	virtual ~Scheme() {};

	Cipher encrypt(ZZ& m);
	ZZ decrypt(Cipher& cipher);

	Cipher add(Cipher& cipher1, Cipher& cipher2);
	Cipher sub(Cipher& cipher1, Cipher& cipher2);
	Cipher mul(Cipher& cipher1, Cipher& cipher2);
	Cipher addConstant(Cipher& cipher, ZZ& cnst);
	Cipher mulByConstant(Cipher& cipher, ZZ& cnst);
	Cipher mulByConstant(Cipher& cipher, ZZ& cnstr, ZZ& cnsti);
	Cipher modSwitch(Cipher& cipher, long newLevel);
	Cipher modEmbed(Cipher& cipher, long newLevel);

	void modEmbedAndEqual(Cipher& cipher, long newLevel);
	void modSwitchAndEqual(Cipher& cipher, long newLevel);
	void addAndEqual(Cipher& cipher1, Cipher& cipher2);
	void subAndEqual(Cipher& cipher1, Cipher& cipher2);
	void mulAndEqual(Cipher& cipher1, Cipher& cipher2);
	void addConstantAndEqual(Cipher& cipher, ZZ& cnst);
	void mulByConstantAndEqual(Cipher& cipher, ZZ& cnst);

	vector<Cipher> fft(vector<Cipher>& ciphers, vector<Ksi>& ksis);
};

#endif /* SCHEME_SCHEME_H_ */
