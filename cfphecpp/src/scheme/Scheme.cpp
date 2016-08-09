#include "Scheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>

#include "../utils/PolyRingUtils.h"
#include "Cipher.h"

using namespace std;
using namespace NTL;

PolyCipher PolyScheme::encrypt(ZZ& m) {
	ZZ tmp;
	RandomBits(tmp, params.tau);
	ZZX c0;
	ZZX c1;

	long i;
	for (i = 0; i < params.tau; ++i) {
		if(bit(tmp, i)) {
			ZZX lwe0 = publicKey.A0[i];
			ZZX lwe1 = publicKey.A1[i];

			PolyRingUtils::addPolyRing2(c0, c0, lwe0, params.logQ, params.phim);
			PolyRingUtils::addPolyRing2(c1, c1, lwe1, params.logQ, params.phim);
		}
	}
	tmp = coeff(c0, 0) + m;
	PolyRingUtils::truncate(tmp, params.logQ);
	SetCoeff(c0, 0, tmp);
	c0.normalize();
	PolyCipher cipher(c0, c1, 1);
	return cipher;
}

ZZ PolyScheme::decrypt(PolyCipher& cipher) {
	long logQi = getLogQi(cipher.level);

	ZZX m;
	PolyRingUtils::mulPolyRing2(m, secretKey.s, cipher.c1, logQi, params.phim);
	PolyRingUtils::addPolyRing2(m, m, cipher.c0, logQi, params.phim);
	ZZ c, tmp;
	GetCoeff(c, m, 0);
	while(c < 0) {
		tmp = 1;
		tmp <<= logQi;
		c += tmp;
	}
	return c;
}

PolyCipher PolyScheme::add(PolyCipher& cipher1, PolyCipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	ZZX c0;
	ZZX c1;
	PolyRingUtils::addPolyRing2(c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	PolyRingUtils::addPolyRing2(c1, cipher1.c1, cipher2.c1, logQi, params.phim);
	PolyCipher res(c0, c1, cipher1.level);
	return res;
}

void PolyScheme::addAndEqual(PolyCipher& cipher1, PolyCipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	PolyRingUtils::addPolyRing2(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	PolyRingUtils::addPolyRing2(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.phim);
}

PolyCipher PolyScheme::sub(PolyCipher& cipher1, PolyCipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	ZZX c0;
	ZZX c1;
	PolyRingUtils::subPolyRing2(c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	PolyRingUtils::subPolyRing2(c1, cipher1.c1, cipher2.c1, logQi, params.phim);
	PolyCipher res(c0, c1, cipher1.level);
	return res;
}

void PolyScheme::subAndEqual(PolyCipher& cipher1, PolyCipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	PolyRingUtils::subPolyRing2(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	PolyRingUtils::subPolyRing2(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.phim);
}

PolyCipher PolyScheme::mul(PolyCipher& cipher1, PolyCipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	long logTQi = getLogTQi(cipher1.level);

	ZZX cc00;
	ZZX cc01;
	ZZX cc10;
	ZZX cc11;

	ZZX mulC1;
	ZZX mulC0;
	PolyRingUtils::mulPolyRing2(cc00, cipher1.c0, cipher2.c0, logQi, params.phim);
	PolyRingUtils::mulPolyRing2(cc01, cipher1.c0, cipher2.c1, logQi, params.phim);
	PolyRingUtils::mulPolyRing2(cc10, cipher1.c1, cipher2.c0, logQi, params.phim);
	PolyRingUtils::mulPolyRing2(cc11, cipher1.c1, cipher2.c1, logQi, params.phim);

	PolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.phim);
	PolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.phim);

	PolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logT, logQi, params.phim);
	PolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logT, logQi, params.phim);

	PolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.phim);
	PolyRingUtils::addPolyRing2(mulC1, mulC1, cc01, logQi, params.phim);
	PolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.phim);

	PolyCipher cipher(mulC0, mulC1, cipher1.level);
	return cipher;
}

void PolyScheme::mulAndEqual(PolyCipher& cipher1, PolyCipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	long logTQi = getLogTQi(cipher1.level);

	ZZX cc00;
	ZZX cc01;
	ZZX cc10;
	ZZX cc11;

	ZZX mulC1;
	ZZX mulC0;

	PolyRingUtils::mulPolyRing2(cc00, cipher1.c0, cipher2.c0, logQi, params.phim);
	PolyRingUtils::mulPolyRing2(cc01, cipher1.c0, cipher2.c1, logQi, params.phim);
	PolyRingUtils::mulPolyRing2(cc10, cipher1.c1, cipher2.c0, logQi, params.phim);
	PolyRingUtils::mulPolyRing2(cc11, cipher1.c1, cipher2.c1, logQi, params.phim);

	PolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.phim);
	PolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.phim);

	PolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logT, logQi, params.phim);
	PolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logT, logQi, params.phim);

	PolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.phim);
	PolyRingUtils::addPolyRing2(mulC1, mulC1, cc01, logQi, params.phim);
	PolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.phim);

	cipher1.c0 = mulC0;
	cipher1.c1 = mulC1;
}

PolyCipher PolyScheme::addConstant(PolyCipher& cipher, ZZ& cnst) {
	long logQi = getLogQi(cipher.level);
	ZZ tmp;
	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;
	tmp = coeff(cipher.c1,0) + cnst;
	PolyRingUtils::truncate(tmp, logQi);
	SetCoeff(c1, 0, tmp);
	c0.normalize();
	PolyCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

PolyCipher PolyScheme::mulByConstant(PolyCipher& cipher, ZZ& cnst) {
	long logQi = getLogQi(cipher.level);
	ZZ tmp;
	ZZX c0;
	ZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		PolyRingUtils::truncate(tmp, logQi);
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		PolyRingUtils::truncate(tmp, logQi);
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();
	PolyCipher newCipher(c0, c1, cipher.level);
	return newCipher;
}

void PolyScheme::addConstantAndEqual(PolyCipher& cipher, ZZ& cnst) {
	ZZ tmp;
	long logQi = getLogQi(cipher.level);
	tmp = coeff(cipher.c0, 0) * cnst;
	PolyRingUtils::truncate(tmp, logQi);
	SetCoeff(cipher.c0, 0, tmp);
	cipher.c0.normalize();
}

void PolyScheme::mulByConstantAndEqual(PolyCipher& cipher, ZZ& cnst) {
	ZZ tmp;
	long logQi = getLogQi(cipher.level);

	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		PolyRingUtils::truncate(tmp, logQi);
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		PolyRingUtils::truncate(tmp, logQi);
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
}

PolyCipher PolyScheme::modSwitch(PolyCipher& cipher, long newLevel) {
	long logDF = params.logP * (newLevel-cipher.level);

	ZZX c0;
	ZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(c0, i, coeff(cipher.c0,i) >> logDF);
		SetCoeff(c1, i, coeff(cipher.c1,i) >> logDF);
	}
	c0.normalize();
	c1.normalize();
	PolyCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void PolyScheme::modSwitchAndEqual(PolyCipher& cipher, long newLevel) {
	long logDF = params.logP * (newLevel-cipher.level);

	long i;
	for (i = 0; i < params.phim; ++i) {
		SetCoeff(cipher.c0, i, coeff(cipher.c0,i) >> logDF);
		SetCoeff(cipher.c1, i, coeff(cipher.c1,i) >> logDF);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;
}

PolyCipher PolyScheme::modEmbed(PolyCipher& cipher, long newLevel) {
	ZZ tmp;
	long newQi = getLogQi(newLevel);
	ZZX c0;
	ZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i);
		PolyRingUtils::truncate(tmp, newQi);
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		PolyRingUtils::truncate(tmp, newQi);
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();
	PolyCipher newCipher(c0, c1, newLevel);
	return newCipher;
}

void PolyScheme::modEmbedAndEqual(PolyCipher& cipher, long newLevel) {
	ZZ tmp;
	long newQi = getLogQi(newLevel);

	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i);
		PolyRingUtils::truncate(tmp, newQi);
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		PolyRingUtils::truncate(tmp, newQi);
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;
}

long PolyScheme::getLogQi(long& level) {
	return params.logQ - params.logP * (level-1);
}

long PolyScheme::getLogTQi(long& level) {
	return params.logTQ - params.logP * (level-1);
}
