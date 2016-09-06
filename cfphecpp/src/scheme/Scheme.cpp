#include "Scheme.h"

#include <NTL/ZZ.h>
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"
#include "../utils/CPolyRingUtils.h"

using namespace std;
using namespace NTL;

Cipher Scheme::encrypt(ZZ& m) {
	ZZ tmp;
	CZZ tmp2;
	RandomBits(tmp, params.tau);
	CZZX c0;
	CZZX c1;

	long i;
	for (i = 0; i < params.tau; ++i) {
		if(bit(tmp, i)) {
			CZZX lwe0 = publicKey.A0[i];
			CZZX lwe1 = publicKey.A1[i];

			CPolyRingUtils::addPolyRing2(c0, c0, lwe0, params.logQ, params.phim);
			CPolyRingUtils::addPolyRing2(c1, c1, lwe1, params.logQ, params.phim);
		}
	}
	tmp2 = coeff(c0, 0) + m;
	CPolyRingUtils::truncate(tmp2, params.logQ);
	SetCoeff(c0, 0, tmp2);
	c0.normalize();
	Cipher cipher(c0, c1, 1, params.B, params.nu);
	return cipher;
}

ZZ Scheme::decrypt(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);

	CZZX m;
	CPolyRingUtils::mulPolyRing2(m, secretKey.s, cipher.c1, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(m, m, cipher.c0, logQi, params.phim);
	CZZ c;
	ZZ tmp;
	GetCoeff(c, m, 0);
	while(c.r < 0) {
		tmp = 1;
		tmp <<= logQi;
		c.r += tmp;
	}
	return c.r;
}

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CZZX c0;
	CZZX c1;
	CPolyRingUtils::addPolyRing2(c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(c1, cipher1.c1, cipher2.c1, logQi, params.phim);

	ZZ B = cipher1.B + cipher2.B;
	ZZ nu = cipher1.nu + cipher2.nu;

	Cipher res(c0, c1, cipher1.level, B, nu);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CPolyRingUtils::addPolyRing2(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.phim);

	cipher1.B += cipher2.B;
	cipher1.nu += cipher2.nu;
}

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CZZX c0;
	CZZX c1;
	CPolyRingUtils::subPolyRing2(c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::subPolyRing2(c1, cipher1.c1, cipher2.c1, logQi, params.phim);

	ZZ B = cipher1.B + cipher2.B;
	ZZ nu = cipher1.nu + cipher2.nu;

	Cipher res(c0, c1, cipher1.level, B, nu);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CPolyRingUtils::subPolyRing2(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::subPolyRing2(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.phim);

	cipher1.B += cipher2.B;
	cipher1.nu += cipher2.nu;
}

Cipher Scheme::mul(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	long logTQi = getLogTQi(cipher1.level);

	CZZX cc00;
	CZZX cc01;
	CZZX cc10;
	CZZX cc11;

	CZZX mulC1;
	CZZX mulC0;

	CPolyRingUtils::mulPolyRing2(cc00, cipher1.c0, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::mulPolyRing2(cc01, cipher1.c0, cipher2.c1, logQi, params.phim);
	CPolyRingUtils::mulPolyRing2(cc10, cipher1.c1, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::mulPolyRing2(cc11, cipher1.c1, cipher2.c1, logQi, params.phim);

	CPolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.phim);
	CPolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.phim);

	CPolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logT, logQi, params.phim);
	CPolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logT, logQi, params.phim);

	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc01, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.phim);

	ZZ B = cipher1.B * cipher2.nu + cipher1.nu * cipher2.B + cipher1.B * cipher2.B;
	ZZ nu = cipher1.nu * cipher2.nu;

	Cipher cipher(mulC0, mulC1, cipher1.level, B, nu);
	return cipher;
}

void Scheme::mulAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	long logTQi = getLogTQi(cipher1.level);

	CZZX cc00;
	CZZX cc01;
	CZZX cc10;
	CZZX cc11;

	CZZX mulC1;
	CZZX mulC0;

	CPolyRingUtils::mulPolyRing2(cc00, cipher1.c0, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::mulPolyRing2(cc01, cipher1.c0, cipher2.c1, logQi, params.phim);
	CPolyRingUtils::mulPolyRing2(cc10, cipher1.c1, cipher2.c0, logQi, params.phim);
	CPolyRingUtils::mulPolyRing2(cc11, cipher1.c1, cipher2.c1, logQi, params.phim);

	CPolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.phim);
	CPolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.phim);

	CPolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logT, logQi, params.phim);
	CPolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logT, logQi, params.phim);

	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc01, logQi, params.phim);
	CPolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.phim);

	cipher1.c0 = mulC0;
	cipher1.c1 = mulC1;

	cipher1.B = cipher1.B * cipher2.nu + cipher1.nu * cipher2.B + cipher1.B * cipher2.B;
	cipher1.nu = cipher1.nu * cipher2.nu;
}

Cipher Scheme::addConstant(Cipher& cipher, ZZ& cnst) {
	long logQi = getLogQi(cipher.level);
	CZZ tmp;
	CZZX c0 = cipher.c0;
	CZZX c1 = cipher.c1;
	tmp = coeff(cipher.c1,0) + cnst;
	CPolyRingUtils::truncate(tmp, logQi);
	SetCoeff(c1, 0, tmp);
	c0.normalize();

	ZZ B = cipher.B + 1;
	ZZ nu = cipher.nu + cnst;

	Cipher newCipher(c0, c1, cipher.level, B, nu);
	return newCipher;
}

Cipher Scheme::mulByConstant(Cipher& cipher, ZZ& cnst) {
	long logQi = getLogQi(cipher.level);
	CZZ tmp;
	CZZX c0;
	CZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		CPolyRingUtils::truncate(tmp, logQi);
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		CPolyRingUtils::truncate(tmp, logQi);
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();

	ZZ B = cipher.B * cnst;
	ZZ nu = cipher.nu * cnst;

	Cipher newCipher(c0, c1, cipher.level, B, nu);
	return newCipher;
}

void Scheme::addConstantAndEqual(Cipher& cipher, ZZ& cnst) {
	CZZ tmp;
	long logQi = getLogQi(cipher.level);
	tmp = coeff(cipher.c0, 0) * cnst;
	CPolyRingUtils::truncate(tmp, logQi);
	SetCoeff(cipher.c0, 0, tmp);
	cipher.c0.normalize();
}

void Scheme::mulByConstantAndEqual(Cipher& cipher, ZZ& cnst) {
	CZZ tmp;
	long logQi = getLogQi(cipher.level);

	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		CPolyRingUtils::truncate(tmp, logQi);
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		CPolyRingUtils::truncate(tmp, logQi);
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();

	cipher.B *= cnst;
	cipher.nu *= cnst;
}

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logDF = params.logP * (newLevel-cipher.level);

	CZZX c0;
	CZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		CZZ shift0 = coeff(cipher.c0, i) >> logDF;
		SetCoeff(c0, i, shift0);
		CZZ shift1 = coeff(cipher.c1, i) >> logDF;
		SetCoeff(c1, i, shift1);
	}
	c0.normalize();
	c1.normalize();

	ZZ Bscale = to_ZZ(0);
	ZZ B = (cipher.B >> params.logP) + Bscale;
	ZZ nu = cipher.nu >> params.logP;

	Cipher newCipher(c0, c1, newLevel, B, nu);
	return newCipher;
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long newLevel) {
	long logDF = params.logP * (newLevel-cipher.level);

	long i;
	for (i = 0; i < params.phim; ++i) {
		CZZ shift0 = coeff(cipher.c0, i) >> logDF;
		SetCoeff(cipher.c0, i, shift0);
		CZZ shift1 = coeff(cipher.c1, i) >> logDF;
		SetCoeff(cipher.c1, i, shift1);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;

	ZZ Bscale = to_ZZ(0);
	cipher.B >>= params.logP;
	cipher.B += Bscale;
	cipher.nu >>= params.logP;
}

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	CZZ tmp;
	long newQi = getLogQi(newLevel);
	CZZX c0;
	CZZX c1;
	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i);
		CPolyRingUtils::truncate(tmp, newQi);
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		CPolyRingUtils::truncate(tmp, newQi);
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();

	ZZ B = cipher.B;
	ZZ nu = cipher.nu;

	Cipher newCipher(c0, c1, newLevel, B, nu);
	return newCipher;
}

void Scheme::modEmbedAndEqual(Cipher& cipher, long newLevel) {
	CZZ tmp;
	long newQi = getLogQi(newLevel);

	long i;
	for (i = 0; i < params.phim; ++i) {
		tmp = coeff(cipher.c0,i);
		CPolyRingUtils::truncate(tmp, newQi);
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		CPolyRingUtils::truncate(tmp, newQi);
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;
}

long Scheme::getLogQi(long& level) {
	return params.logQ - params.logP * (level-1);
}

long Scheme::getLogTQi(long& level) {
	return params.logTQ - params.logP * (level-1);
}
