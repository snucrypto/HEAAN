#include "Scheme.h"

#include <NTL/ZZ.h>
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"
#include "../utils/CPolyRingUtils.h"

using namespace std;
using namespace NTL;

Cipher Scheme::encrypt(CZZ& m, ZZ& nu) {
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
	Cipher cipher(c0, c1, 1, params.Bclean, nu);
	return cipher;
}

CZZ Scheme::decrypt(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	ZZ qi = getQi(cipher.level);

	CZZX m;
	CPolyRingUtils::mulPolyRing2(m, secretKey.s, cipher.c1, logQi, params.n);
	CPolyRingUtils::addPolyRing2(m, m, cipher.c0, logQi, params.n);
	CZZ c;
	GetCoeff(c, m, 0);
	return c;
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

Cipher Scheme::square(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	long logTQi = getLogTQi(cipher.level);

	CZZX cc00;
	CZZX cc10;
	CZZX cc11;

	CZZX mulC1;
	CZZX mulC0;

	CPolyRingUtils::mulPolyRing2(cc00, cipher.c0, cipher.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc10, cipher.c1, cipher.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc11, cipher.c1, cipher.c1, logQi, params.n);

	CPolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.n);
	CPolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.n);

	CPolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logP, logQi, params.n);
	CPolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logP, logQi, params.n);

	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.n);

	ZZ B = 2 * cipher.B * cipher.nu + cipher.B * cipher.B;
	ZZ nu = cipher.nu * cipher.nu;

	Cipher c(mulC0, mulC1, cipher.level, B, nu);
	return c;
}

Cipher Scheme::mulAndModSwitch(Cipher& cipher1, Cipher& cipher2) {
	Cipher c = mul(cipher1, cipher2);
	Cipher cms = modSwitch(c, c.level + 1);
	return cms;
}

Cipher Scheme::squareAndModSwitch(Cipher& cipher) {
	Cipher c = square(cipher);
	Cipher cms = modSwitch(c, c.level + 1);
	return cms;
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

void Scheme::squareAndEqual(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	long logTQi = getLogTQi(cipher.level);

	CZZX cc00;
	CZZX cc10;
	CZZX cc11;

	CZZX mulC1;
	CZZX mulC0;

	CPolyRingUtils::mulPolyRing2(cc00, cipher.c0, cipher.c0, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc10, cipher.c0, cipher.c1, logQi, params.n);
	CPolyRingUtils::mulPolyRing2(cc11, cipher.c1, cipher.c1, logQi, params.n);

	CPolyRingUtils::mulPolyRing2(mulC1, publicKey.aStar, cc11, logTQi, params.n);
	CPolyRingUtils::mulPolyRing2(mulC0, publicKey.bStar, cc11, logTQi, params.n);

	CPolyRingUtils::rightShiftPolyRing2(mulC1, mulC1, params.logP, logQi, params.n);
	CPolyRingUtils::rightShiftPolyRing2(mulC0, mulC0, params.logP, logQi, params.n);

	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC1, mulC1, cc10, logQi, params.n);
	CPolyRingUtils::addPolyRing2(mulC0, mulC0, cc00, logQi, params.n);

	cipher.c0 = mulC0;
	cipher.c1 = mulC1;

	cipher.B *= 2 * cipher.nu + cipher.B;
	cipher.nu *= cipher.nu;
}

Cipher Scheme::addConstant(Cipher& cipher, ZZ& cnst) {
	long logQi = getLogQi(cipher.level);
	CZZ tmp;
	CZZX c0 = cipher.c0;
	CZZX c1 = cipher.c1;
	tmp = coeff(cipher.c0,0) + cnst;
	CPolyRingUtils::truncate(tmp, logQi);
	SetCoeff(c0, 0, tmp);
	c0.normalize();

	ZZ B = cipher.B + 1;
	ZZ nu = cipher.nu + cnst;

	Cipher newCipher(c0, c1, cipher.level, B, nu);
	return newCipher;
}

Cipher Scheme::addConstant(Cipher& cipher, CZZ& cnst) {
	ZZ qi = getQi(cipher.level);
	CZZ tmp;
	CZZX c0 = cipher.c0;
	CZZX c1 = cipher.c1;
	tmp = coeff(cipher.c0,0) + cnst;
	tmp.r %= qi;
	tmp.i %= qi;
	SetCoeff(c0, 0, tmp);
	c0.normalize();

	ZZ norm = cnst.norm();

	ZZ B = cipher.B + 1;
	ZZ nu = cipher.nu + norm;

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

Cipher Scheme::mulByConstant(Cipher& cipher, CZZ& cnst) {
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

	ZZ norm = cnst.norm();

	ZZ B = cipher.B * norm;
	ZZ nu = cipher.nu * norm;

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
	ZZ newQi = getQi(newLevel);
	CZZ tmp;
	CZZX c0;
	CZZX c1;
	long i;
	for (i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i);
		tmp.r %= newQi;
		tmp.i %= newQi;
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		tmp.r %= newQi;
		tmp.i %= newQi;
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

vector<Cipher> Scheme::fft(vector<Cipher>& ciphers, vector<Ksi>& ksis) {
	long csize = ciphers.size();
	if(csize == 1) {
		return ciphers;
	}

	vector<Cipher> res;

	long logcsize = log2(csize);

	vector<Cipher> sub1;
	vector<Cipher> sub2;

	long i;
	for (i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fft(sub1, ksis);
	vector<Cipher> y2 = fft(sub2, ksis);

	for (i = 0; i < csize/2; ++i) {
		Cipher mul1 = mulByConstant(y1[i], params.p);
		CZZ x = ksis[logcsize].pows[i];
		Cipher mul2 = mulByConstant(y2[i], x);
		Cipher sum = add(mul1, mul2);
		Cipher ms = modSwitch(sum, sum.level + 1);
		CZZ d = decrypt(ms);
		res.push_back(ms);
	}

	for (i = 0; i < csize/2; ++i) {
		Cipher mul1 = mulByConstant(y1[i], params.p);
		CZZ x = ksis[logcsize].pows[i];
		Cipher mul2 = mulByConstant(y2[i], x);
		Cipher diff = sub(mul1, mul2);
		Cipher ms = modSwitch(diff, diff.level + 1);
		res.push_back(ms);
	}
	return res;
}

vector<CZZ> Scheme::fft(vector<CZZ>& vals, vector<Ksi>& ksis) {
	long csize = vals.size();
	if(csize == 1) {
		return vals;
	}

	vector<CZZ> res;

	long logcsize = log2(csize);

	vector<CZZ> sub1;
	vector<CZZ> sub2;

	long i;
	for (i = 0; i < csize; i = i+2) {
		sub1.push_back(vals[i]);
		sub2.push_back(vals[i+1]);
	}

	vector<CZZ> y1 = fft(sub1, ksis);
	vector<CZZ> y2 = fft(sub2, ksis);

	for (i = 0; i < csize/2; ++i) {
		CZZ mul1 = y1[i] * params.p;
		CZZ x = ksis[logcsize].pows[i];

		CZZ mul2 = y2[i] * x;
		CZZ sum = mul1 + mul2;
		CZZ ms = sum / params.p;
		res.push_back(ms);
	}

	for (i = 0; i < csize/2; ++i) {
		CZZ mul1 = y1[i] * params.p;
		CZZ x = ksis[logcsize].pows[i];

		CZZ mul2 = y2[i] * x;
		CZZ diff = mul1 - mul2;
		CZZ ms = diff / params.p;
		res.push_back(ms);
	}
	return res;

}

ZZ Scheme:: getQi(long& level) {
	return params.qi[params.L - level];
}

long Scheme::getLogQi(long& level) {
	return params.logq - params.logp * (level-1);
}

long Scheme::getLogTQi(long& level) {
	return params.logPq - params.logp * (level-1);
}
