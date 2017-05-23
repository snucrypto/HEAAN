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

void Scheme::rlweInstance(ZZX& b, ZZX& a, ZZ& qi) {
	ZZX v, e;
	NumUtils::sampleZO(v, params.N);
	Ring2Utils::mult(b, v, publicKey.b, qi, params.N);
//	NumUtils::sampleGauss(e, params.N, params.sigma);
//	Ring2Utils::add(b, e, b, qi, params.N);

	Ring2Utils::mult(a, v, publicKey.a, qi, params.N);
//	NumUtils::sampleGauss(e, params.N, params.sigma);
//	Ring2Utils::add(a, e, a, qi, params.N);
}

void Scheme::rlweInstance(ZZX& b, ZZX& a) {
	rlweInstance(b, a, params.q);
}

//-----------------------------------------

CZZ* Scheme::groupidx(CZZ*& vals, long slots) {
	CZZ* res = new CZZ[slots * 2];
	long logslots = log2(slots);
	for (long i = 0; i < slots; ++i) {
		res[(params.group3pows[logslots][i] - 1) / 2] = vals[i];
		res[(params.group3powsInv[logslots][i] - 1) / 2] = vals[i].conjugate();
	}
	return res;
}

CZZ* Scheme::groupidx(CZZ& val) {
	CZZ* res;
	res = new CZZ[2];
	res[0] = val;
	res[1] = val.conjugate();
	return res;
}

CZZ* Scheme::degroupidx(CZZ*& vals, long dslots) {
	long slots = dslots / 2;
	long logslots = log2(slots);
	CZZ* res = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		res[i] = vals[(params.group3pows[logslots][i] - 1) / 2];
	}
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
	ZZX b, a;
	ZZ qi = getqi(level);
	rlweInstance(b, a, qi);
	Ring2Utils::add(b, msg.mx, b, qi, params.N);
	return Cipher(b, a, msg.slots, level);
}

Cipher Scheme::encryptFull(CZZ*& vals, long slots, long level) {
	CZZ* conj = groupidx(vals, slots);
	Message msg = encode(conj, slots * 2);
	return encrypt(msg, level);
}

Cipher Scheme::encryptFull(CZZ& val, long level) {
	CZZ* conj = groupidx(val);
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
	Ring2Utils::mult(poly, cipher.a, secretKey.s, qi, params.N);
	Ring2Utils::add(poly, poly, cipher.b, qi, params.N);
	Message msg(poly, cipher.slots, cipher.level);
	return msg;
}

CZZ* Scheme::decode(Message& msg) {
	CZZ* fftinv = new CZZ[msg.slots];
	ZZ qi = getqi(msg.level);

	long idx = 0;
	long gap = params.N / msg.slots;
	for (long i = 0; i < msg.slots; ++i) {
		CZZ c(msg.mx.rep[idx], ZZ(0));
		trueValue(c, qi);
		fftinv[i] = c;
		idx += gap;
	}
	return NumUtils::fftSpecial(fftinv, msg.slots, params.ksiPows);
}

CZZ* Scheme::decryptFull(Cipher& cipher) {
	Message msg = decrypt(cipher);
	CZZ* conj = decode(msg);
	return degroupidx(conj, msg.slots);
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
	ZZX b, a;

	Ring2Utils::add(b, cipher1.b, cipher2.b, qi, params.N);
	Ring2Utils::add(a, cipher1.a, cipher2.a, qi, params.N);

	Cipher res(b, a, cipher1.slots, cipher1.level);
	return res;
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::addAndEqual(cipher1.b, cipher2.b, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.a, cipher2.a, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::addConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX b = cipher.b;
	ZZX a = cipher.a;

	AddMod(b.rep[0], cipher.b.rep[0], cnst, qi);
	b.normalize();
	Cipher newCipher(b, a, cipher.slots, cipher.level);
	return newCipher;
}

Cipher Scheme::addConst(Cipher& cipher, CZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX b = cipher.b;
	ZZX a = cipher.a;

	CZZ* cconj = groupidx(cnst);
	Message cmsg = encode(cconj, cipher.slots);
	Cipher cnstcipher = encrypt(cmsg, cipher.level);
	return add(cipher, cnstcipher);
}

void Scheme::addConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	AddMod(cipher.b.rep[0], cipher.b.rep[0], cnst, qi);
	cipher.b.normalize();
}

//-----------------------------------------

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX b, a;

	Ring2Utils::sub(b, cipher1.b, cipher2.b, qi, params.N);
	Ring2Utils::sub(a, cipher1.a, cipher2.a, qi, params.N);

	Cipher res(b, a, cipher1.slots, cipher1.level);
	return res;
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::subAndEqual(cipher1.b, cipher2.b, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.a, cipher2.a, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::mult(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX bb, aa, amult, bmult;

	ZZX add1, add2, mult;
	Ring2Utils::add(add1, cipher1.b, cipher1.a, qi, params.N);
	Ring2Utils::add(add2, cipher2.b, cipher2.a, qi, params.N);
	Ring2Utils::mult(mult, add1, add2, qi, params.N);

	Ring2Utils::mult(bb, cipher1.b, cipher2.b, qi, params.N);
	Ring2Utils::mult(aa, cipher1.a, cipher2.a, qi, params.N);

	Ring2Utils::mult(amult, aa, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(bmult, aa, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(amult, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bmult, params.logP, params.N);

	Ring2Utils::addAndEqual(amult, mult, qi, params.N);
	Ring2Utils::subAndEqual(amult, bb, qi, params.N);
	Ring2Utils::subAndEqual(amult, aa, qi, params.N);
	Ring2Utils::addAndEqual(bmult, bb, qi, params.N);

	Cipher cipher(bmult, amult, cipher1.slots, cipher1.level);
	return cipher;
}

void Scheme::multAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX bb, aa;

	ZZX add1, add2, mult;
	Ring2Utils::mult(bb, cipher1.b, cipher2.b, qi, params.N);
	Ring2Utils::add(add1, cipher1.b, cipher1.a, qi, params.N);
	Ring2Utils::add(add2, cipher2.b, cipher2.a, qi, params.N);
	Ring2Utils::mult(mult, add1, add2, qi, params.N);
	Ring2Utils::mult(aa, cipher1.a, cipher2.a, qi, params.N);

	Ring2Utils::mult(cipher1.a, aa, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(cipher1.b, aa, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(cipher1.a, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(cipher1.b, params.logP, params.N);

	Ring2Utils::addAndEqual(cipher1.a, mult, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.a, bb, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.a, aa, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.b, bb, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::square(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX bb, ab, aa, bmult, amult;

	Ring2Utils::square(bb, cipher.b, qi, params.N);
	Ring2Utils::mult(ab, cipher.a, cipher.b, qi, params.N);
	Ring2Utils::addAndEqual(ab, ab, qi, params.N);
	Ring2Utils::square(aa, cipher.a, qi, params.N);

	Ring2Utils::mult(amult, aa, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(bmult, aa, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(amult, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bmult, params.logP, params.N);

	Ring2Utils::addAndEqual(amult, ab, qi, params.N);
	Ring2Utils::addAndEqual(bmult, bb, qi, params.N);

	Cipher c(bmult, amult, cipher.slots, cipher.level);
	return c;
}

void Scheme::squareAndEqual(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX bb, ab, aa, bmult, amult;

	Ring2Utils::square(bb, cipher.b, qi, params.N);
	Ring2Utils::mult(ab, cipher.b, cipher.a, qi, params.N);
	Ring2Utils::addAndEqual(ab, ab, qi, params.N);
	Ring2Utils::square(aa, cipher.a, qi, params.N);

	Ring2Utils::mult(amult, aa, publicKey.aStar, Pqi, params.N);
	Ring2Utils::mult(bmult, aa, publicKey.bStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(amult, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bmult, params.logP, params.N);

	Ring2Utils::addAndEqual(amult, ab, qi, params.N);
	Ring2Utils::addAndEqual(bmult, bb, qi, params.N);

	cipher.b = bmult;
	cipher.a = amult;
}

//-----------------------------------------

Cipher Scheme::multByConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX b, a;
	Ring2Utils::multByConst(b, cipher.b, cnst, qi, params.N);
	Ring2Utils::multByConst(a, cipher.a, cnst, qi, params.N);

	Cipher newCipher(b, a, cipher.slots, cipher.level);
	return newCipher;
}

void Scheme::multByConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::multByConstAndEqual(cipher.b, cnst, qi, params.N);
	Ring2Utils::multByConstAndEqual(cipher.a, cnst, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::multByMonomial(Cipher& cipher, const long& degree) {
	ZZX b, a;

	Ring2Utils::multByMonomial(b, cipher.b, degree, params.N);
	Ring2Utils::multByMonomial(a, cipher.a, degree, params.N);

	Cipher res(b, a, cipher.slots, cipher.level);
	return res;
}

void Scheme::multByMonomialAndEqual(Cipher& cipher, const long& degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.b, degree, params.N);
	Ring2Utils::multByMonomialAndEqual(cipher.a, degree, params.N);
}

//-----------------------------------------

Cipher Scheme::leftShift(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	ZZX b, a;

	Ring2Utils::leftShift(b, cipher.b, bits, logqi, params.N);
	Ring2Utils::leftShift(a, cipher.a, bits, logqi, params.N);

	Cipher newCipher(b, a, cipher.slots, cipher.level);
	return newCipher;
}

void Scheme::leftShiftAndEqual(Cipher& cipher, long& bits) {
	long logqi = getLogqi(cipher.level);
	Ring2Utils::leftShiftAndEqual(cipher.b, bits, logqi, params.N);
	Ring2Utils::leftShiftAndEqual(cipher.a, bits, logqi, params.N);
}

//-----------------------------------------

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	ZZX b, a;

	Ring2Utils::rightShift(b, cipher.b, logdf, params.N);
	Ring2Utils::rightShift(a, cipher.a, logdf, params.N);

	Cipher newCipher(b, a, cipher.slots, newLevel);
	return newCipher;
}

Cipher Scheme::modSwitch(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modSwitch(cipher, newLevel);
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	Ring2Utils::rightShiftAndEqual(cipher.b, logdf, params.N);
	Ring2Utils::rightShiftAndEqual(cipher.a, logdf, params.N);
	cipher.level = newLevel;
}

void Scheme::modSwitchAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modSwitchAndEqual(cipher, newLevel);
}

//-----------------------------------------

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	long newLogqi = getLogqi(newLevel);
	ZZX b, a;
	Ring2Utils::truncate(b, cipher.b, newLogqi, params.N);
	Ring2Utils::truncate(a, cipher.a, newLogqi, params.N);

	Cipher newCipher(b, a, cipher.slots, newLevel);
	return newCipher;
}

Cipher Scheme::modEmbed(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modEmbed(cipher, newLevel);
}

void Scheme::modEmbedAndEqual(Cipher& cipher, long newLevel) {
	long newLogqi = getLogqi(newLevel);
	Ring2Utils::truncateAndEqual(cipher.b, newLogqi, params.N);
	Ring2Utils::truncateAndEqual(cipher.b, newLogqi, params.N);
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

Cipher Scheme::rotate2(Cipher& cipher, long& logPow) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX brot, arot, aastar, abstar;

	long pow = (1 << logPow);

	Ring2Utils::inpower(brot, cipher.b, params.group3pows[params.logNh][pow], params.N);
	Ring2Utils::inpower(arot, cipher.a, params.group3pows[params.logNh][pow], params.N);

	Ring2Utils::mult(aastar, publicKey.aKeySwitch[logPow], arot, Pqi, params.N);
	Ring2Utils::mult(abstar, publicKey.bKeySwitch[logPow], arot, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(aastar, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(abstar, params.logP, params.N);

	Ring2Utils::addAndEqual(abstar, brot, qi, params.N);
	return Cipher(abstar, aastar, cipher.slots, cipher.level);
}

void Scheme::rotate2AndEqual(Cipher& cipher, long& logPow) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX brot, arot, aastar, abstar;

	long pow = (1 << logPow);

	Ring2Utils::inpower(brot, cipher.b, params.group3pows[params.logNh][pow], params.N);
	Ring2Utils::inpower(arot, cipher.a, params.group3pows[params.logNh][pow], params.N);

	Ring2Utils::mult(aastar, publicKey.aKeySwitch[logPow], arot, Pqi, params.N);
	Ring2Utils::mult(abstar, publicKey.bKeySwitch[logPow], arot, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(aastar, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(abstar, params.logP, params.N);

	Ring2Utils::addAndEqual(abstar, brot, qi, params.N);

	cipher.b = abstar;
	cipher.a = aastar;
}

Cipher Scheme::rotate(Cipher& cipher, long& steps) {
	Cipher res = cipher;
	long logsteps = log2(steps);
	for (long i = 0; i < logsteps + 1; ++i) {
		if(bit(steps, i)) {
			res = rotate2(res, i);
		}
	}
	return res;
}

void Scheme::rotateAndEqual(Cipher& cipher, long& steps) {
	long logsteps = log2(steps);
	for (long i = 0; i < logsteps; ++i) {
		if(bit(steps, i)) {
			rotate2AndEqual(cipher, i);
		}
	}
}
