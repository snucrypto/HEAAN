#include "Scheme.h"

#include <NTL/ZZ.h>
#include <cmath>
#include <iostream>

#include "../czz/CZZ.h"
#include "../czz/CZZX.h"
#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"

using namespace std;
using namespace NTL;

Cipher Scheme::encrypt(CZZ& m, ZZ& nu) {
	ZZ q = params.qi[params.L - 1];
	CZZ tmp;

	CZZX v, e, c0, c1;

	NumUtils::sampleZO(v, params.n, params.rho);
	Ring2Utils::mult(c0, v, publicKey.b, params.logq, params.n);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::add(c0, e, c0, params.logq, params.n);

	Ring2Utils::mult(c1, v, publicKey.a, params.logq, params.n);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::add(c1, e, c1, params.logq, params.n);

	tmp = coeff(c0, 0) + m;
	tmp %= q;
	SetCoeff(c0, 0, tmp);
	c0.normalize();
	Cipher cipher(c0, c1, 1, params.Bclean, nu);
	return cipher;
}

CZZ Scheme::decrypt(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	ZZ qi = getQi(cipher.level);

	CZZX poly;
	Ring2Utils::mult(poly, cipher.c1, secretKey.s, logQi, params.n);
	Ring2Utils::add(poly, poly, cipher.c0, logQi, params.n);

	CZZ m;
	GetCoeff(m, poly, 0);

	while(2 * m.r > qi) m.r -= qi;
	while(2 * m.r < -qi) m.r += qi;

	while(2 * m.i > qi) m.i -= qi;
	while(2 * m.i < -qi) m.i += qi;

	return m;
}

void Scheme::decrypt(vector<CZZ>& res, vector<Cipher>& ciphers) {
	res.clear();
	for (long i = 0; i < ciphers.size(); ++i) {
		CZZ d = decrypt(ciphers[i]);
		res.push_back(d);
	}
}

CZZX Scheme::encode(long& logSlots, vector<CZZ>& mvec) {
	CZZX res;
	vector<CZZ> fft = NumUtils::fft(mvec, params.cksi, false);

	long i, idx = 0;
	long slots = 1 << logSlots;
	long gap = (params.n >> logSlots);

	for (i = 0; i < slots; ++i) {
		SetCoeff(res, idx, fft[i]);
		idx += gap;
	}

	return res;
}

Cipher Scheme::encrypt(long& logSlots, vector<CZZ>& mvec, ZZ& nu) {
	CZZ tmp;

	CZZX c0, c1;
	CZZX v, e;

	NumUtils::sampleZO(v, params.n, params.rho);
	Ring2Utils::mult(c0, v, publicKey.b, params.logq, params.n);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::add(c0, e, c0, params.logq, params.n);

	Ring2Utils::mult(c1, v, publicKey.a, params.logq, params.n);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::add(c1, e, c1, params.logq, params.n);

	CZZX f = encode(logSlots, mvec);

	Ring2Utils::add(c0, f, c0, params.logq, params.n);

	Cipher cipher(c0, c1, 1, params.Bclean, nu);
	return cipher;
}

vector<CZZ> Scheme::decrypt(long& logSlots, Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	ZZ qi = getQi(cipher.level);

	vector<CZZ> fft;
	CZZX poly;

	Ring2Utils::mult(poly, cipher.c1, secretKey.s, logQi, params.n);
	Ring2Utils::add(poly, poly, cipher.c0, logQi, params.n);

	long i, idx = 0;
	long slots = (1 << logSlots);
	long gap = (params.n >> logSlots);
	for (i = 0; i < slots; ++i) {
		fft.push_back(coeff(poly, idx));
		idx += gap;
	}

	vector<CZZ> res = NumUtils::fft(fft, params.cksi, true);
	return res;
}

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CZZX c0, c1;

	Ring2Utils::add(c0, cipher1.c0, cipher2.c0, logQi, params.n);
	Ring2Utils::add(c1, cipher1.c1, cipher2.c1, logQi, params.n);

	ZZ eBnd = cipher1.eBnd + cipher2.eBnd;
	ZZ mBnd = cipher1.mBnd + cipher2.mBnd;

	Cipher res(c0, c1, cipher1.level, eBnd, mBnd);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	Ring2Utils::add(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.n);
	Ring2Utils::add(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.n);

	cipher1.eBnd += cipher2.eBnd;
	cipher1.mBnd += cipher2.mBnd;
}

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	CZZX c0 , c1;

	Ring2Utils::sub(c0, cipher1.c0, cipher2.c0, logQi, params.n);
	Ring2Utils::sub(c1, cipher1.c1, cipher2.c1, logQi, params.n);

	ZZ B = cipher1.eBnd + cipher2.eBnd;
	ZZ nu = cipher1.mBnd + cipher2.mBnd;

	Cipher res(c0, c1, cipher1.level, B, nu);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	Ring2Utils::sub(cipher1.c0, cipher1.c0, cipher2.c0, logQi, params.n);
	Ring2Utils::sub(cipher1.c1, cipher1.c1, cipher2.c1, logQi, params.n);

	cipher1.eBnd += cipher2.eBnd;
	cipher1.mBnd += cipher2.mBnd;
}

Cipher Scheme::mult(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	long logTQi = getLogTQi(cipher1.level);

	CZZX cc00, cc01, cc10, cc11;
	CZZX mulC0, mulC1;

	Ring2Utils::mult(cc00, cipher1.c0, cipher2.c0, logQi, params.n);
	Ring2Utils::mult(cc01, cipher1.c0, cipher2.c1, logQi, params.n);
	Ring2Utils::mult(cc10, cipher1.c1, cipher2.c0, logQi, params.n);
	Ring2Utils::mult(cc11, cipher1.c1, cipher2.c1, logQi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, logTQi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, logTQi, params.n);

	Ring2Utils::rightShift(mulC1, mulC1, params.logP, logQi, params.n);
	Ring2Utils::rightShift(mulC0, mulC0, params.logP, logQi, params.n);

	Ring2Utils::add(mulC1, mulC1, cc10, logQi, params.n);
	Ring2Utils::add(mulC1, mulC1, cc01, logQi, params.n);
	Ring2Utils::add(mulC0, mulC0, cc00, logQi, params.n);

	ZZ eBnd = cipher1.eBnd * (cipher2.mBnd + cipher2.eBnd) + cipher1.mBnd * cipher2.eBnd;
	ZZ mBnd = cipher1.mBnd * cipher2.mBnd;

	Cipher cipher(mulC0, mulC1, cipher1.level, eBnd, mBnd);
	return cipher;
}

Cipher Scheme::square(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	long logTQi = getLogTQi(cipher.level);

	CZZX cc00, cc10, cc11;
	CZZX mulC0, mulC1;

	Ring2Utils::mult(cc00, cipher.c0, cipher.c0, logQi, params.n);
	Ring2Utils::mult(cc10, cipher.c1, cipher.c0, logQi, params.n);
	Ring2Utils::mult(cc11, cipher.c1, cipher.c1, logQi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, logTQi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, logTQi, params.n);

	Ring2Utils::rightShift(mulC1, mulC1, params.logP, logQi, params.n);
	Ring2Utils::rightShift(mulC0, mulC0, params.logP, logQi, params.n);

	Ring2Utils::add(mulC1, mulC1, cc10, logQi, params.n);
	Ring2Utils::add(mulC1, mulC1, cc10, logQi, params.n);
	Ring2Utils::add(mulC0, mulC0, cc00, logQi, params.n);

	ZZ eBnd = 2 * cipher.eBnd * cipher.mBnd + cipher.eBnd * cipher.eBnd;
	ZZ mBnd = cipher.mBnd * cipher.mBnd;

	Cipher c(mulC0, mulC1, cipher.level, eBnd, mBnd);
	return c;
}

Cipher Scheme::multAndModSwitch(Cipher& cipher1, Cipher& cipher2) {
	Cipher c = mult(cipher1, cipher2);
	Cipher cms = modSwitch(c, c.level + 1);
	return cms;
}

Cipher Scheme::squareAndModSwitch(Cipher& cipher) {
	Cipher c = square(cipher);
	Cipher cms = modSwitch(c, c.level + 1);
	return cms;
}

void Scheme::multAndEqual(Cipher& cipher1, Cipher& cipher2) {
	long logQi = getLogQi(cipher1.level);
	long logTQi = getLogTQi(cipher1.level);

	CZZX cc00, cc01, cc10, cc11;
	CZZX mulC0, mulC1;

	Ring2Utils::mult(cc00, cipher1.c0, cipher2.c0, logQi, params.n);
	Ring2Utils::mult(cc01, cipher1.c0, cipher2.c1, logQi, params.n);
	Ring2Utils::mult(cc10, cipher1.c1, cipher2.c0, logQi, params.n);
	Ring2Utils::mult(cc11, cipher1.c1, cipher2.c1, logQi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, logTQi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, logTQi, params.n);

	Ring2Utils::rightShift(mulC1, mulC1, params.logP, logQi, params.n);
	Ring2Utils::rightShift(mulC0, mulC0, params.logP, logQi, params.n);

	Ring2Utils::add(mulC1, mulC1, cc10, logQi, params.n);
	Ring2Utils::add(mulC1, mulC1, cc01, logQi, params.n);
	Ring2Utils::add(mulC0, mulC0, cc00, logQi, params.n);

	cipher1.c0 = mulC0;
	cipher1.c1 = mulC1;

	cipher1.eBnd = cipher1.eBnd * (cipher2.mBnd + cipher2.eBnd) + cipher1.mBnd * cipher2.eBnd;
	cipher1.mBnd = cipher1.mBnd * cipher2.mBnd;
}

void Scheme::squareAndEqual(Cipher& cipher) {
	long logQi = getLogQi(cipher.level);
	long logTQi = getLogTQi(cipher.level);

	CZZX cc00, cc10, cc11;
	CZZX mulC0, mulC1;

	Ring2Utils::mult(cc00, cipher.c0, cipher.c0, logQi, params.n);
	Ring2Utils::mult(cc10, cipher.c0, cipher.c1, logQi, params.n);
	Ring2Utils::mult(cc11, cipher.c1, cipher.c1, logQi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, logTQi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, logTQi, params.n);

	Ring2Utils::rightShift(mulC1, mulC1, params.logP, logQi, params.n);
	Ring2Utils::rightShift(mulC0, mulC0, params.logP, logQi, params.n);

	Ring2Utils::add(mulC1, mulC1, cc10, logQi, params.n);
	Ring2Utils::add(mulC1, mulC1, cc10, logQi, params.n);
	Ring2Utils::add(mulC0, mulC0, cc00, logQi, params.n);

	cipher.c0 = mulC0;
	cipher.c1 = mulC1;

	cipher.eBnd *= 2 * cipher.mBnd + cipher.eBnd;
	cipher.mBnd *= cipher.mBnd;
}

Cipher Scheme::addConstant(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getQi(cipher.level);
	CZZ tmp;
	CZZX c0 = cipher.c0;
	CZZX c1 = cipher.c1;
	tmp = coeff(cipher.c0,0) + cnst;
	tmp %= qi;
	SetCoeff(c0, 0, tmp);
	c0.normalize();

	ZZ eBnd = cipher.eBnd + 1;
	ZZ mBnd = cipher.mBnd + cnst;

	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	return newCipher;
}

Cipher Scheme::addConstant(Cipher& cipher, CZZ& cnst) {
	ZZ qi = getQi(cipher.level);
	CZZ tmp;
	CZZX c0 = cipher.c0;
	CZZX c1 = cipher.c1;
	tmp = coeff(cipher.c0,0) + cnst;
	tmp %= qi;
	SetCoeff(c0, 0, tmp);
	c0.normalize();

	ZZ norm = cnst.norm();

	ZZ eBnd = cipher.eBnd + 1;
	ZZ mBnd = cipher.mBnd + norm;

	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	return newCipher;
}

Cipher Scheme::multByConstant(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getQi(cipher.level);
	CZZ tmp;
	CZZX c0, c1;

	for (long i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		tmp %= qi;
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		tmp %= qi;
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();

	ZZ eBnd = cipher.eBnd * cnst;
	ZZ mBnd = cipher.mBnd * cnst;

	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	return newCipher;
}

Cipher Scheme::multByConstant(Cipher& cipher, CZZ& cnst) {
	ZZ qi = getQi(cipher.level);
	CZZ tmp;
	CZZX c0, c1;

	for (long i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		tmp %= qi;
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		tmp %= qi;
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();

	ZZ norm = cnst.norm();

	ZZ eBnd = cipher.eBnd * norm;
	ZZ mBnd = cipher.mBnd * norm;

	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	return newCipher;
}

void Scheme::addConstantAndEqual(Cipher& cipher, ZZ& cnst) {
	CZZ tmp;
	ZZ qi = getQi(cipher.level);
	tmp = coeff(cipher.c0, 0) * cnst;
	tmp %= qi;
	SetCoeff(cipher.c0, 0, tmp);
	cipher.c0.normalize();
}

void Scheme::multByConstantAndEqual(Cipher& cipher, ZZ& cnst) {
	CZZ tmp;
	ZZ qi = getQi(cipher.level);

	for (long i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		tmp %= qi;
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		tmp %= qi;
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();

	cipher.eBnd *= cnst;
	cipher.mBnd *= cnst;
}

void Scheme::multByConstantAndEqual(Cipher& cipher, CZZ& cnst) {
	CZZ tmp;
	ZZ qi = getQi(cipher.level);

	for (long i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i) * cnst;
		tmp %= qi;
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i) * cnst;
		tmp %= qi;
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();

	ZZ norm = cnst.norm();

	cipher.eBnd *= norm;
	cipher.mBnd *= norm;

}

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logDF = params.logp * (newLevel-cipher.level);

	CZZX c0;
	CZZX c1;
	for (long i = 0; i < params.n; ++i) {
		CZZ shift0 = coeff(cipher.c0, i) >> logDF;
		SetCoeff(c0, i, shift0);
		CZZ shift1 = coeff(cipher.c1, i) >> logDF;
		SetCoeff(c1, i, shift1);
	}
	c0.normalize();
	c1.normalize();

	ZZ eBnd = (cipher.eBnd >> params.logp) + params.Bscale;
	ZZ mBnd = cipher.mBnd >> params.logp;

	Cipher newCipher(c0, c1, newLevel, eBnd, mBnd);
	return newCipher;
}

Cipher Scheme::modSwitch(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modSwitch(cipher, newLevel);
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long newLevel) {
	long logDF = params.logp * (newLevel-cipher.level);

	for (long i = 0; i < params.n; ++i) {
		CZZ shift0 = coeff(cipher.c0, i) >> logDF;
		SetCoeff(cipher.c0, i, shift0);
		CZZ shift1 = coeff(cipher.c1, i) >> logDF;
		SetCoeff(cipher.c1, i, shift1);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;

	cipher.eBnd >>= params.logp;
	cipher.eBnd += params.Bscale;
	cipher.mBnd >>= params.logp;
}

void Scheme::modSwitchAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modSwitchAndEqual(cipher, newLevel);
}

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	ZZ newQi = getQi(newLevel);
	CZZ tmp;
	CZZX c0, c1;

	for (long i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i);
		tmp %= newQi;
		SetCoeff(c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		tmp %= newQi;
		SetCoeff(c1, i, tmp);
	}
	c0.normalize();
	c1.normalize();

	Cipher newCipher(c0, c1, newLevel, cipher.eBnd, cipher.mBnd);
	return newCipher;
}

void Scheme::modEmbedAndEqual(Cipher& cipher, long newLevel) {
	CZZ tmp;
	ZZ newQi = getQi(newLevel);

	for (long i = 0; i < params.n; ++i) {
		tmp = coeff(cipher.c0,i);
		tmp %= newQi;
		SetCoeff(cipher.c0, i, tmp);
		tmp = coeff(cipher.c1,i);
		tmp %= newQi;
		SetCoeff(cipher.c1, i, tmp);
	}
	cipher.c0.normalize();
	cipher.c1.normalize();
	cipher.level = newLevel;
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
