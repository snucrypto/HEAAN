#include "FFTScheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>

#include "../utils/FFTRingUtils.h"
#include "FFTCipher.h"

using namespace std;
using namespace NTL;

FFTCipher FFTScheme::encrypt(ZZ& m) {
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
			FFTRingUtils::addFFTRing(c0fft, c0fft, lwefft0, params.qL, params.phim);
			FFTRingUtils::addFFTRing(c1fft, c1fft, lwefft1, params.qL, params.phim);
		}
	}
	FFTRingUtils::addFFTConstantRing(c0fft, c0fft, m, params.qL, params.phim);
	FFTCipher cipher(c0fft, c1fft, 1);
	return cipher;
}

ZZ FFTScheme::decrypt(FFTCipher& cipher) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> mfft;
	vector<ZZ> m;
	FFTRingUtils::mulFFTRing(mfft, secretKey.sfft, cipher.c1fft, qi, params.phim);
	FFTRingUtils::addFFTRing(mfft, mfft, cipher.c0fft, qi, params.phim);

	FFTRingUtils::convertfftInv(m, mfft, params.fftInv, qi, params.phim);

	ZZ c = m[0];
	if(c < 0) {c += qi;}
	return c;
}

FFTCipher FFTScheme::add(FFTCipher& cipher1, FFTCipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	vector<ZZ> c0;
	vector<ZZ> c1;
	FFTRingUtils::addFFTRing(c0, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	FFTRingUtils::addFFTRing(c1, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
	FFTCipher res(c0, c1, cipher1.level);
	return res;
}

void FFTScheme::addAndEqual(FFTCipher& cipher1, FFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);

	FFTRingUtils::addFFTRing(cipher1.c0fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	FFTRingUtils::addFFTRing(cipher1.c1fft, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
}

FFTCipher FFTScheme::sub(FFTCipher& cipher1, FFTCipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	vector<ZZ> c0;
	vector<ZZ> c1;
	FFTRingUtils::subFFTRing(c0, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	FFTRingUtils::subFFTRing(c1, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
	FFTCipher res(c0, c1, cipher1.level);
	return res;
}

void FFTScheme::subAndEqual(FFTCipher& cipher1, FFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	FFTRingUtils::subFFTRing(cipher1.c0fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	FFTRingUtils::subFFTRing(cipher1.c1fft, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
}

FFTCipher FFTScheme::mul(FFTCipher& cipher1, FFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	vector<ZZ> cc00fft;
	vector<ZZ> cc01fft;
	vector<ZZ> cc10fft;
	vector<ZZ> cc11fft;

	vector<ZZ> mulC1fft;
	vector<ZZ> mulC0fft;

	FFTRingUtils::mulFFTRing(cc00fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	FFTRingUtils::mulFFTRing(cc01fft, cipher1.c0fft, cipher2.c1fft, qi, params.phim);
	FFTRingUtils::mulFFTRing(cc10fft, cipher1.c1fft, cipher2.c0fft, qi, params.phim);

	FFTRingUtils::mulFFTRing(cc11fft, cipher1.c1fft, cipher2.c1fft, Pqi, params.phim);

	FFTRingUtils::mulFFTRing(mulC1fft, publicKey.c1Starfft, cc11fft, Pqi, params.phim);
	FFTRingUtils::mulFFTRing(mulC0fft, publicKey.c0Starfft, cc11fft, Pqi, params.phim);

	FFTRingUtils::divFFTByConstantRing(mulC1fft, mulC1fft, params.P, qi, params.phim);
	FFTRingUtils::divFFTByConstantRing(mulC0fft, mulC0fft, params.P, qi, params.phim);

	FFTRingUtils::addFFTRing(mulC1fft, mulC1fft, cc10fft, qi, params.phim);
	FFTRingUtils::addFFTRing(mulC1fft, mulC1fft, cc01fft, qi, params.phim);
	FFTRingUtils::addFFTRing(mulC0fft, mulC0fft, cc00fft, qi, params.phim);

	FFTCipher cipher(mulC0fft, mulC1fft, cipher1.level);
	return cipher;
}

void FFTScheme::mulAndEqual(FFTCipher& cipher1, FFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	vector<ZZ> cc00fft;
	vector<ZZ> cc01fft;
	vector<ZZ> cc10fft;
	vector<ZZ> cc11fft;

	vector<ZZ> mulC1fft;
	vector<ZZ> mulC0fft;

	FFTRingUtils::mulFFTRing(cc00fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	FFTRingUtils::mulFFTRing(cc01fft, cipher1.c0fft, cipher2.c1fft, qi, params.phim);
	FFTRingUtils::mulFFTRing(cc10fft, cipher1.c1fft, cipher2.c0fft, qi, params.phim);

	FFTRingUtils::mulFFTRing(cc11fft, cipher1.c1fft, cipher2.c1fft, Pqi, params.phim);

	FFTRingUtils::mulFFTRing(mulC1fft, publicKey.c1Starfft, cc11fft, Pqi, params.phim);
	FFTRingUtils::mulFFTRing(mulC0fft, publicKey.c0Starfft, cc11fft, Pqi, params.phim);

	FFTRingUtils::divFFTByConstantRing(mulC1fft, mulC1fft, params.P, qi, params.phim);
	FFTRingUtils::divFFTByConstantRing(mulC0fft, mulC0fft, params.P, qi, params.phim);

	FFTRingUtils::addFFTRing(mulC1fft, mulC1fft, cc10fft, qi, params.phim);
	FFTRingUtils::addFFTRing(mulC1fft, mulC1fft, cc01fft, qi, params.phim);
	FFTRingUtils::addFFTRing(mulC0fft, mulC0fft, cc00fft, qi, params.phim);

	cipher1.c0fft = mulC0fft;
	cipher1.c1fft = mulC1fft;
}

FFTCipher FFTScheme::addConstant(FFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> c0 = cipher.c0fft;
	vector<ZZ> c1 = cipher.c1fft;

	FFTRingUtils::addFFTConstantRing(c0, c0, cnst, qi, params.phim);
	FFTCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

FFTCipher FFTScheme::mulByConstant(FFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> c0;
	vector<ZZ> c1;
	FFTRingUtils::mulFFTByConstantRing(c0, cipher.c0fft, cnst, qi, params.phim);
	FFTRingUtils::mulFFTByConstantRing(c1, cipher.c1fft, cnst, qi, params.phim);
	FFTCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

void FFTScheme::addConstantAndEqual(FFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);
	FFTRingUtils::addFFTConstantRing(cipher.c0fft, cipher.c0fft, cnst, qi, params.phim);
}

void FFTScheme::mulByConstantAndEqual(FFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	FFTRingUtils::mulFFTByConstantRing(cipher.c0fft, cipher.c0fft, cnst, qi, params.phim);
	FFTRingUtils::mulFFTByConstantRing(cipher.c1fft, cipher.c1fft, cnst, qi, params.phim);
}

FFTCipher FFTScheme::modSwitch(FFTCipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];

	vector<ZZ> c0;
	vector<ZZ> c1;

	FFTRingUtils::divFFTByConstantRing(c0, cipher.c0fft, divFactor, params.qL, params.phim);
	FFTRingUtils::divFFTByConstantRing(c1, cipher.c1fft, divFactor, params.qL, params.phim);
	FFTCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FFTScheme::modSwitchAndEqual(FFTCipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];
	FFTRingUtils::divFFTByConstantRing(cipher.c0fft, cipher.c0fft, divFactor, params.qL, params.phim);
	FFTRingUtils::divFFTByConstantRing(cipher.c1fft, cipher.c1fft, divFactor, params.qL, params.phim);
	cipher.level = newLevel;
}

FFTCipher FFTScheme::modEmbed(FFTCipher& cipher, long newLevel) {
	vector<ZZ> c0 = cipher.c0fft;
	vector<ZZ> c1 = cipher.c1fft;
	FFTCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FFTScheme::modEmbedAndEqual(FFTCipher& cipher, long newLevel) {
	cipher.level = newLevel;
}

ZZ& FFTScheme::getModulo(long level) {
	return params.qi[params.levels - level];
}

ZZ& FFTScheme::getPqModulo(long level) {
	return params.Pqi[params.levels - level];
}
