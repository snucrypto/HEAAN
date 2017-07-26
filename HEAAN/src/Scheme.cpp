#include "Scheme.h"

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <cmath>
#include <vector>

#include "CZZ.h"
#include "NumUtils.h"
#include "Ring2Utils.h"
#include "StringUtils.h"
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

CZZ* Scheme::groupidx(CZZ*& vals, long slots) {
	CZZ* res = new CZZ[slots * 2];
	long logslots = log2(slots);
	for (long i = 0; i < slots; ++i) {
		res[(params.rotGroup[logslots][i] - 1) / 2] = vals[i];
		res[(params.rotGroupInv[logslots][i] - 1) / 2] = vals[i].conjugate();
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

CZZ* Scheme::degroupidx(CZZ*& vals, long slots) {
	long logslots = log2(slots);
	CZZ* res = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		res[i] = vals[(params.rotGroup[logslots][i] - 1) / 2];
	}
	return res;
}

//-----------------------------------------

Message Scheme::encode(CZZ*& vals, long slots) {
	ZZX mx;
	mx.SetLength(params.N);
	long idx = 0;
	long doubleslots = slots * 2;
	long logDoubleslots = log2(slots) + 1;
	long gap = (params.N >> logDoubleslots);
	CZZ* fftInv = NumUtils::fftSpecialInv(vals, doubleslots, aux.ksiPowsr, aux.ksiPowsi, params.logp);
	for (long i = 0; i < doubleslots; ++i) {
		mx.rep[idx] = fftInv[i].r;
		idx += gap;
	}
	return Message(mx, slots);
}

Cipher Scheme::encryptMsg(Message& msg, long level) {
	ZZX ax, bx, vx, eax, ebx;
	ZZ qi = getqi(level);
	NumUtils::sampleZO(vx, params.N, 64);

	Ring2Utils::mult(ax, vx, publicKey.ax, qi, params.N);
	NumUtils::sampleGauss(eax, params.N, params.sigma);
	Ring2Utils::addAndEqual(ax, eax, qi, params.N);

	Ring2Utils::mult(bx, vx, publicKey.bx, qi, params.N);
	NumUtils::sampleGauss(ebx, params.N, params.sigma);
	Ring2Utils::addAndEqual(bx, ebx, qi, params.N);

	Ring2Utils::addAndEqual(bx, msg.mx, qi, params.N);

	return Cipher(ax, bx, msg.slots, level);
}

Cipher Scheme::encrypt(CZZ*& vals, long slots, long level) {
	CZZ* gvals = groupidx(vals, slots);
	Message msg = encode(gvals, slots);
	return encryptMsg(msg, level);
}

Cipher Scheme::encryptSingle(CZZ& val, long level) {
	CZZ* gvals = groupidx(val);
	Message msg = encode(gvals, 1);
	return encryptMsg(msg, level);
}

//-----------------------------------------

Message Scheme::decryptMsg(SecKey& secretKey, Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZX mx;
	mx.SetLength(params.N);
	Ring2Utils::mult(mx, cipher.ax, secretKey.sx, qi, params.N);
	Ring2Utils::addAndEqual(mx, cipher.bx, qi, params.N);
	return Message(mx, cipher.slots, cipher.level);
}

CZZ* Scheme::decode(Message& msg) {
	long doubleslots = msg.slots * 2;
	CZZ* fftinv = new CZZ[doubleslots];
	ZZ qi = getqi(msg.level);
	long logqi = getLogqi(msg.level);

	long idx = 0;
	long gap = params.N / doubleslots;
	for (long i = 0; i < doubleslots; ++i) {
		ZZ tmp = msg.mx.rep[idx] % qi;
		if(NumBits(tmp) == logqi) tmp -= qi;
		fftinv[i] = CZZ(tmp, ZZ(0));
		idx += gap;
	}
	return NumUtils::fftSpecial(fftinv, doubleslots, aux.ksiPowsr, aux.ksiPowsi, params.logp);
}

CZZ* Scheme::decrypt(SecKey& secretKey, Cipher& cipher) {
	Message msg = decryptMsg(secretKey, cipher);
	CZZ* gvals = decode(msg);
	return degroupidx(gvals, msg.slots);
}

CZZ Scheme::decryptSingle(SecKey& secretKey, Cipher& cipher) {
	Message msg = decryptMsg(secretKey, cipher);
	CZZ* gvals = decode(msg);
	return gvals[0];
}

//-----------------------------------------

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX ax, bx;

	Ring2Utils::add(ax, cipher1.ax, cipher2.ax, qi, params.N);
	Ring2Utils::add(bx, cipher1.bx, cipher2.bx, qi, params.N);

	return Cipher(ax, bx, cipher1.slots, cipher1.level);
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::addAndEqual(cipher1.ax, cipher2.ax, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.bx, cipher2.bx, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::addConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	AddMod(bx.rep[0], cipher.bx.rep[0], cnst, qi);
	bx.normalize();
	return Cipher(ax, bx, cipher.slots, cipher.level);
}

void Scheme::addConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnst, qi);
	cipher.bx.normalize();
}

//-----------------------------------------

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZX ax, bx;

	Ring2Utils::sub(ax, cipher1.ax, cipher2.ax, qi, params.N);
	Ring2Utils::sub(bx, cipher1.bx, cipher2.bx, qi, params.N);

	return Cipher(ax, bx, cipher1.slots, cipher1.level);
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	Ring2Utils::subAndEqual(cipher1.ax, cipher2.ax, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.bx, cipher2.bx, qi, params.N);
}

Cipher Scheme::conjugate(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX bxconj, bxres, axres;

	Ring2Utils::conjugate(bxconj, cipher.bx, params.N);
	Ring2Utils::conjugate(bxres, cipher.ax, params.N);

	Ring2Utils::mult(axres, bxres, publicKey.axConj, Pqi, params.N);
	Ring2Utils::multAndEqual(bxres, publicKey.bxConj, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logP, params.N);

	Ring2Utils::addAndEqual(bxres, bxconj, qi, params.N);
	return Cipher(axres, bxres, cipher.slots, cipher.level);
}

void Scheme::conjugateAndEqual(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX bxconj, bxres, axres;

	Ring2Utils::conjugate(bxconj, cipher.bx, params.N);
	Ring2Utils::conjugate(bxres, cipher.ax, params.N);

	Ring2Utils::mult(axres, bxres, publicKey.axConj, Pqi, params.N);
	Ring2Utils::multAndEqual(bxres, publicKey.bxConj, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logP, params.N);

	Ring2Utils::addAndEqual(bxres, bxconj, qi, params.N);

	cipher.ax = axres;
	cipher.bx = bxres;
}

Cipher Scheme::imult(Cipher& cipher) {

	ZZ qi = getqi(cipher.level);
	ZZX bxres, axres, axtmp, bxtmp;

	Ring2Utils::multByMonomial(axtmp, cipher.ax, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(axtmp, aux.poversqrt2, qi, params.N);

	Ring2Utils::multByMonomial(bxtmp, cipher.bx, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(bxtmp, aux.poversqrt2, qi, params.N);

	Ring2Utils::multByMonomial(axres, cipher.ax, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(axres, aux.poversqrt2, qi, params.N);

	Ring2Utils::multByMonomial(bxres, cipher.bx, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(bxres, aux.poversqrt2, qi, params.N);

	Ring2Utils::addAndEqual(axres, axtmp, qi, params.N);
	Ring2Utils::addAndEqual(bxres, bxtmp, qi, params.N);

	Cipher res(axres, bxres, cipher.slots, cipher.level);
	modSwitchOneAndEqual(res);
	return res;
}

void Scheme::imultAndEqual(Cipher& cipher) {

	ZZ qi = getqi(cipher.level);
	ZZX axtmp, bxtmp;

	Ring2Utils::multByMonomial(axtmp, cipher.ax, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(axtmp, aux.poversqrt2, qi, params.N);

	Ring2Utils::multByMonomial(bxtmp, cipher.bx, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(bxtmp, aux.poversqrt2, qi, params.N);

	Ring2Utils::multByMonomialAndEqual(cipher.ax, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(cipher.ax, aux.poversqrt2, qi, params.N);

	Ring2Utils::multByMonomialAndEqual(cipher.bx, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, aux.poversqrt2, qi, params.N);

	Ring2Utils::addAndEqual(cipher.ax, axtmp, qi, params.N);
	Ring2Utils::addAndEqual(cipher.bx, bxtmp, qi, params.N);

	modSwitchOneAndEqual(cipher);
}
//-----------------------------------------

Cipher Scheme::mult(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX axbx1 = Ring2Utils::add(cipher1.ax, cipher1.bx, qi, params.N);
	ZZX axbx2 = Ring2Utils::add(cipher2.ax, cipher2.bx, qi, params.N);
	Ring2Utils::multAndEqual(axbx1, axbx2, qi, params.N);

	ZZX bxbx = Ring2Utils::mult(cipher1.bx, cipher2.bx, qi, params.N);
	ZZX axax = Ring2Utils::mult(cipher1.ax, cipher2.ax, qi, params.N);

	ZZX axmult = Ring2Utils::mult(axax, publicKey.axStar, Pqi, params.N);
	ZZX bxmult = Ring2Utils::mult(axax, publicKey.bxStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axmult, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxmult, params.logP, params.N);

	Ring2Utils::addAndEqual(axmult, axbx1, qi, params.N);
	Ring2Utils::subAndEqual(axmult, bxbx, qi, params.N);
	Ring2Utils::subAndEqual(axmult, axax, qi, params.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, qi, params.N);

	return Cipher(axmult, bxmult, cipher1.slots, cipher1.level);
}

void Scheme::multAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ qi = getqi(cipher1.level);
	ZZ Pqi = getPqi(cipher1.level);

	ZZX axbx1 = Ring2Utils::add(cipher1.ax, cipher1.bx, qi, params.N);
	ZZX axbx2 = Ring2Utils::add(cipher2.ax, cipher2.bx, qi, params.N);
	Ring2Utils::multAndEqual(axbx1, axbx2, qi, params.N);

	ZZX bxbx = Ring2Utils::mult(cipher1.bx, cipher2.bx, qi, params.N);
	ZZX axax = Ring2Utils::mult(cipher1.ax, cipher2.ax, qi, params.N);

	cipher1.ax = Ring2Utils::mult(axax, publicKey.axStar, Pqi, params.N);
	cipher1.bx = Ring2Utils::mult(axax, publicKey.bxStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(cipher1.ax, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(cipher1.bx, params.logP, params.N);

	Ring2Utils::addAndEqual(cipher1.ax, axbx1, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.ax, bxbx, qi, params.N);
	Ring2Utils::subAndEqual(cipher1.ax, axax, qi, params.N);
	Ring2Utils::addAndEqual(cipher1.bx, bxbx, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::square(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);
	ZZX axax, axbx, bxbx, bxmult, axmult;

	Ring2Utils::square(bxbx, cipher.bx, qi, params.N);
	Ring2Utils::mult(axbx, cipher.ax, cipher.bx, qi, params.N);
	Ring2Utils::addAndEqual(axbx, axbx, qi, params.N);
	Ring2Utils::square(axax, cipher.ax, qi, params.N);

	Ring2Utils::mult(axmult, axax, publicKey.axStar, Pqi, params.N);
	Ring2Utils::mult(bxmult, axax, publicKey.bxStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axmult, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxmult, params.logP, params.N);

	Ring2Utils::addAndEqual(axmult, axbx, qi, params.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, qi, params.N);

	return Cipher(axmult, bxmult, cipher.slots, cipher.level);
}

void Scheme::squareAndEqual(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);
	ZZX bxbx, axbx, axax, bxmult, axmult;

	Ring2Utils::square(bxbx, cipher.bx, qi, params.N);
	Ring2Utils::mult(axbx, cipher.bx, cipher.ax, qi, params.N);
	Ring2Utils::addAndEqual(axbx, axbx, qi, params.N);
	Ring2Utils::square(axax, cipher.ax, qi, params.N);

	Ring2Utils::mult(axmult, axax, publicKey.axStar, Pqi, params.N);
	Ring2Utils::mult(bxmult, axax, publicKey.bxStar, Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axmult, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxmult, params.logP, params.N);

	Ring2Utils::addAndEqual(axmult, axbx, qi, params.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, qi, params.N);

	cipher.bx = bxmult;
	cipher.ax = axmult;
}

//-----------------------------------------

Cipher Scheme::multByConst(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	ZZX ax, bx;
	Ring2Utils::multByConst(ax, cipher.ax, cnst, qi, params.N);
	Ring2Utils::multByConst(bx, cipher.bx, cnst, qi, params.N);

	return Cipher(ax, bx, cipher.slots, cipher.level);
}

void Scheme::multByConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::multByConstAndEqual(cipher.ax, cnst, qi, params.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, cnst, qi, params.N);
}

Cipher Scheme::multByConstBySlots(Cipher& cipher, CZZ*& cnstvec) {
	ZZ qi = getqi(cipher.level);
	CZZ* gcnstvec = groupidx(cnstvec, cipher.slots);
	Message msg = encode(gcnstvec, cipher.slots);

	ZZX axres, bxres;
	Ring2Utils::mult(axres, cipher.ax, msg.mx, qi, params.N);
	Ring2Utils::mult(bxres, cipher.bx, msg.mx, qi, params.N);
	return Cipher(axres, bxres, cipher.slots, cipher.level);
}

void Scheme::multByConstBySlotsAndEqual(Cipher& cipher, CZZ*& cnstvec) {
	ZZ qi = getqi(cipher.level);
	CZZ* gcnstvec = groupidx(cnstvec, cipher.slots);
	Message msg = encode(gcnstvec, cipher.slots);

	Ring2Utils::multAndEqual(cipher.ax, msg.mx, qi, params.N);
	Ring2Utils::multAndEqual(cipher.bx, msg.mx, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::multByMonomial(Cipher& cipher, const long& degree) {
	ZZX ax, bx;

	Ring2Utils::multByMonomial(ax, cipher.ax, degree, params.N);
	Ring2Utils::multByMonomial(bx, cipher.bx, degree, params.N);

	return Cipher(ax, bx, cipher.slots, cipher.level);
}

void Scheme::multByMonomialAndEqual(Cipher& cipher, const long& degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, degree, params.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, degree, params.N);
}

//-----------------------------------------

Cipher Scheme::leftShift(Cipher& cipher, long& bits) {
	ZZ qi = getqi(cipher.level);
	ZZX ax, bx;

	Ring2Utils::leftShift(ax, cipher.ax, bits, qi, params.N);
	Ring2Utils::leftShift(bx, cipher.bx, bits, qi, params.N);

	return Cipher(ax, bx, cipher.slots, cipher.level);
}

void Scheme::leftShiftAndEqual(Cipher& cipher, long& bits) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::leftShiftAndEqual(cipher.ax, bits, qi, params.N);
	Ring2Utils::leftShiftAndEqual(cipher.bx, bits, qi, params.N);
}

void Scheme::doubleAndEqual(Cipher& cipher) {
	ZZ qi = getqi(cipher.level);
	Ring2Utils::doubleAndEqual(cipher.ax, qi, params.N);
	Ring2Utils::doubleAndEqual(cipher.bx, qi, params.N);
}

//-----------------------------------------

Cipher Scheme::modSwitch(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	ZZX ax, bx;

	Ring2Utils::rightShift(ax, cipher.ax, logdf, params.N);
	Ring2Utils::rightShift(bx, cipher.bx, logdf, params.N);

	return Cipher(ax, bx, cipher.slots, newLevel);
}

Cipher Scheme::modSwitchOne(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modSwitch(cipher, newLevel);
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long newLevel) {
	long logdf = params.logp * (newLevel-cipher.level);
	Ring2Utils::rightShiftAndEqual(cipher.ax, logdf, params.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, logdf, params.N);
	cipher.level = newLevel;
}

void Scheme::modSwitchOneAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modSwitchAndEqual(cipher, newLevel);
}

//-----------------------------------------

Cipher Scheme::modEmbed(Cipher& cipher, long newLevel) {
	if(cipher.level < newLevel) {
		ZZ newqi = getqi(newLevel);
		ZZX bx, ax;
		Ring2Utils::mod(ax, cipher.ax, newqi, params.N);
		Ring2Utils::mod(bx, cipher.bx, newqi, params.N);
		return Cipher(ax, bx, cipher.slots, newLevel);
	} else {
		Cipher c = cipher;
		return c;
	}
}

Cipher Scheme::modEmbedOne(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	return modEmbed(cipher, newLevel);
}

void Scheme::modEmbedAndEqual(Cipher& cipher, long newLevel) {
	if(cipher.level < newLevel) {
		ZZ newqi = getqi(newLevel);
		Ring2Utils::modAndEqual(cipher.ax, newqi, params.N);
		Ring2Utils::modAndEqual(cipher.bx, newqi, params.N);
		cipher.level = newLevel;
	}
}

void Scheme::modEmbedOneAndEqual(Cipher& cipher) {
	long newLevel = cipher.level + 1;
	modEmbedAndEqual(cipher, newLevel);
}

//-----------------------------------------

Cipher Scheme::multAndModSwitchOne(Cipher& cipher1, Cipher& cipher2) {
	Cipher cmult = mult(cipher1, cipher2);
	modSwitchOneAndEqual(cmult);
	return cmult;
}

void Scheme::multModSwitchOneAndEqual(Cipher& cipher1, Cipher& cipher2) {
	multAndEqual(cipher1, cipher2);
	modSwitchOneAndEqual(cipher1);
}

//-----------------------------------------

Cipher Scheme::leftRotateByPo2(Cipher& cipher, long& logrotSlots) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX bxrot, bxres, axres;

	long rotSlots = (1 << logrotSlots);

	Ring2Utils::inpower(bxrot, cipher.bx, params.rotGroup[params.logN - 1][rotSlots], params.q, params.N);
	Ring2Utils::inpower(bxres, cipher.ax, params.rotGroup[params.logN - 1][rotSlots], params.q, params.N);

	Ring2Utils::mult(axres, bxres, publicKey.axKeySwitch[logrotSlots], Pqi, params.N);
	Ring2Utils::multAndEqual(bxres, publicKey.bxKeySwitch[logrotSlots], Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logP, params.N);

	Ring2Utils::addAndEqual(bxres, bxrot, qi, params.N);
	return Cipher(axres, bxres, cipher.slots, cipher.level);
}

void Scheme::leftRotateByPo2AndEqual(Cipher& cipher, long& logrotSlots) {
	ZZ qi = getqi(cipher.level);
	ZZ Pqi = getPqi(cipher.level);

	ZZX bxrot, bxres, axres;

	long rotSlots = (1 << logrotSlots);

	Ring2Utils::inpower(bxrot, cipher.bx, params.rotGroup[params.logN - 1][rotSlots], params.q, params.N);
	Ring2Utils::inpower(bxres, cipher.ax, params.rotGroup[params.logN - 1][rotSlots], params.q, params.N);

	Ring2Utils::mult(axres, bxres, publicKey.axKeySwitch[logrotSlots], Pqi, params.N);
	Ring2Utils::multAndEqual(bxres, publicKey.bxKeySwitch[logrotSlots], Pqi, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logP, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logP, params.N);

	Ring2Utils::addAndEqual(bxres, bxrot, qi, params.N);

	cipher.ax = axres;
	cipher.bx = bxres;
}

Cipher Scheme::leftRotate(Cipher& cipher, long& rotSlots) {
	Cipher res = cipher;
	leftRotateAndEqual(res, rotSlots);
	return res;
}

void Scheme::leftRotateAndEqual(Cipher& cipher, long& rotSlots) {
	long remrotSlots = rotSlots % cipher.slots;
	long logrotSlots = log2(remrotSlots) + 1;
	for (long i = 0; i < logrotSlots; ++i) {
		if(bit(remrotSlots, i)) {
			leftRotateByPo2AndEqual(cipher, i);
		}
	}
}
