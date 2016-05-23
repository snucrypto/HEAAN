#ifndef FPHESCHEME_H_
#define FPHESCHEME_H_

#include "FPHEParams.h"
#include "FPHESecKey.h"
#include "FPHEPubKey.h"
#include "TimeUtils.h"



class FPHECipher;


using namespace std;
using namespace NTL;

class FPHEScheme {
private:

public:
	TimeUtils timeutils;
	FPHEParams& params;
	FPHESecKey& secretKey;
	FPHEPubKey& publicKey;

	ZZ& getModulo(long level);
	ZZ& getPqModulo(long level);

	FPHEScheme(FPHEParams& params, FPHESecKey& secretKey, FPHEPubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	virtual ~FPHEScheme() {};

	FPHECipher encrypt(ZZ& m);
	ZZ decrypt(FPHECipher& cipher);

	FPHECipher add(FPHECipher& cipher1, FPHECipher& cipher2);
	FPHECipher sub(FPHECipher& cipher1, FPHECipher& cipher2);
	FPHECipher mul(FPHECipher& cipher1, FPHECipher& cipher2);
	FPHECipher addConstant(FPHECipher& cipher, ZZ& cnst);
	FPHECipher mulByConstant(FPHECipher& cipher, ZZ& cnst);
	FPHECipher modSwitch(FPHECipher& cipher, long newLevel);
	FPHECipher modEmbed(FPHECipher& cipher, long newLevel);

	void modEmbedAndEqual(FPHECipher& cipher, long newLevel);
	void modSwitchAndEqual(FPHECipher& cipher, long newLevel);
	void addAndEqual(FPHECipher& cipher1, FPHECipher& cipher2);
	void subAndEqual(FPHECipher& cipher1, FPHECipher& cipher2);
	void mulAndEqual(FPHECipher& cipher1, FPHECipher& cipher2);
	void addConstantAndEqual(FPHECipher& cipher, ZZ& cnst);
	void mulByConstantAndEqual(FPHECipher& cipher, ZZ& cnst);


};

#endif /* FPHESCHEME_H_ */
