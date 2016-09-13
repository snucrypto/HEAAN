#include "Scheme.h"

#include <NTL/ZZ.h>
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"
#include "../utils/CPolyRingUtils.h"

using namespace std;
using namespace NTL;

Cipher Scheme::encrypt(ZZ& m) {
	CZZ tmp;

	CZZX v;
	CZZX c0, c1;
	CZZX e;

	CPolyRingUtils::sampleZO(v, params.rho, params.n);


	CPolyRingUtils::mulPolyRing2(c0, v, publicKey.b, params.logq, params.n);
	CPolyRingUtils::sampleGaussian(e, params.n, params.sigma);
	CPolyRingUtils::addPolyRing2(c0, e, c0, params.logq, params.n);

	CPolyRingUtils::mulPolyRing2(c1, v, publicKey.a, params.logq, params.n);
	CPolyRingUtils::sampleGaussian(e, params.n, params.sigma);
	CPolyRingUtils::addPolyRing2(c1, e, c1, params.logq, params.n);

	tmp = coeff(c0, 0) + m;
	CPolyRingUtils::truncate(tmp, params.logq);
	SetCoeff(c0, 0, tmp);
	c0.normalize();
	Cipher cipher(c0, c1, 1, params.Bclean, m);
	return cipher;
}

ZZ Scheme::decrypt(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);

	CZZX m;
	CPolyRingUtils::mulPolyRing2(m, secretKey.s, cipher.c1, logQi, params.n);
	CPolyRingUtils::addPolyRing2(m, m, cipher.c0, logQi, params.n);
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
	CPolyRingUtils::addPolyRing2(c0, cipher1.c0, cipher2.c0, logQi, params.n);
	CPolyRingUtils::addPolyRing2(c1, cipher1.c1, cipher2.c1, logQi, params.n);

	ZZ B = cipher1.B + cipher2.B;
	ZZ nu = cipher1.nu + cipher2.nu;

	Cipher res(c0, c1, cipher1.level, B, nu);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CPolyRingUtils::addPolyRing2(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.n);
	CPolyRingUtils::addPolyRing2(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.n);

	cipher1.B += cipher2.B;
	cipher1.nu += cipher2.nu;
}

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CZZX c0;
	CZZX c1;
	CPolyRingUtils::subPolyRing2(c0, cipher1.c0, cipher2.c0, logQi, params.n);
	CPolyRingUtils::subPolyRing2(c1, cipher1.c1, cipher2.c1, logQi, params.n);

	ZZ B = cipher1.B + cipher2.B;
	ZZ nu = cipher1.nu + cipher2.nu;

	Cipher res(c0, c1, cipher1.level, B, nu);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CPolyRingUtils::subPolyRing2(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.n);
	CPolyRingUtils::subPolyRing2(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.n);

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

	CPolyRingUtils::mulPolyRing2(cc00, cipher1.c0, cipher2.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc01, cipher1.c0, cipher2.c1, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc10, cipher1.c1, cipher2.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc11, cipher1.c1, cipher2.c1, logQi, params.n);

	CPolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.n);
	CPolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.n);

	CPolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logP, logQi, params.n);
	CPolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logP, logQi, params.n);

	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc01, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.n);

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

	CPolyRingUtils::mulPolyRing2(cc00, cipher1.c0, cipher2.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc01, cipher1.c0, cipher2.c1, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc10, cipher1.c1, cipher2.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc11, cipher1.c1, cipher2.c1, logQi, params.n);

	CPolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.n);
	CPolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.n);

	CPolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logP, logQi, params.n);
	CPolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logP, logQi, params.n);

	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc01, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.n);

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
	for (i = 0; i < params.n; ++i) {
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
	for (i = 0; i < params.n; ++i) {
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
	long logDF = params.logp * (newLevel-cipher.level);

	CZZX c0;
	CZZX c1;
	long i;
	for (i = 0; i < params.n; ++i) {
		CZZ shift0 = coeff(cipher.c0, i) >> logDF;
		SetCoeff(c0, i, shift0);
		CZZ shift1 = coeff(cipher.c1, i) >> logDF;
		SetCoeff(c1, i, shift1);
	}
	c0.normalize();
	c1.normalize();

	ZZ B = (cipher.B >> params.logp) + params.Bscale;
	ZZ nu = cipher.nu >> params.logp;

	Cipher newCipher(c0, c1, newLevel, B, nu);
	return newCipher;
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long newLevel) {
	long logDF = params.logp * (newLevel-cipher.level);

	long i;
	for (i = 0; i < params.n; ++i) {
		CZZ shift0 = coeff(cipher.c0, i) >> logDF;
		SetCoeff(cipher.c0, i, shift0);
		CZZ shift1 = coeff(cipher.c1, i) >> logDF;
		SetCoeff(cipher.c1, i, shift1);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;

	cipher.B >>= params.logp;
	cipher.B += params.Bscale;
	cipher.nu >>= params.logp;
}

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	CZZ tmp;
	long newQi = getLogQi(newLevel);
	CZZX c0;
	CZZX c1;
	long i;
	for (i = 0; i < params.n; ++i) {
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
	for (i = 0; i < params.n; ++i) {
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
	return params.logq - params.logp * (level-1);
}

long Scheme::getLogTQi(long& level) {
	return params.logPq - params.logp * (level-1);
}
