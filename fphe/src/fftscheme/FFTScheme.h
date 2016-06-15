#ifndef FPHEFFTSCHEME_H_
#define FPHEFFTSCHEME_H_

#include "FFTParams.h"
#include "FFTPubKey.h"
#include "FFTSecKey.h"
#include "TimeUtils.h"



class FFTCipher;


using namespace std;
using namespace NTL;

class FFTScheme {
private:

public:
	TimeUtils timeutils;
	FFTParams& params;
	FFTSecKey& secretKey;
	FFTPubKey& publicKey;

	ZZ& getModulo(long level);
	ZZ& getPqModulo(long level);

	FFTScheme(FFTParams& params, FFTSecKey& secretKey, FFTPubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

	virtual ~FFTScheme() {};

	FFTCipher encrypt(ZZ& m);
	ZZ decrypt(FFTCipher& cipher);

	FFTCipher add(FFTCipher& cipher1, FFTCipher& cipher2);
	FFTCipher sub(FFTCipher& cipher1, FFTCipher& cipher2);
	FFTCipher mul(FFTCipher& cipher1, FFTCipher& cipher2);
	FFTCipher addConstant(FFTCipher& cipher, ZZ& cnst);
	FFTCipher mulByConstant(FFTCipher& cipher, ZZ& cnst);
	FFTCipher modSwitch(FFTCipher& cipher, long newLevel);
	FFTCipher modEmbed(FFTCipher& cipher, long newLevel);

	void modEmbedAndEqual(FFTCipher& cipher, long newLevel);
	void modSwitchAndEqual(FFTCipher& cipher, long newLevel);
	void addAndEqual(FFTCipher& cipher1, FFTCipher& cipher2);
	void subAndEqual(FFTCipher& cipher1, FFTCipher& cipher2);
	void mulAndEqual(FFTCipher& cipher1, FFTCipher& cipher2);
	void addConstantAndEqual(FFTCipher& cipher, ZZ& cnst);
	void mulByConstantAndEqual(FFTCipher& cipher, ZZ& cnst);


};

#endif /* FPHEFFTSCHEME_H_ */
