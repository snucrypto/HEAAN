#include "FPHEScheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>

#include "FPHECipher.h"
#include "ZRingUtils.h"

using namespace std;
using namespace NTL;

FPHECipher FPHEScheme::encrypt(ZZ& m) {
	ZZ bits;
	RandomBits(bits, params.tau);
	vector<ZZ> c0fft;
	vector<ZZ> c1fft;

	c0fft.resize(params.phim);
	c1fft.resize(params.phim);

	long i;
	for (i = 0; i < params.tau; ++i) {
		if(bit(bits, i)) {
			vector<ZZ> lwefft0 = publicKey.A0fft[i];
			vector<ZZ> lwefft1 = publicKey.A1fft[i];
			ZRingUtils::addRing(c0fft, c0fft, lwefft0, params.qL, params.phim);
			ZRingUtils::addRing(c1fft, c1fft, lwefft1, params.qL, params.phim);
		}
	}
	ZRingUtils::addConstantRing(c0fft, c0fft, m, params.qL, params.phim);
	FPHECipher cipher(c0fft, c1fft, 1);
	return cipher;
}

ZZ FPHEScheme::decrypt(FPHECipher& cipher) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> mfft;
	vector<ZZ> m;
	ZRingUtils::mulRing(mfft, secretKey.sfft, cipher.c1fft, qi, params.phim);
	ZRingUtils::addRing(mfft, mfft, cipher.c0fft, qi, params.phim);

	ZRingUtils::convertfftInv(m, mfft, params.fftInv, qi, params.phim);

	ZZ c = m[0];
	if(c < 0) {c += qi;}
	return c;
}

FPHECipher FPHEScheme::add(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	vector<ZZ> c0;
	vector<ZZ> c1;
	ZRingUtils::addRing(c0, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtils::addRing(c1, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
	FPHECipher res(c0, c1, cipher1.level);
	return res;
}

void FPHEScheme::addAndEqual(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);

	ZRingUtils::addRing(cipher1.c0fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtils::addRing(cipher1.c1fft, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
}

FPHECipher FPHEScheme::sub(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	vector<ZZ> c0;
	vector<ZZ> c1;
	ZRingUtils::subRing(c0, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtils::subRing(c1, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
	FPHECipher res(c0, c1, cipher1.level);
	return res;
}

void FPHEScheme::subAndEqual(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZRingUtils::subRing(cipher1.c0fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtils::subRing(cipher1.c1fft, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
}

FPHECipher FPHEScheme::mul(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	vector<ZZ> cc00fft;
	vector<ZZ> cc01fft;
	vector<ZZ> cc10fft;
	vector<ZZ> cc11fft;

	vector<ZZ> mulC1fft;
	vector<ZZ> mulC0fft;

	ZRingUtils::mulRing(cc00fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtils::mulRing(cc01fft, cipher1.c0fft, cipher2.c1fft, qi, params.phim);
	ZRingUtils::mulRing(cc10fft, cipher1.c1fft, cipher2.c0fft, qi, params.phim);

	ZRingUtils::mulRing(cc11fft, cipher1.c1fft, cipher2.c1fft, Pqi, params.phim);

	ZRingUtils::mulRing(mulC1fft, publicKey.c1Starfft, cc11fft, Pqi, params.phim);
	ZRingUtils::mulRing(mulC0fft, publicKey.c0Starfft, cc11fft, Pqi, params.phim);

	ZRingUtils::divByConstantRing(mulC1fft, mulC1fft, params.P, qi, params.phim);
	ZRingUtils::divByConstantRing(mulC0fft, mulC0fft, params.P, qi, params.phim);

	ZRingUtils::addRing(mulC1fft, mulC1fft, cc10fft, qi, params.phim);
	ZRingUtils::addRing(mulC1fft, mulC1fft, cc01fft, qi, params.phim);
	ZRingUtils::addRing(mulC0fft, mulC0fft, cc00fft, qi, params.phim);

	FPHECipher cipher(mulC0fft, mulC1fft, cipher1.level);
	return cipher;
}

void FPHEScheme::mulAndEqual(FPHECipher& cipher1, FPHECipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	vector<ZZ> cc00fft;
	vector<ZZ> cc01fft;
	vector<ZZ> cc10fft;
	vector<ZZ> cc11fft;

	vector<ZZ> mulC1fft;
	vector<ZZ> mulC0fft;

	ZRingUtils::mulRing(cc00fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtils::mulRing(cc01fft, cipher1.c0fft, cipher2.c1fft, qi, params.phim);
	ZRingUtils::mulRing(cc10fft, cipher1.c1fft, cipher2.c0fft, qi, params.phim);

	ZRingUtils::mulRing(cc11fft, cipher1.c1fft, cipher2.c1fft, Pqi, params.phim);

	ZRingUtils::mulRing(mulC1fft, publicKey.c1Starfft, cc11fft, Pqi, params.phim);
	ZRingUtils::mulRing(mulC0fft, publicKey.c0Starfft, cc11fft, Pqi, params.phim);

	ZRingUtils::divByConstantRing(mulC1fft, mulC1fft, params.P, qi, params.phim);
	ZRingUtils::divByConstantRing(mulC0fft, mulC0fft, params.P, qi, params.phim);

	ZRingUtils::addRing(mulC1fft, mulC1fft, cc10fft, qi, params.phim);
	ZRingUtils::addRing(mulC1fft, mulC1fft, cc01fft, qi, params.phim);
	ZRingUtils::addRing(mulC0fft, mulC0fft, cc00fft, qi, params.phim);

	cipher1.c0fft = mulC0fft;
	cipher1.c1fft = mulC1fft;
}

FPHECipher FPHEScheme::addConstant(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> c0 = cipher.c0fft;
	vector<ZZ> c1 = cipher.c1fft;

	ZRingUtils::addConstantRing(c0, c0, cnst, qi, params.phim);
	FPHECipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

FPHECipher FPHEScheme::mulByConstant(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> c0;
	vector<ZZ> c1;
	ZRingUtils::mulByConstantRing(c0, cipher.c0fft, cnst, qi, params.phim);
	ZRingUtils::mulByConstantRing(c1, cipher.c1fft, cnst, qi, params.phim);
	FPHECipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

void FPHEScheme::addConstantAndEqual(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);
	ZRingUtils::addConstantRing(cipher.c0fft, cipher.c0fft, cnst, qi, params.phim);
}

void FPHEScheme::mulByConstantAndEqual(FPHECipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	ZRingUtils::mulByConstantRing(cipher.c0fft, cipher.c0fft, cnst, qi, params.phim);
	ZRingUtils::mulByConstantRing(cipher.c1fft, cipher.c1fft, cnst, qi, params.phim);
}

FPHECipher FPHEScheme::modSwitch(FPHECipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];

	vector<ZZ> c0;
	vector<ZZ> c1;

	ZRingUtils::divByConstantRing(c0, cipher.c0fft, divFactor, params.qL, params.phim);
	ZRingUtils::divByConstantRing(c1, cipher.c1fft, divFactor, params.qL, params.phim);
	FPHECipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FPHEScheme::modSwitchAndEqual(FPHECipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];
	ZRingUtils::divByConstantRing(cipher.c0fft, cipher.c0fft, divFactor, params.qL, params.phim);
	ZRingUtils::divByConstantRing(cipher.c1fft, cipher.c1fft, divFactor, params.qL, params.phim);
	cipher.level = newLevel;
}

FPHECipher FPHEScheme::modEmbed(FPHECipher& cipher, long newLevel) {
	vector<ZZ> c0 = cipher.c0fft;
	vector<ZZ> c1 = cipher.c1fft;
	FPHECipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FPHEScheme::modEmbedAndEqual(FPHECipher& cipher, long newLevel) {
	cipher.level = newLevel;
}

ZZ& FPHEScheme::getModulo(long level) {
	return params.qi[params.levels - level];
}

ZZ& FPHEScheme::getPqModulo(long level) {
	return params.Pqi[params.levels - level];
}
