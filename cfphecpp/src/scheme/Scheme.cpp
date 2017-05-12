#include "Scheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"
#include "Params.h"

using namespace std;
using namespace NTL;

Scheme::Scheme(Params& params, SecKey& secretKey, PubKey& publicKey): params(params), secretKey(secretKey), publicKey(publicKey) {};

void Scheme::rlweInstance(ZZX& c0, ZZX& c1, ZZ& qi) {
	ZZX v, e;
	NumUtils::sampleZO(v, params.n);
	Ring2Utils::mult(c0, v, publicKey.b, qi, params.n);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::add(c0, e, c0, qi, params.n);

	Ring2Utils::mult(c1, v, publicKey.a, qi, params.n);
	NumUtils::sampleGauss(e, params.n, params.sigma);
	Ring2Utils::add(c1, e, c1, qi, params.n);
}

void Scheme::rlweInstance(ZZX& c0, ZZX& c1) {
	rlweInstance(c0, c1, params.q);
}

void Scheme::trueValue(CZZ& m, ZZ& qi) {
	while(2 * m.r > qi) m.r -= qi;
	while(2 * m.r < -qi) m.r += qi;

	while(2 * m.i > qi) m.i -= qi;
	while(2 * m.i < -qi) m.i += qi;
}

void Scheme::trueValue(ZZ& m, ZZ& qi) {
	while(2 * m > qi) m -= qi;
	while(2 * m < -qi) m += qi;
}

vector<CZZ> Scheme::conj(vector<CZZ>& vals) {
	vector<CZZ> res;
	long vsize = vals.size();
	res.push_back(CZZ(0,0));
	for (long i = 0; i < vsize; ++i) {
		res.push_back(vals[i]);
	}
	res.push_back(CZZ(0,0));
	for (long i = 0; i < vsize; ++i) {
		res.push_back(vals[vsize - i - 1].conjugate());
	}
	return res;
}

vector<CZZ> Scheme::conj(CZZ& val) {
	vector<CZZ> res;
	res.push_back(CZZ(0,0));
	res.push_back(val);
	res.push_back(CZZ(0,0));
	res.push_back(val.conjugate());
	return res;
}

Message Scheme::encode(vector<CZZ>& vals) {
	ZZX poly;
	poly.SetLength(params.n);
	long idx = 0;
	long slots = vals.size();
	long logSlots = log2(slots);
	long gap = (params.n >> logSlots);
	vector<CZZ> fftInv = NumUtils::fftInv(vals, params.cksi);
	for (long i = 0; i < slots; ++i) {
		poly.rep[idx] = fftInv[i].r;
		idx += gap;
	}
	Message res(poly, logSlots);
	return res;
}

Cipher Scheme::encrypt(Message& msg, long& level) {
	ZZX c0, c1;
	ZZ qi = getqi(level);
	rlweInstance(c0, c1, qi);
	Ring2Utils::add(c0, msg.poly, c0, qi, params.n);
//	Cipher cipher(c0, c1, 1, params.Bclean, msg.nu);
	Cipher cipher(c0, c1, level, msg.logSlots);
	return cipher;
}

Cipher Scheme::encrypt(Message& msg) {
	long level = 1;
	Cipher cipher = encrypt(msg, level);
	return cipher;
}

Message Scheme::decrypt(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZX poly;
	poly.SetLength(params.n);
	Ring2Utils::mult(poly, cipher.c1, secretKey.s, qi, params.n);
	Ring2Utils::add(poly, poly, cipher.c0, qi, params.n);
	Message msg(poly, cipher.logSlots, cipher.level);
	return msg;
}

vector<CZZ> Scheme::decode(Message& msg) {
	vector<CZZ> fftinv;
	ZZ qi = getqi(msg.level);

	long idx = 0;
	long slots = (1 << msg.logSlots);
	long gap = (params.n >> msg.logSlots);
	for (long i = 0; i < slots; ++i) {
		CZZ c(msg.poly.rep[idx], ZZ(0));
		trueValue(c, qi);
		fftinv.push_back(c);
		idx += gap;
	}
	vector<CZZ> fft = NumUtils::fft(fftinv, params.cksi);
	return fft;
}

vector<CZZ> Scheme::dconj(vector<CZZ>& vals) {
	vector<CZZ> res;
	long size = vals.size();
	for (long i = 1; i < size / 2; ++i) {
		res.push_back(vals[i]);
	}
	return res;
}
//-----------------------------

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX c0, c1;

	Ring2Utils::add(c0, cipher1.c0, cipher2.c0, qi, params.n);
	Ring2Utils::add(c1, cipher1.c1, cipher2.c1, qi, params.n);

//	ZZ eBnd = cipher1.eBnd + cipher2.eBnd;
//	ZZ mBnd = cipher1.mBnd + cipher2.mBnd;

//	Cipher res(c0, c1, cipher1.level, eBnd, mBnd);
	Cipher res(c0, c1, cipher1.level, cipher1.logSlots);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::addAndEqual(cipher1.c0, cipher2.c0, qi, params.n);
	Ring2Utils::addAndEqual(cipher1.c1, cipher2.c1, qi, params.n);

//	cipher1.eBnd += cipher2.eBnd;
//	cipher1.mBnd += cipher2.mBnd;
}

Cipher Scheme::addConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;

	AddMod(c0.rep[0], cipher.c0.rep[0], cnst, qi);
	c0.normalize();

//	ZZ eBnd = cipher.eBnd + 1;
//	ZZ mBnd = cipher.mBnd + cnst;

//	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	Cipher newCipher(c0, c1, cipher.level, cipher.logSlots);
	return newCipher;
}

Cipher Scheme::addConst(Cipher& cipher, CZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;

	vector<CZZ> cconj = conj(cnst);
	Message cmsg = encode(cconj);
	Cipher cnstcipher = encrypt(cmsg, cipher.level);
	Cipher res = add(cipher, cnstcipher);

//	ZZ norm = cnst.norm();

//	ZZ eBnd = cipher.eBnd + 1;
//	ZZ mBnd = cipher.mBnd + norm;

	return res;
}

void Scheme::addConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	AddMod(cipher.c0.rep[0], cipher.c0.rep[0], cnst, qi);
	cipher.c0.normalize();
}

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX c0 , c1;

	Ring2Utils::sub(c0, cipher1.c0, cipher2.c0, qi, params.n);
	Ring2Utils::sub(c1, cipher1.c1, cipher2.c1, qi, params.n);

//	ZZ B = cipher1.eBnd + cipher2.eBnd;
//	ZZ nu = cipher1.mBnd + cipher2.mBnd;

//	Cipher res(c0, c1, cipher1.level, B, nu);
	Cipher res(c0, c1, cipher1.level, cipher1.logSlots);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::subAndEqual(cipher1.c0, cipher2.c0, qi, params.n);
	Ring2Utils::subAndEqual(cipher1.c1, cipher2.c1, qi, params.n);

//	cipher1.eBnd += cipher2.eBnd;
//	cipher1.mBnd += cipher2.mBnd;
}

Cipher Scheme::mult(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX cc00, cc01, cc10, cc11, mulC1, mulC0;

	Ring2Utils::mult(cc00, cipher1.c0, cipher2.c0, qi, params.n);
	Ring2Utils::mult(cc01, cipher1.c0, cipher2.c1, qi, params.n);
	Ring2Utils::mult(cc10, cipher1.c1, cipher2.c0, qi, params.n);
	Ring2Utils::mult(cc11, cipher1.c1, cipher2.c1, qi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, Pqi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, Pqi, params.n);

	Ring2Utils::rightShiftAndEqual(mulC1, params.logP, params.n);
	Ring2Utils::rightShiftAndEqual(mulC0, params.logP, params.n);

	Ring2Utils::addAndEqual(mulC1, cc10, qi, params.n);
	Ring2Utils::addAndEqual(mulC1, cc01, qi, params.n);
	Ring2Utils::addAndEqual(mulC0, cc00, qi, params.n);

//	ZZ eBnd = cipher1.eBnd * (cipher2.mBnd + cipher2.eBnd) + cipher1.mBnd * cipher2.eBnd;
//	ZZ mBnd = cipher1.mBnd * cipher2.mBnd;

//	Cipher cipher(mulC0, mulC1, cipher1.level, eBnd, mBnd);
	Cipher cipher(mulC0, mulC1, cipher1.level, cipher1.logSlots);
	return cipher;
}

void Scheme::multAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX cc00, cc01, cc10;

	Ring2Utils::mult(cc00, cipher1.c0, cipher2.c0, qi, params.n);
	Ring2Utils::mult(cc01, cipher1.c0, cipher2.c1, qi, params.n);
	Ring2Utils::mult(cc10, cipher1.c1, cipher2.c0, qi, params.n);
	Ring2Utils::mult(cipher1.c0, cipher1.c1, cipher2.c1, qi, params.n);

	Ring2Utils::mult(cipher1.c1, cipher1.c0, publicKey.aStar, Pqi, params.n);
	Ring2Utils::multAndEqual(cipher1.c0, publicKey.bStar, Pqi, params.n);

	Ring2Utils::rightShiftAndEqual(cipher1.c1, params.logP, params.n);
	Ring2Utils::rightShiftAndEqual(cipher1.c0, params.logP, params.n);

	Ring2Utils::addAndEqual(cipher1.c1, cc10, qi, params.n);
	Ring2Utils::addAndEqual(cipher1.c1, cc01, qi, params.n);
	Ring2Utils::addAndEqual(cipher1.c0, cc00, qi, params.n);

//	cipher1.eBnd = cipher1.eBnd * (cipher2.mBnd + cipher2.eBnd) + cipher1.mBnd * cipher2.eBnd;
//	cipher1.mBnd = cipher1.mBnd * cipher2.mBnd;
}

Cipher Scheme::square(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX cc00, cc10, cc11, mulC0, mulC1;

	Ring2Utils::square(cc00, cipher.c0, qi, params.n);
	Ring2Utils::mult(cc10, cipher.c1, cipher.c0, qi, params.n);
	Ring2Utils::addAndEqual(cc10, cc10, qi, params.n);
	Ring2Utils::square(cc11, cipher.c1, qi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, Pqi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, Pqi, params.n);

	Ring2Utils::rightShiftAndEqual(mulC1, params.logP, params.n);
	Ring2Utils::rightShiftAndEqual(mulC0, params.logP, params.n);

	Ring2Utils::addAndEqual(mulC1, cc10, qi, params.n);
	Ring2Utils::addAndEqual(mulC0, cc00, qi, params.n);

//	ZZ eBnd = 2 * cipher.eBnd * cipher.mBnd + cipher.eBnd * cipher.eBnd;
//	ZZ mBnd = cipher.mBnd * cipher.mBnd;

//	Cipher c(mulC0, mulC1, cipher.level, eBnd, mBnd);
	Cipher c(mulC0, mulC1, cipher.level, cipher.logSlots);
	return c;
}

void Scheme::squareAndEqual(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX cc00, cc10, cc11, mulC0, mulC1;

	Ring2Utils::square(cc00, cipher.c0, qi, params.n);
	Ring2Utils::mult(cc10, cipher.c0, cipher.c1, qi, params.n);
	Ring2Utils::addAndEqual(cc10, cc10, qi, params.n);
	Ring2Utils::square(cc11, cipher.c1, qi, params.n);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, Pqi, params.n);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, Pqi, params.n);

	Ring2Utils::rightShiftAndEqual(mulC1, params.logP, params.n);
	Ring2Utils::rightShiftAndEqual(mulC0, params.logP, params.n);

	Ring2Utils::addAndEqual(mulC1, cc10, qi, params.n);
	Ring2Utils::addAndEqual(mulC0, cc00, qi, params.n);

	cipher.c0 = mulC0;
	cipher.c1 = mulC1;

//	cipher.eBnd *= 2 * cipher.mBnd + cipher.eBnd;
//	cipher.mBnd *= cipher.mBnd;
}

Cipher Scheme::multByConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0, c1;
	Ring2Utils::multByConst(c0, cipher.c0, cnst, qi, params.n);
	Ring2Utils::multByConst(c1, cipher.c1, cnst, qi, params.n);

//	ZZ eBnd = cipher.eBnd * cnst;
//	ZZ mBnd = cipher.mBnd * cnst;

//	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	Cipher newCipher(c0, c1, cipher.level, cipher.logSlots);
	return newCipher;
}

void Scheme::multByConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::multByConstAndEqual(cipher.c0, cnst, qi, params.n);
	Ring2Utils::multByConstAndEqual(cipher.c1, cnst, qi, params.n);

//	cipher.eBnd *= cnst;
//	cipher.mBnd *= cnst;
}

Cipher Scheme::multByMonomial(Cipher& cipher, const long& degree) {
	ZZX c0 , c1;

	Ring2Utils::multByMonomial(c0, cipher.c0, degree, params.n);
	Ring2Utils::multByMonomial(c1, cipher.c1, degree, params.n);

//	Cipher res(c0, c1, cipher.level, cipher.eBnd, cipher.mBnd);
	Cipher res(c0, c1, cipher.level, cipher.logSlots);
	return res;
}

void Scheme::multByMonomialAndEqual(Cipher& cipher, const long& degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.c0, degree, params.n);
	Ring2Utils::multByMonomialAndEqual(cipher.c1, degree, params.n);
}

Cipher Scheme::leftShift(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	ZZX c0, c1;

	Ring2Utils::leftShift(c0, cipher.c0, bits, logqi, params.n);
	Ring2Utils::leftShift(c1, cipher.c1, bits, logqi, params.n);

//	ZZ eBnd = cipher.eBnd << bits;
//	ZZ mBnd = cipher.mBnd << bits;

//	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	Cipher newCipher(c0, c1, cipher.level, cipher.logSlots);
	return newCipher;
}

void Scheme::leftShiftAndEqual(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	Ring2Utils::leftShiftAndEqual(cipher.c0, bits, logqi, params.n);
	Ring2Utils::leftShiftAndEqual(cipher.c1, bits, logqi, params.n);

//	cipher.eBnd <<= bits;
//	cipher.mBnd <<= bits;
}

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	ZZX c0, c1;

	Ring2Utils::rightShift(c0, cipher.c0, logdf, params.n);
	Ring2Utils::rightShift(c1, cipher.c1, logdf, params.n);

//	ZZ eBnd = (cipher.eBnd >> params.logp) + params.Bscale;
//	ZZ mBnd = cipher.mBnd >> params.logp;

//	Cipher newCipher(c0, c1, newLevel, eBnd, mBnd);
	Cipher newCipher(c0, c1, newLevel, cipher.logSlots);
	return newCipher;
}

Cipher Scheme::modSwitch(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modSwitch(cipher, newLevel);
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	Ring2Utils::rightShiftAndEqual(cipher.c0, logdf, params.n);
	Ring2Utils::rightShiftAndEqual(cipher.c1, logdf, params.n);
	cipher.level = newLevel;

//	cipher.eBnd >>= params.logp;
//	cipher.eBnd += params.Bscale;
//	cipher.mBnd >>= params.logp;
}

void Scheme::modSwitchAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modSwitchAndEqual(cipher, newLevel);
}

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	long newLogqi = getLogqi(newLevel);
	ZZX c0, c1;
	Ring2Utils::truncate(c0, cipher.c0, newLogqi, params.n);
	Ring2Utils::truncate(c1, cipher.c1, newLogqi, params.n);
//	Cipher newCipher(c0, c1, newLevel, cipher.eBnd, cipher.mBnd);
	Cipher newCipher(c0, c1, newLevel, cipher.logSlots);
	return newCipher;
}

Cipher Scheme::modEmbed(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modEmbed(cipher, newLevel);
}

void Scheme::modEmbedAndEqual(Cipher& cipher, long newLevel) {
	long newLogqi = getLogqi(newLevel);
	Ring2Utils::truncateAndEqual(cipher.c0, newLogqi, params.n);
	Ring2Utils::truncateAndEqual(cipher.c0, newLogqi, params.n);
	cipher.level = newLevel;
}

void Scheme::modEmbedAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modEmbedAndEqual(cipher, newLevel);
}

Cipher Scheme::multAndModSwitch(Cipher& cipher1, Cipher& cipher2) {
	Cipher c = mult(cipher1, cipher2);
	modSwitchAndEqual(c);
	return c;
}

void Scheme::multModSwitchAndEqual(Cipher& cipher1, Cipher& cipher2) {
	multAndEqual(cipher1, cipher2);
	modSwitchAndEqual(cipher1);
}

//----------------------------------

ZZ Scheme::getqi(long& level) {
	return params.qi[params.L - level];
}

ZZ Scheme::getPqi(long& level) {
	return params.Pqi[params.L - level];
}

long Scheme::getLogqi(long& level) {
	return params.logq - params.logp * (level-1);
}
