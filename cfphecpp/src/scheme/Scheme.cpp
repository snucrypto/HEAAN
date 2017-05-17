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

ZZ Scheme::getqi(long& level) {
	return params.qi[params.L - level];
}

ZZ Scheme::getPqi(long& level) {
	return params.Pqi[params.L - level];
}

long Scheme::getLogqi(long& level) {
	return params.logq - params.logp * (level-1);
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

void Scheme::rlweInstance(ZZX& c0, ZZX& c1, ZZ& qi) {
	ZZX v, e;
	NumUtils::sampleZO(v, params.N);
	Ring2Utils::mult(c0, v, publicKey.b, qi, params.N);
	NumUtils::sampleGauss(e, params.N, params.sigma);
	Ring2Utils::add(c0, e, c0, qi, params.N);

	Ring2Utils::mult(c1, v, publicKey.a, qi, params.N);
	NumUtils::sampleGauss(e, params.N, params.sigma);
	Ring2Utils::add(c1, e, c1, qi, params.N);
}

void Scheme::rlweInstance(ZZX& c0, ZZX& c1) {
	rlweInstance(c0, c1, params.q);
}

vector<CZZ> Scheme::doubleConjugate(vector<CZZ>& vals) {
	vector<CZZ> res;
	long vsize = vals.size();
	for (long i = 0; i < vsize; ++i) {
		res.push_back(vals[i]);
	}
	for (long i = 0; i < vsize; ++i) {
		res.push_back(vals[vsize - i - 1].conjugate());
	}
	return res;
}

vector<CZZ> Scheme::doubleConjugate(CZZ& val) {
	vector<CZZ> res;
	res.push_back(val);
	res.push_back(val.conjugate());
	return res;
}

Message Scheme::encode(vector<CZZ>& vals) {
	ZZX poly;
	poly.SetLength(params.N);
	long idx = 0;
	long slots = vals.size();
	long logSlots = log2(slots);
	long gap = (params.N >> logSlots);
	vector<CZZ> fftInv = NumUtils::fftSpecialInv(vals, params.ksiPows);
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
	Ring2Utils::add(c0, msg.poly, c0, qi, params.N);
//	Cipher cipher(c0, c1, 1, params.Bclean, msg.nu);
	Cipher cipher(c0, c1, level, msg.logSlots);
	return cipher;
}

Cipher Scheme::encrypt(Message& msg) {
	long level = 1;
	Cipher cipher = encrypt(msg, level);
	return cipher;
}

Cipher Scheme::fullEncrypt(vector<CZZ>& vals, long& level) {
	vector<CZZ> conj = doubleConjugate(vals);
	Message msg = encode(conj);
	return encrypt(msg, level);
}

Cipher Scheme::fullEncrypt(vector<CZZ>& vals) {
	vector<CZZ> conj = doubleConjugate(vals);
	Message msg = encode(conj);
	return encrypt(msg);
}

Cipher Scheme::fullEncrypt(CZZ& val, long& level) {
	vector<CZZ> conj = doubleConjugate(val);
	Message msg = encode(conj);
	return encrypt(msg, level);
}

Cipher Scheme::fullEncrypt(CZZ& val) {
	vector<CZZ> conj = doubleConjugate(val);
	Message msg = encode(conj);
	return encrypt(msg);
}

Message Scheme::decrypt(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZX poly;
	poly.SetLength(params.N);
	Ring2Utils::mult(poly, cipher.c1, secretKey.s, qi, params.N);
	Ring2Utils::add(poly, poly, cipher.c0, qi, params.N);
	Message msg(poly, cipher.logSlots, cipher.level);
	return msg;
}

vector<CZZ> Scheme::decode(Message& msg) {
	vector<CZZ> fftinv;
	ZZ qi = getqi(msg.level);

	long idx = 0;
	long slots = (1 << msg.logSlots);
	long gap = (params.N >> msg.logSlots);
	for (long i = 0; i < slots; ++i) {
		CZZ c(msg.poly.rep[idx], ZZ(0));
		trueValue(c, qi);
		fftinv.push_back(c);
		idx += gap;
	}
	vector<CZZ> fft = NumUtils::fftSpecial(fftinv, params.ksiPows);
	return fft;
}

vector<CZZ> Scheme::deConjugate(vector<CZZ>& vals) {
	vector<CZZ> res;
	long size = vals.size();
	for (long i = 0; i < size / 2; ++i) {
		res.push_back(vals[i]);
	}
	return res;
}

vector<CZZ> Scheme::fullDecrypt(Cipher& cipher) {
	Message msg = decrypt(cipher);
	vector<CZZ> conj = decode(msg);
	return deConjugate(conj);
}

//-----------------------------

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX c0, c1;

	Ring2Utils::add(c0, cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::add(c1, cipher1.c1, cipher2.c1, qi, params.N);

//	ZZ eBnd = cipher1.eBnd + cipher2.eBnd;
//	ZZ mBnd = cipher1.mBnd + cipher2.mBnd;

//	Cipher res(c0, c1, cipher1.level, eBnd, mBnd);
	Cipher res(c0, c1, cipher1.level, cipher1.logSlots);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::addAndEqual(cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.c1, cipher2.c1, qi, params.N);

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

	vector<CZZ> cconj = doubleConjugate(cnst);
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

	Ring2Utils::sub(c0, cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::sub(c1, cipher1.c1, cipher2.c1, qi, params.N);

//	ZZ B = cipher1.eBnd + cipher2.eBnd;
//	ZZ nu = cipher1.mBnd + cipher2.mBnd;

//	Cipher res(c0, c1, cipher1.level, B, nu);
	Cipher res(c0, c1, cipher1.level, cipher1.logSlots);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::subAndEqual(cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.c1, cipher2.c1, qi, params.N);

//	cipher1.eBnd += cipher2.eBnd;
//	cipher1.mBnd += cipher2.mBnd;
}

Cipher Scheme::mult(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX cc00, cc11, mulC1, mulC0;

	ZZX add1, add2, mult;
	Ring2Utils::add(add1, cipher1.c0, cipher1.c1, qi, params.N);
	Ring2Utils::add(add2, cipher2.c0, cipher2.c1, qi, params.N);
	Ring2Utils::mult(mult, add1, add2, qi, params.N);

	Ring2Utils::mult(cc00, cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::mult(cc11, cipher1.c1, cipher2.c1, qi, params.N);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(mulC1, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(mulC0, params.logP, params.N);

	Ring2Utils::addAndEqual(mulC1, mult, qi, params.N);
	Ring2Utils::subAndEqual(mulC1, cc00, qi, params.N);
	Ring2Utils::subAndEqual(mulC1, cc11, qi, params.N);
	Ring2Utils::addAndEqual(mulC0, cc00, qi, params.N);

//	ZZ eBnd = cipher1.eBnd * (cipher2.mBnd + cipher2.eBnd) + cipher1.mBnd * cipher2.eBnd;
//	ZZ mBnd = cipher1.mBnd * cipher2.mBnd;

//	Cipher cipher(mulC0, mulC1, cipher1.level, eBnd, mBnd);
	Cipher cipher(mulC0, mulC1, cipher1.level, cipher1.logSlots);
	return cipher;
}

void Scheme::multAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX cc00, cc11;

	ZZX add1, add2, mult;
	Ring2Utils::mult(cc00, cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::add(add1, cipher1.c0, cipher1.c1, qi, params.N);
	Ring2Utils::add(add2, cipher2.c0, cipher2.c1, qi, params.N);
	Ring2Utils::mult(mult, add1, add2, qi, params.N);
	Ring2Utils::mult(cc11, cipher1.c1, cipher2.c1, qi, params.N);

	Ring2Utils::mult(cipher1.c1, cc11, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(cipher1.c0, cc11, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(cipher1.c1, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(cipher1.c0, params.logP, params.N);

	Ring2Utils::addAndEqual(cipher1.c1, mult, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.c1, cc00, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.c1, cc11, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.c0, cc00, qi, params.N);

//	cipher1.eBnd = cipher1.eBnd * (cipher2.mBnd + cipher2.eBnd) + cipher1.mBnd * cipher2.eBnd;
//	cipher1.mBnd = cipher1.mBnd * cipher2.mBnd;
}

Cipher Scheme::square(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX cc00, cc10, cc11, mulC0, mulC1;

	Ring2Utils::square(cc00, cipher.c0, qi, params.N);
	Ring2Utils::mult(cc10, cipher.c1, cipher.c0, qi, params.N);
	Ring2Utils::addAndEqual(cc10, cc10, qi, params.N);
	Ring2Utils::square(cc11, cipher.c1, qi, params.N);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(mulC1, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(mulC0, params.logP, params.N);

	Ring2Utils::addAndEqual(mulC1, cc10, qi, params.N);
	Ring2Utils::addAndEqual(mulC0, cc00, qi, params.N);

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

	Ring2Utils::square(cc00, cipher.c0, qi, params.N);
	Ring2Utils::mult(cc10, cipher.c0, cipher.c1, qi, params.N);
	Ring2Utils::addAndEqual(cc10, cc10, qi, params.N);
	Ring2Utils::square(cc11, cipher.c1, qi, params.N);

	Ring2Utils::mult(mulC1, cc11, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(mulC0, cc11, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(mulC1, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(mulC0, params.logP, params.N);

	Ring2Utils::addAndEqual(mulC1, cc10, qi, params.N);
	Ring2Utils::addAndEqual(mulC0, cc00, qi, params.N);

	cipher.c0 = mulC0;
	cipher.c1 = mulC1;

//	cipher.eBnd *= 2 * cipher.mBnd + cipher.eBnd;
//	cipher.mBnd *= cipher.mBnd;
}

Cipher Scheme::multByConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0, c1;
	Ring2Utils::multByConst(c0, cipher.c0, cnst, qi, params.N);
	Ring2Utils::multByConst(c1, cipher.c1, cnst, qi, params.N);

//	ZZ eBnd = cipher.eBnd * cnst;
//	ZZ mBnd = cipher.mBnd * cnst;

//	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	Cipher newCipher(c0, c1, cipher.level, cipher.logSlots);
	return newCipher;
}

void Scheme::multByConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::multByConstAndEqual(cipher.c0, cnst, qi, params.N);
	Ring2Utils::multByConstAndEqual(cipher.c1, cnst, qi, params.N);

//	cipher.eBnd *= cnst;
//	cipher.mBnd *= cnst;
}

Cipher Scheme::multByMonomial(Cipher& cipher, const long& degree) {
	ZZX c0 , c1;

	Ring2Utils::multByMonomial(c0, cipher.c0, degree, params.N);
	Ring2Utils::multByMonomial(c1, cipher.c1, degree, params.N);

//	Cipher res(c0, c1, cipher.level, cipher.eBnd, cipher.mBnd);
	Cipher res(c0, c1, cipher.level, cipher.logSlots);
	return res;
}

void Scheme::multByMonomialAndEqual(Cipher& cipher, const long& degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.c0, degree, params.N);
	Ring2Utils::multByMonomialAndEqual(cipher.c1, degree, params.N);
}

Cipher Scheme::leftShift(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	ZZX c0, c1;

	Ring2Utils::leftShift(c0, cipher.c0, bits, logqi, params.N);
	Ring2Utils::leftShift(c1, cipher.c1, bits, logqi, params.N);

//	ZZ eBnd = cipher.eBnd << bits;
//	ZZ mBnd = cipher.mBnd << bits;

//	Cipher newCipher(c0, c1, cipher.level, eBnd, mBnd);
	Cipher newCipher(c0, c1, cipher.level, cipher.logSlots);
	return newCipher;
}

void Scheme::leftShiftAndEqual(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	Ring2Utils::leftShiftAndEqual(cipher.c0, bits, logqi, params.N);
	Ring2Utils::leftShiftAndEqual(cipher.c1, bits, logqi, params.N);

//	cipher.eBnd <<= bits;
//	cipher.mBnd <<= bits;
}

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	ZZX c0, c1;

	Ring2Utils::rightShift(c0, cipher.c0, logdf, params.N);
	Ring2Utils::rightShift(c1, cipher.c1, logdf, params.N);

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
	Ring2Utils::rightShiftAndEqual(cipher.c0, logdf, params.N);
	Ring2Utils::rightShiftAndEqual(cipher.c1, logdf, params.N);
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
	Ring2Utils::truncate(c0, cipher.c0, newLogqi, params.N);
	Ring2Utils::truncate(c1, cipher.c1, newLogqi, params.N);
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
	Ring2Utils::truncateAndEqual(cipher.c0, newLogqi, params.N);
	Ring2Utils::truncateAndEqual(cipher.c0, newLogqi, params.N);
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
