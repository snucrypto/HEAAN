#include "FPHEFFTScheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>

#include "FPHEFFTCipher.h"
#include "ZRingUtilsFFT.h"

using namespace std;
using namespace NTL;

FPHEFFTCipher FPHEFFTScheme::encrypt(ZZ& m) {
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
			ZRingUtilsFFT::addFFTRing(c0fft, c0fft, lwefft0, params.qL, params.phim);
			ZRingUtilsFFT::addFFTRing(c1fft, c1fft, lwefft1, params.qL, params.phim);
		}
	}
	ZRingUtilsFFT::addFFTConstantRing(c0fft, c0fft, m, params.qL, params.phim);
	FPHEFFTCipher cipher(c0fft, c1fft, 1);
	return cipher;
}

ZZ FPHEFFTScheme::decrypt(FPHEFFTCipher& cipher) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> mfft;
	vector<ZZ> m;
	ZRingUtilsFFT::mulFFTRing(mfft, secretKey.sfft, cipher.c1fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(mfft, mfft, cipher.c0fft, qi, params.phim);

	ZRingUtilsFFT::convertfftInv(m, mfft, params.fftInv, qi, params.phim);

	ZZ c = m[0];
	if(c < 0) {c += qi;}
	return c;
}

FPHEFFTCipher FPHEFFTScheme::add(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	vector<ZZ> c0;
	vector<ZZ> c1;
	ZRingUtilsFFT::addFFTRing(c0, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(c1, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
	FPHEFFTCipher res(c0, c1, cipher1.level);
	return res;
}

void FPHEFFTScheme::addAndEqual(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);

	ZRingUtilsFFT::addFFTRing(cipher1.c0fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(cipher1.c1fft, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
}

FPHEFFTCipher FPHEFFTScheme::sub(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2) {
	ZZ& qi = getModulo(cipher1.level);
	vector<ZZ> c0;
	vector<ZZ> c1;
	ZRingUtilsFFT::subFFTRing(c0, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtilsFFT::subFFTRing(c1, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
	FPHEFFTCipher res(c0, c1, cipher1.level);
	return res;
}

void FPHEFFTScheme::subAndEqual(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZRingUtilsFFT::subFFTRing(cipher1.c0fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtilsFFT::subFFTRing(cipher1.c1fft, cipher1.c1fft, cipher2.c1fft, qi, params.phim);
}

FPHEFFTCipher FPHEFFTScheme::mul(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	vector<ZZ> cc00fft;
	vector<ZZ> cc01fft;
	vector<ZZ> cc10fft;
	vector<ZZ> cc11fft;

	vector<ZZ> mulC1fft;
	vector<ZZ> mulC0fft;

	ZRingUtilsFFT::mulFFTRing(cc00fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtilsFFT::mulFFTRing(cc01fft, cipher1.c0fft, cipher2.c1fft, qi, params.phim);
	ZRingUtilsFFT::mulFFTRing(cc10fft, cipher1.c1fft, cipher2.c0fft, qi, params.phim);

	ZRingUtilsFFT::mulFFTRing(cc11fft, cipher1.c1fft, cipher2.c1fft, Pqi, params.phim);

	ZRingUtilsFFT::mulFFTRing(mulC1fft, publicKey.c1Starfft, cc11fft, Pqi, params.phim);
	ZRingUtilsFFT::mulFFTRing(mulC0fft, publicKey.c0Starfft, cc11fft, Pqi, params.phim);

	ZRingUtilsFFT::divFFTByConstantRing(mulC1fft, mulC1fft, params.P, qi, params.phim);
	ZRingUtilsFFT::divFFTByConstantRing(mulC0fft, mulC0fft, params.P, qi, params.phim);

	ZRingUtilsFFT::addFFTRing(mulC1fft, mulC1fft, cc10fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(mulC1fft, mulC1fft, cc01fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(mulC0fft, mulC0fft, cc00fft, qi, params.phim);

	FPHEFFTCipher cipher(mulC0fft, mulC1fft, cipher1.level);
	return cipher;
}

void FPHEFFTScheme::mulAndEqual(FPHEFFTCipher& cipher1, FPHEFFTCipher& cipher2) {
	ZZ qi = getModulo(cipher1.level);
	ZZ Pqi = getPqModulo(cipher1.level);

	vector<ZZ> cc00fft;
	vector<ZZ> cc01fft;
	vector<ZZ> cc10fft;
	vector<ZZ> cc11fft;

	vector<ZZ> mulC1fft;
	vector<ZZ> mulC0fft;

	ZRingUtilsFFT::mulFFTRing(cc00fft, cipher1.c0fft, cipher2.c0fft, qi, params.phim);
	ZRingUtilsFFT::mulFFTRing(cc01fft, cipher1.c0fft, cipher2.c1fft, qi, params.phim);
	ZRingUtilsFFT::mulFFTRing(cc10fft, cipher1.c1fft, cipher2.c0fft, qi, params.phim);

	ZRingUtilsFFT::mulFFTRing(cc11fft, cipher1.c1fft, cipher2.c1fft, Pqi, params.phim);

	ZRingUtilsFFT::mulFFTRing(mulC1fft, publicKey.c1Starfft, cc11fft, Pqi, params.phim);
	ZRingUtilsFFT::mulFFTRing(mulC0fft, publicKey.c0Starfft, cc11fft, Pqi, params.phim);

	ZRingUtilsFFT::divFFTByConstantRing(mulC1fft, mulC1fft, params.P, qi, params.phim);
	ZRingUtilsFFT::divFFTByConstantRing(mulC0fft, mulC0fft, params.P, qi, params.phim);

	ZRingUtilsFFT::addFFTRing(mulC1fft, mulC1fft, cc10fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(mulC1fft, mulC1fft, cc01fft, qi, params.phim);
	ZRingUtilsFFT::addFFTRing(mulC0fft, mulC0fft, cc00fft, qi, params.phim);

	cipher1.c0fft = mulC0fft;
	cipher1.c1fft = mulC1fft;
}

FPHEFFTCipher FPHEFFTScheme::addConstant(FPHEFFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> c0 = cipher.c0fft;
	vector<ZZ> c1 = cipher.c1fft;

	ZRingUtilsFFT::addFFTConstantRing(c0, c0, cnst, qi, params.phim);
	FPHEFFTCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

FPHEFFTCipher FPHEFFTScheme::mulByConstant(FPHEFFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	vector<ZZ> c0;
	vector<ZZ> c1;
	ZRingUtilsFFT::mulFFTByConstantRing(c0, cipher.c0fft, cnst, qi, params.phim);
	ZRingUtilsFFT::mulFFTByConstantRing(c1, cipher.c1fft, cnst, qi, params.phim);
	FPHEFFTCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

void FPHEFFTScheme::addConstantAndEqual(FPHEFFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);
	ZRingUtilsFFT::addFFTConstantRing(cipher.c0fft, cipher.c0fft, cnst, qi, params.phim);
}

void FPHEFFTScheme::mulByConstantAndEqual(FPHEFFTCipher& cipher, ZZ& cnst) {
	ZZ& qi = getModulo(cipher.level);

	ZRingUtilsFFT::mulFFTByConstantRing(cipher.c0fft, cipher.c0fft, cnst, qi, params.phim);
	ZRingUtilsFFT::mulFFTByConstantRing(cipher.c1fft, cipher.c1fft, cnst, qi, params.phim);
}

FPHEFFTCipher FPHEFFTScheme::modSwitch(FPHEFFTCipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];

	vector<ZZ> c0;
	vector<ZZ> c1;

	ZRingUtilsFFT::divFFTByConstantRing(c0, cipher.c0fft, divFactor, params.qL, params.phim);
	ZRingUtilsFFT::divFFTByConstantRing(c1, cipher.c1fft, divFactor, params.qL, params.phim);
	FPHEFFTCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FPHEFFTScheme::modSwitchAndEqual(FPHEFFTCipher& cipher, long newLevel) {
	ZZ& divFactor = params.qi[newLevel-cipher.level-1];
	ZRingUtilsFFT::divFFTByConstantRing(cipher.c0fft, cipher.c0fft, divFactor, params.qL, params.phim);
	ZRingUtilsFFT::divFFTByConstantRing(cipher.c1fft, cipher.c1fft, divFactor, params.qL, params.phim);
	cipher.level = newLevel;
}

FPHEFFTCipher FPHEFFTScheme::modEmbed(FPHEFFTCipher& cipher, long newLevel) {
	vector<ZZ> c0 = cipher.c0fft;
	vector<ZZ> c1 = cipher.c1fft;
	FPHEFFTCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void FPHEFFTScheme::modEmbedAndEqual(FPHEFFTCipher& cipher, long newLevel) {
	cipher.level = newLevel;
}

ZZ& FPHEFFTScheme::getModulo(long level) {
	return params.qi[params.levels - level];
}

ZZ& FPHEFFTScheme::getPqModulo(long level) {
	return params.Pqi[params.levels - level];
}
