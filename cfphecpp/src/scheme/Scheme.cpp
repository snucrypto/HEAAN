#include "Scheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <cmath>

#include "../czz/CZZ.h"
#include "../utils/NumUtils.h"
#include "../utils/Ring2Utils.h"
#include "../utils/StringUtils.h"
#include "Params.h"

using namespace std;
using namespace NTL;

//-----------------------------------------

ZZ Scheme::getqi(long& level) {
	return params.qi[params.L - level];
}

ZZ Scheme::getPqi(long& level) {
	return params.Pqi[params.L - level];
}

long Scheme::getLogqi(long& level) {
	return params.logq - params.logp * (level-1);
}

//-----------------------------------------

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

//-----------------------------------------

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

//-----------------------------------------

CZZ* Scheme::doubleConjugate(CZZ*& vals, long slots) {
	CZZ* res = new CZZ[slots * 2];
	for (long i = 0; i < slots; ++i) {
		res[i] = vals[i];
		res[2 * slots - i - 1] = vals[i].conjugate();
	}

	return res;
}

CZZ* Scheme::doubleConjugate(CZZ& val) {
	CZZ* res;
	res = new CZZ[2];
	res[0] = val;
	res[1] = val.conjugate();
	return res;
}

//-----------------------------------------

Message Scheme::encode(CZZ*& vals, long slots) {
	ZZX poly;
	poly.SetLength(params.N);
	long idx = 0;
	long logSlots = log2(slots);
	long gap = (params.N >> logSlots);
	CZZ* fftInv = NumUtils::fftSpecialInv(vals, slots, params.ksiPows);
	for (long i = 0; i < slots; ++i) {
		poly.rep[idx] = fftInv[i].r;
		idx += gap;
	}
	return Message(poly, slots);
}

Cipher Scheme::encrypt(Message& msg, long level) {
	ZZX c0, c1;
	ZZ qi = getqi(level);
	rlweInstance(c0, c1, qi);
	Ring2Utils::add(c0, msg.mx, c0, qi, params.N);
	return Cipher(c0, c1, msg.slots, level);
}

Cipher Scheme::encryptFull(CZZ*& vals, long slots, long level) {
	CZZ* conj = doubleConjugate(vals, slots);
	Message msg = encode(conj, slots * 2);
	return encrypt(msg, level);
}

Cipher Scheme::encryptFull(CZZ& val, long level) {
	CZZ* conj = doubleConjugate(val);
	Message msg = encode(conj, 2);
	return encrypt(msg, level);
}

Cipher* Scheme::encryptFullSingleArray(CZZ*& vals, long size) {
	Cipher* res = new Cipher[size];
	for (long i = 0; i < size; ++i) {
		res[i] = encryptFull(vals[i]);
	}
	return res;
}

//-----------------------------------------

Message Scheme::decrypt(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZX poly;
	poly.SetLength(params.N);
	Ring2Utils::mult(poly, cipher.c1, secretKey.s, qi, params.N);
	Ring2Utils::add(poly, poly, cipher.c0, qi, params.N);
	Message msg(poly, cipher.slots, cipher.level);
	return msg;
}

CZZ* Scheme::decode(Message& msg) {
	CZZ* fftinv = new CZZ[msg.slots];
	ZZ qi = getqi(msg.level);

	long idx = 0;
	long logSlots = log2(msg.slots);
	long gap = (params.N >> logSlots);
	for (long i = 0; i < msg.slots; ++i) {
		CZZ c(msg.mx.rep[idx], ZZ(0));
		trueValue(c, qi);
		fftinv[i] = c;
		idx += gap;
	}
	return NumUtils::fftSpecial(fftinv, msg.slots, params.ksiPows);
}

CZZ* Scheme::deConjugate(CZZ*& vals, long slots) {
	CZZ* res = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		res[i] = vals[i];
	}
	return res;
}

CZZ* Scheme::decryptFull(Cipher& cipher) {
	Message msg = decrypt(cipher);
	CZZ* conj = decode(msg);
	return deConjugate(conj, msg.slots);
}

CZZ Scheme::decryptFullSingle(Cipher& cipher) {
	Message msg = decrypt(cipher);
	CZZ* conj = decode(msg);
	return conj[0];
}

CZZ* Scheme::decryptFullSingleArray(Cipher*& ciphers, long size) {
	CZZ* res = new CZZ[size];
	for (int i = 0; i < size; ++i) {
		Message msg = decrypt(ciphers[i]);
		CZZ* conj = decode(msg);
		res[i] = conj[0];
	}
	return res;
}

//-----------------------------------------

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX c0, c1;

	Ring2Utils::add(c0, cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::add(c1, cipher1.c1, cipher2.c1, qi, params.N);

	Cipher res(c0, c1, cipher1.slots, cipher1.level);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::addAndEqual(cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.c1, cipher2.c1, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::addConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;

	AddMod(c0.rep[0], cipher.c0.rep[0], cnst, qi);
	c0.normalize();
	Cipher newCipher(c0, c1, cipher.slots, cipher.level);
	return newCipher;
}

Cipher Scheme::addConst(Cipher& cipher, CZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0 = cipher.c0;
	ZZX c1 = cipher.c1;

	CZZ* cconj = doubleConjugate(cnst);
	Message cmsg = encode(cconj, cipher.slots);
	Cipher cnstcipher = encrypt(cmsg, cipher.level);
	return add(cipher, cnstcipher);
}

void Scheme::addConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	AddMod(cipher.c0.rep[0], cipher.c0.rep[0], cnst, qi);
	cipher.c0.normalize();
}

//-----------------------------------------

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX c0 , c1;

	Ring2Utils::sub(c0, cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::sub(c1, cipher1.c1, cipher2.c1, qi, params.N);

	Cipher res(c0, c1, cipher1.slots, cipher1.level);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::subAndEqual(cipher1.c0, cipher2.c0, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.c1, cipher2.c1, qi, params.N);
}

//-----------------------------------------

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

	Cipher cipher(mulC0, mulC1, cipher1.slots, cipher1.level);
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
}

//-----------------------------------------

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

	Cipher c(mulC0, mulC1, cipher.slots, cipher.level);
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
}

//-----------------------------------------

Cipher Scheme::multByConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX c0, c1;
	Ring2Utils::multByConst(c0, cipher.c0, cnst, qi, params.N);
	Ring2Utils::multByConst(c1, cipher.c1, cnst, qi, params.N);

	Cipher newCipher(c0, c1, cipher.slots, cipher.level);
	return newCipher;
}

void Scheme::multByConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::multByConstAndEqual(cipher.c0, cnst, qi, params.N);
	Ring2Utils::multByConstAndEqual(cipher.c1, cnst, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::multByMonomial(Cipher& cipher, const long& degree) {
	ZZX c0 , c1;

	Ring2Utils::multByMonomial(c0, cipher.c0, degree, params.N);
	Ring2Utils::multByMonomial(c1, cipher.c1, degree, params.N);

	Cipher res(c0, c1, cipher.slots, cipher.level);
	return res;
}

void Scheme::multByMonomialAndEqual(Cipher& cipher, const long& degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.c0, degree, params.N);
	Ring2Utils::multByMonomialAndEqual(cipher.c1, degree, params.N);
}

//-----------------------------------------

Cipher Scheme::leftShift(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	ZZX c0, c1;

	Ring2Utils::leftShift(c0, cipher.c0, bits, logqi, params.N);
	Ring2Utils::leftShift(c1, cipher.c1, bits, logqi, params.N);

	Cipher newCipher(c0, c1, cipher.slots, cipher.level);
	return newCipher;
}

void Scheme::leftShiftAndEqual(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	Ring2Utils::leftShiftAndEqual(cipher.c0, bits, logqi, params.N);
	Ring2Utils::leftShiftAndEqual(cipher.c1, bits, logqi, params.N);
}

//-----------------------------------------

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	ZZX c0, c1;

	Ring2Utils::rightShift(c0, cipher.c0, logdf, params.N);
	Ring2Utils::rightShift(c1, cipher.c1, logdf, params.N);

	Cipher newCipher(c0, c1, cipher.slots, newLevel);
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
}

void Scheme::modSwitchAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modSwitchAndEqual(cipher, newLevel);
}

//-----------------------------------------

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	long newLogqi = getLogqi(newLevel);
	ZZX c0, c1;
	Ring2Utils::truncate(c0, cipher.c0, newLogqi, params.N);
	Ring2Utils::truncate(c1, cipher.c1, newLogqi, params.N);

	Cipher newCipher(c0, c1, cipher.slots, newLevel);
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

//-----------------------------------------

Cipher Scheme::multAndModSwitch(Cipher& cipher1, Cipher& cipher2) {
	Cipher c = mult(cipher1, cipher2);
	modSwitchAndEqual(c);
	return c;
}

void Scheme::multModSwitchAndEqual(Cipher& cipher1, Cipher& cipher2) {
	multAndEqual(cipher1, cipher2);
	modSwitchAndEqual(cipher1);
}

//-----------------------------------------

//Cipher Scheme::rotate(Cipher& cipher, long& steps) {
//
//}
//
//void Scheme::rotateAndEqual(Cipher& cipher, long& steps) {
//
//}
