#ifndef FPHEFFTSCHEME_H_
#define FPHEFFTSCHEME_H_

#include "FPHEFFTParams.h"
#include "FPHEFFTPubKey.h"
#include "FPHEFFTSecKey.h"
#include "TimeUtils.h"



class FPHEFFTCipher;


using namespace std;
using namespace NTL;

class FPHEFFTScheme {
private:

public:
	TimeUtils timeutils;
	FPHEFFTParams& params;
	FPHEFFTSecKey& secretKey;
	FPHEFFTPubKey& publicKey;

	ZZ& getModulo(long level);
	ZZ& getPqModulo(long level);

	FPHEFFTScheme(FPHEFFTParams& params, FPHEFFTSecKey& secretKey, FPHEFFTPubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	virtual ~FPHEFFTScheme() {};

	FPHEFFTCipher encrypt(ZZ& m);
	ZZ decrypt(FPHEFFTCipher& cipher);

	FPHEFFTCipher add(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2);
	FPHEFFTCipher sub(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2);
	FPHEFFTCipher mul(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2);
	FPHEFFTCipher addConstant(FPHEFFTCipher& cipher, ZZ& cnst);
	FPHEFFTCipher mulByConstant(FPHEFFTCipher& cipher, ZZ& cnst);
	FPHEFFTCipher modSwitch(FPHEFFTCipher& cipher, long newLevel);
	FPHEFFTCipher modEmbed(FPHEFFTCipher& cipher, long newLevel);

	void modEmbedAndEqual(FPHEFFTCipher& cipher, long newLevel);
	void modSwitchAndEqual(FPHEFFTCipher& cipher, long newLevel);
	void addAndEqual(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2);
	void subAndEqual(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2);
	void mulAndEqual(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2);
	void addConstantAndEqual(FPHEFFTCipher& cipher, ZZ& cnst);
	void mulByConstantAndEqual(FPHEFFTCipher& cipher, ZZ& cnst);


};

#endif /* FPHEFFTSCHEME_H_ */
