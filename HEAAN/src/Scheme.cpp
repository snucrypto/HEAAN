#include "Scheme.h"

#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <cmath>

#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Params.h"
#include "PubKey.h"
#include "Ring2Utils.h"

using namespace std;
using namespace NTL;

CZZ* Scheme::groupidx(CZZ*& vals, long slots) {
	CZZ* res = new CZZ[slots * 2];
	for (long i = 0; i < slots; ++i) {
		long idx = (params.rotGroup[i] % (slots << 2) - 1) / 2;
		res[idx] = vals[i];
		res[2 * slots - idx - 1] = vals[i].conjugate();
	}
	return res;
}

CZZ* Scheme::groupidx(CZZ& val) {
	CZZ* res = new CZZ[2];
	res[0] = val;
	res[1] = val.conjugate();
	return res;
}

CZZ* Scheme::degroupidx(CZZ*& vals, long slots) {
	CZZ* res = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		long idx = (params.rotGroup[i] % (slots << 2) - 1) / 2;
		res[i] = vals[idx];
	}
	return res;
}

//-----------------------------------------

Message Scheme::encodeWithBits(CZZ*& vals, long cbits, long slots) {
	long doubleslots = slots << 1;
	CZZ* gvals = new CZZ[doubleslots];
	for (long i = 0; i < slots; ++i) {
		long idx = (params.rotGroup[i] % (slots << 2) - 1) / 2;
		gvals[idx] = vals[i];
		gvals[doubleslots - idx - 1] = vals[i].conjugate();
	}
	ZZX mx;
	mx.SetLength(params.N);
	ZZ mod = power2_ZZ(cbits);
	long idx = 0;
	long gap = params.N / doubleslots;
	NumUtils::fftSpecialInv(gvals, doubleslots, aux);
	for (long i = 0; i < doubleslots; ++i) {
		mx.rep[idx] = gvals[i].r;
		idx += gap;
	}
	delete[] gvals;
	return Message(mx, mod, cbits, slots);
}

Message Scheme::encodeSingleWithBits(CZZ& val, long cbits) {
	CZZ* gvals = new CZZ[2];
	gvals[0] = val;
	gvals[1] = val.conjugate();
	ZZX mx;
	mx.SetLength(params.N);
	ZZ mod = power2_ZZ(cbits);
	long idx = 0;
	long gap = params.N / 2;
	NumUtils::fftSpecialInv(gvals, 2, aux);
	for (long i = 0; i < 2; ++i) {
		mx.rep[idx] = gvals[i].r;
		idx += gap;
	}
	delete[] gvals;
	return Message(mx, mod, cbits, 1);
}

Message Scheme::encode(CZZ*& vals, long slots) {
	long doubleslots = slots << 1;
	CZZ* gvals = new CZZ[doubleslots];
	for (long i = 0; i < slots; ++i) {
		long idx = (params.rotGroup[i] % (slots << 2) - 1) / 2;
		gvals[idx] = vals[i];
		gvals[doubleslots - idx - 1] = vals[i].conjugate();
	}

	ZZX mx;
	mx.SetLength(params.N);
	long idx = 0;
	long gap = params.N / doubleslots;

	NumUtils::fftSpecialInv(gvals, doubleslots, aux);

	for (long i = 0; i < doubleslots; ++i) {
		mx.rep[idx] = gvals[i].r;
		idx += gap;
	}
	delete[] gvals;
	return Message(mx, params.q, params.logq, slots);
}

Message Scheme::encodeSingle(CZZ& val) {
	CZZ* gvals = new CZZ[2];
	gvals[0] = val;
	gvals[1] = val.conjugate();

	ZZX mx;
	mx.SetLength(params.N);
	long idx = 0;
	long gap = params.N / 2;

	NumUtils::fftSpecialInv(gvals, 2, aux);

	for (long i = 0; i < 2; ++i) {
		mx.rep[idx] = gvals[i].r;
		idx += gap;
	}
	delete[] gvals;
	return Message(mx, params.q, params.logq, 1);
}

Cipher Scheme::encryptMsg(Message& msg) {
	ZZX ax, bx, vx, eax, ebx;
	NumUtils::sampleZO(vx, params.N);
	RLWE key = publicKey.keyMap.at(ENCRYPTION);

	Ring2Utils::mult(ax, vx, key.ax, msg.mod, params.N);
	NumUtils::sampleGauss(eax, params.N, params.sigma);
	Ring2Utils::addAndEqual(ax, eax, msg.mod, params.N);

	Ring2Utils::mult(bx, vx, key.bx, msg.mod, params.N);
	NumUtils::sampleGauss(ebx, params.N, params.sigma);
	Ring2Utils::addAndEqual(bx, ebx, msg.mod, params.N);

	Ring2Utils::addAndEqual(bx, msg.mx, msg.mod, params.N);

	return Cipher(ax, bx, msg.mod, msg.cbits, msg.slots);
}

Cipher Scheme::encryptWithBits(CZZ*& vals, long cbits, long slots) {
	Message msg = encodeWithBits(vals, cbits, slots);
	return encryptMsg(msg);
}

Cipher Scheme::encrypt(CZZ*& vals, long slots) {
	Message msg = encode(vals, slots);
	return encryptMsg(msg);
}

Cipher Scheme::encryptSingleWithBits(CZZ& val, long cbits) {
	Message msg = encodeSingleWithBits(val, cbits);
	return encryptMsg(msg);
}

Cipher Scheme::encryptSingle(CZZ& val) {
	Message msg = encodeSingle(val);
	return encryptMsg(msg);
}

//-----------------------------------------

Message Scheme::decryptMsg(SecKey& secretKey, Cipher& cipher) {
	ZZX mx;
	Ring2Utils::mult(mx, cipher.ax, secretKey.sx, cipher.mod, params.N);
	Ring2Utils::addAndEqual(mx, cipher.bx, cipher.mod, params.N);
	return Message(mx, cipher.mod, cipher.cbits, cipher.slots);
}

CZZ* Scheme::decode(Message& msg) {
	long doubleslots = msg.slots * 2;
	CZZ* fftinv = new CZZ[doubleslots];

	long idx = 0;
	long gap = params.N / doubleslots;
	for (long i = 0; i < doubleslots; ++i) {
		ZZ tmp = msg.mx.rep[idx] % msg.mod;
		if(NumBits(tmp) == msg.cbits) tmp -= msg.mod;
		fftinv[i] = CZZ(tmp, ZZ(0));
		idx += gap;
	}
	NumUtils::fftSpecial(fftinv, doubleslots, aux);
	return fftinv;
}

CZZ* Scheme::decrypt(SecKey& secretKey, Cipher& cipher) {
	Message msg = decryptMsg(secretKey, cipher);
	CZZ* gvals = decode(msg);
	CZZ* res = degroupidx(gvals, msg.slots);
	delete[] gvals;
	return res;
}

CZZ Scheme::decryptSingle(SecKey& secretKey, Cipher& cipher) {
	Message msg = decryptMsg(secretKey, cipher);
	CZZ* gvals = decode(msg);
	CZZ res = gvals[0];
	delete[] gvals;
	return res;
}

//-----------------------------------------

Cipher Scheme::add(Cipher& cipher1, Cipher& cipher2) {
	ZZX ax, bx;

	Ring2Utils::add(ax, cipher1.ax, cipher2.ax, cipher1.mod, params.N);
	Ring2Utils::add(bx, cipher1.bx, cipher2.bx, cipher1.mod, params.N);

	return Cipher(ax, bx, cipher1.mod, cipher1.cbits, cipher1.slots);
}

void Scheme::addAndEqual(Cipher& cipher1, Cipher& cipher2) {
	Ring2Utils::addAndEqual(cipher1.ax, cipher2.ax, cipher1.mod, params.N);
	Ring2Utils::addAndEqual(cipher1.bx, cipher2.bx, cipher1.mod, params.N);
}

//-----------------------------------------

Cipher Scheme::addConst(Cipher& cipher, ZZ& cnst) {
	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	AddMod(bx.rep[0], cipher.bx.rep[0], cnst, cipher.mod);
	return Cipher(ax, bx, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::addConstAndEqual(Cipher& cipher, ZZ& cnst) {
	ZZ mod = power2_ZZ(cipher.cbits);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnst, mod);
}

//-----------------------------------------

Cipher Scheme::sub(Cipher& cipher1, Cipher& cipher2) {
	ZZX ax, bx;

	Ring2Utils::sub(ax, cipher1.ax, cipher2.ax, cipher1.mod, params.N);
	Ring2Utils::sub(bx, cipher1.bx, cipher2.bx, cipher1.mod, params.N);

	return Cipher(ax, bx, cipher1.mod, cipher1.cbits, cipher1.slots);
}

void Scheme::subAndEqual(Cipher& cipher1, Cipher& cipher2) {
	Ring2Utils::subAndEqual(cipher1.ax, cipher2.ax, cipher1.mod, params.N);
	Ring2Utils::subAndEqual(cipher1.bx, cipher2.bx, cipher1.mod, params.N);
}

void Scheme::subAndEqual2(Cipher& cipher1, Cipher& cipher2) {
	Ring2Utils::subAndEqual2(cipher1.ax, cipher2.ax, cipher1.mod, params.N);
	Ring2Utils::subAndEqual2(cipher1.bx, cipher2.bx, cipher1.mod, params.N);
}

Cipher Scheme::conjugate(Cipher& cipher) {
	ZZ Pmod = cipher.mod << params.logq;

	ZZX bxconj, bxres, axres;

	Ring2Utils::conjugate(bxconj, cipher.bx, params.N);
	Ring2Utils::conjugate(bxres, cipher.ax, params.N);

	RLWE key = publicKey.keyMap.at(CONJUGATION);

	Ring2Utils::mult(axres, bxres, key.ax, Pmod, params.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logq, params.N);

	Ring2Utils::addAndEqual(bxres, bxconj, cipher.mod, params.N);
	return Cipher(axres, bxres, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::conjugateAndEqual(Cipher& cipher) {
	ZZ Pmod = cipher.mod << params.logq;

	ZZX bxconj, bxres, axres;

	Ring2Utils::conjugate(bxconj, cipher.bx, params.N);
	Ring2Utils::conjugate(bxres, cipher.ax, params.N);

	RLWE key = publicKey.keyMap.at(CONJUGATION);
	Ring2Utils::mult(axres, bxres, key.ax, Pmod, params.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logq, params.N);

	Ring2Utils::addAndEqual(bxres, bxconj, cipher.mod, params.N);

	cipher.ax = axres;
	cipher.bx = bxres;
}

Cipher Scheme::imult(Cipher& cipher, const long precisionBits) {
	ZZ tmp = EvaluatorUtils::evaluateVal(sqrt(to_RR(2.0)), precisionBits - 1);

	ZZX bxres, axres, axtmp, bxtmp;

	Ring2Utils::multByMonomial(axtmp, cipher.ax, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(axtmp, tmp, cipher.mod, params.N);

	Ring2Utils::multByMonomial(bxtmp, cipher.bx, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(bxtmp, tmp, cipher.mod, params.N);

	Ring2Utils::multByMonomial(axres, cipher.ax, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(axres, tmp, cipher.mod, params.N);

	Ring2Utils::multByMonomial(bxres, cipher.bx, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(bxres, tmp, cipher.mod, params.N);

	Ring2Utils::addAndEqual(axres, axtmp, cipher.mod, params.N);
	Ring2Utils::addAndEqual(bxres, bxtmp, cipher.mod, params.N);

	Cipher res(axres, bxres, cipher.mod, cipher.cbits, cipher.slots);
	modSwitchAndEqual(res, precisionBits);
	return res;
}

void Scheme::imultAndEqual(Cipher& cipher, const long precisionBits) {

	ZZ tmp = EvaluatorUtils::evaluateVal(sqrt(to_RR(2.0)), precisionBits - 1);

	ZZX axtmp, bxtmp;

	Ring2Utils::multByMonomial(axtmp, cipher.ax, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(axtmp, tmp, cipher.mod, params.N);

	Ring2Utils::multByMonomial(bxtmp, cipher.bx, params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(bxtmp, tmp, cipher.mod, params.N);

	Ring2Utils::multByMonomialAndEqual(cipher.ax, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(cipher.ax, tmp, cipher.mod, params.N);

	Ring2Utils::multByMonomialAndEqual(cipher.bx, 3 * params.N / 4, params.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, tmp, cipher.mod, params.N);

	Ring2Utils::addAndEqual(cipher.ax, axtmp, cipher.mod, params.N);
	Ring2Utils::addAndEqual(cipher.bx, bxtmp, cipher.mod, params.N);

	modSwitchAndEqual(cipher, precisionBits);
}

Cipher Scheme::mult(Cipher& cipher1, Cipher& cipher2) {
	ZZ Pmod = cipher1.mod << params.logq;

	ZZX axbx1 = Ring2Utils::add(cipher1.ax, cipher1.bx, cipher1.mod, params.N);
	ZZX axbx2 = Ring2Utils::add(cipher2.ax, cipher2.bx, cipher1.mod, params.N);
	Ring2Utils::multAndEqual(axbx1, axbx2, cipher1.mod, params.N);

	ZZX bxbx = Ring2Utils::mult(cipher1.bx, cipher2.bx, cipher1.mod, params.N);
	ZZX axax = Ring2Utils::mult(cipher1.ax, cipher2.ax, cipher1.mod, params.N);

	RLWE key = publicKey.keyMap.at(MULTIPLICATION);
	ZZX axmult = Ring2Utils::mult(axax, key.ax, Pmod, params.N);
	ZZX bxmult = Ring2Utils::mult(axax, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axmult, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxmult, params.logq, params.N);

	Ring2Utils::addAndEqual(axmult, axbx1, cipher1.mod, params.N);
	Ring2Utils::subAndEqual(axmult, bxbx, cipher1.mod, params.N);
	Ring2Utils::subAndEqual(axmult, axax, cipher1.mod, params.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, cipher1.mod, params.N);

	return Cipher(axmult, bxmult, cipher1.mod, cipher1.cbits, cipher1.slots);
}

void Scheme::multAndEqual(Cipher& cipher1, Cipher& cipher2) {
	ZZ Pmod = cipher1.mod << params.logq;

	ZZX axbx1 = Ring2Utils::add(cipher1.ax, cipher1.bx, cipher1.mod, params.N);
	ZZX axbx2 = Ring2Utils::add(cipher2.ax, cipher2.bx, cipher1.mod, params.N);
	Ring2Utils::multAndEqual(axbx1, axbx2, cipher1.mod, params.N);

	ZZX bxbx = Ring2Utils::mult(cipher1.bx, cipher2.bx, cipher1.mod, params.N);
	ZZX axax = Ring2Utils::mult(cipher1.ax, cipher2.ax, cipher1.mod, params.N);

	RLWE key = publicKey.keyMap.at(MULTIPLICATION);
	cipher1.ax = Ring2Utils::mult(axax, key.ax, Pmod, params.N);
	cipher1.bx = Ring2Utils::mult(axax, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(cipher1.ax, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(cipher1.bx, params.logq, params.N);

	Ring2Utils::addAndEqual(cipher1.ax, axbx1, cipher1.mod, params.N);
	Ring2Utils::subAndEqual(cipher1.ax, bxbx, cipher1.mod, params.N);
	Ring2Utils::subAndEqual(cipher1.ax, axax, cipher1.mod, params.N);
	Ring2Utils::addAndEqual(cipher1.bx, bxbx, cipher1.mod, params.N);
}

//-----------------------------------------

Cipher Scheme::square(Cipher& cipher) {
	ZZ Pmod = cipher.mod << params.logq;

	ZZX axax, axbx, bxbx, bxmult, axmult;

	Ring2Utils::square(bxbx, cipher.bx, cipher.mod, params.N);
	Ring2Utils::mult(axbx, cipher.ax, cipher.bx, cipher.mod, params.N);
	Ring2Utils::addAndEqual(axbx, axbx, cipher.mod, params.N);
	Ring2Utils::square(axax, cipher.ax, cipher.mod, params.N);

	RLWE key = publicKey.keyMap.at(MULTIPLICATION);
	Ring2Utils::mult(axmult, axax, key.ax, Pmod, params.N);
	Ring2Utils::mult(bxmult, axax, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axmult, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxmult, params.logq, params.N);

	Ring2Utils::addAndEqual(axmult, axbx, cipher.mod, params.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, cipher.mod, params.N);

	return Cipher(axmult, bxmult, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::squareAndEqual(Cipher& cipher) {
	ZZ Pmod = cipher.mod << params.logq;

	ZZX bxbx, axbx, axax, bxmult, axmult;

	Ring2Utils::square(bxbx, cipher.bx, cipher.mod, params.N);
	Ring2Utils::mult(axbx, cipher.bx, cipher.ax, cipher.mod, params.N);
	Ring2Utils::addAndEqual(axbx, axbx, cipher.mod, params.N);
	Ring2Utils::square(axax, cipher.ax, cipher.mod, params.N);

	RLWE key = publicKey.keyMap.at(MULTIPLICATION);
	Ring2Utils::mult(axmult, axax, key.ax, Pmod, params.N);
	Ring2Utils::mult(bxmult, axax, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axmult, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxmult, params.logq, params.N);

	Ring2Utils::addAndEqual(axmult, axbx, cipher.mod, params.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, cipher.mod, params.N);

	cipher.bx = bxmult;
	cipher.ax = axmult;
}

//-----------------------------------------

Cipher Scheme::multByConst(Cipher& cipher, ZZ& cnst) {
	ZZX ax, bx;
	Ring2Utils::multByConst(ax, cipher.ax, cnst, cipher.mod, params.N);
	Ring2Utils::multByConst(bx, cipher.bx, cnst, cipher.mod, params.N);

	return Cipher(ax, bx, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::multByConstAndEqual(Cipher& cipher, ZZ& cnst) {
	Ring2Utils::multByConstAndEqual(cipher.ax, cnst, cipher.mod, params.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, cnst, cipher.mod, params.N);
}

Cipher Scheme::multByPoly(Cipher& cipher, ZZX& poly) {
	ZZX axres, bxres;
	Ring2Utils::mult(axres, cipher.ax, poly, cipher.mod, params.N);
	Ring2Utils::mult(bxres, cipher.bx, poly, cipher.mod, params.N);
	return Cipher(axres, bxres, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::multByPolyAndEqual(Cipher& cipher, ZZX& poly) {
	Ring2Utils::multAndEqual(cipher.ax, poly, cipher.mod, params.N);
	Ring2Utils::multAndEqual(cipher.bx, poly, cipher.mod, params.N);
}

Cipher Scheme::multByConstBySlots(Cipher& cipher, CZZ*& cnstvec) {
	Message msg = encode(cnstvec, cipher.slots);

	ZZX axres, bxres;
	Ring2Utils::mult(axres, cipher.ax, msg.mx, cipher.mod, params.N);
	Ring2Utils::mult(bxres, cipher.bx, msg.mx, cipher.mod, params.N);
	return Cipher(axres, bxres, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::multByConstBySlotsAndEqual(Cipher& cipher, CZZ*& cnstvec) {
	Message msg = encode(cnstvec, cipher.slots);

	Ring2Utils::multAndEqual(cipher.ax, msg.mx, cipher.mod, params.N);
	Ring2Utils::multAndEqual(cipher.bx, msg.mx, cipher.mod, params.N);
}

//-----------------------------------------

Cipher Scheme::multByMonomial(Cipher& cipher, const long degree) {
	ZZX ax, bx;

	Ring2Utils::multByMonomial(ax, cipher.ax, degree, params.N);
	Ring2Utils::multByMonomial(bx, cipher.bx, degree, params.N);

	return Cipher(ax, bx, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::multByMonomialAndEqual(Cipher& cipher, const long degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, degree, params.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, degree, params.N);
}

//-----------------------------------------

Cipher Scheme::leftShift(Cipher& cipher, long bits) {
	ZZX ax, bx;

	Ring2Utils::leftShift(ax, cipher.ax, bits, cipher.mod, params.N);
	Ring2Utils::leftShift(bx, cipher.bx, bits, cipher.mod, params.N);

	return Cipher(ax, bx, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::leftShiftAndEqual(Cipher& cipher, long bits) {
	Ring2Utils::leftShiftAndEqual(cipher.ax, bits, cipher.mod, params.N);
	Ring2Utils::leftShiftAndEqual(cipher.bx, bits, cipher.mod, params.N);
}

void Scheme::doubleAndEqual(Cipher& cipher) {
	Ring2Utils::doubleAndEqual(cipher.ax, cipher.mod, params.N);
	Ring2Utils::doubleAndEqual(cipher.bx, cipher.mod, params.N);
}

//-----------------------------------------

Cipher Scheme::modSwitch(Cipher& cipher, long bitsDown) {
	ZZX ax, bx;

	Ring2Utils::rightShift(ax, cipher.ax, bitsDown, params.N);
	Ring2Utils::rightShift(bx, cipher.bx, bitsDown, params.N);

	long newcbits = cipher.cbits - bitsDown;
	ZZ newmod = cipher.mod >> bitsDown;
	return Cipher(ax, bx, newmod, newcbits, cipher.slots);
}

void Scheme::modSwitchAndEqual(Cipher& cipher, long bitsDown) {
	Ring2Utils::rightShiftAndEqual(cipher.ax, bitsDown, params.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, bitsDown, params.N);
	cipher.cbits -= bitsDown;
	cipher.mod >>= bitsDown;
}

Cipher Scheme::modEmbed(Cipher& cipher, long bitsDown) {
	ZZ newmod = cipher.mod >> bitsDown;
	long newcbits = cipher.cbits - bitsDown;
	ZZX bx, ax;
	Ring2Utils::mod(ax, cipher.ax, newmod, params.N);
	Ring2Utils::mod(bx, cipher.bx, newmod, params.N);
	return Cipher(ax, bx, newmod, newcbits, cipher.slots);
}

void Scheme::modEmbedAndEqual(Cipher& cipher, long bitsDown) {
	cipher.mod >>= bitsDown;
	cipher.cbits -= bitsDown;
	Ring2Utils::modAndEqual(cipher.ax, cipher.mod, params.N);
	Ring2Utils::modAndEqual(cipher.bx, cipher.mod, params.N);
}

Cipher Scheme::leftRotateFast(Cipher& cipher, long rotSlots) {
	ZZ Pmod = cipher.mod << params.logq;

	ZZX bxrot, bxres, axres;

	Ring2Utils::inpower(bxrot, cipher.bx, params.rotGroup[rotSlots], params.q, params.N);
	Ring2Utils::inpower(bxres, cipher.ax, params.rotGroup[rotSlots], params.q, params.N);

	RLWE key = publicKey.leftRotKeyMap.at(rotSlots);

	Ring2Utils::mult(axres, bxres, key.ax, Pmod, params.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logq, params.N);

	Ring2Utils::addAndEqual(bxres, bxrot, cipher.mod, params.N);
	return Cipher(axres, bxres, cipher.mod, cipher.cbits, cipher.slots);
}

void Scheme::leftRotateAndEqualFast(Cipher& cipher, long rotSlots) {
	ZZ Pmod = cipher.mod << params.logq;

	ZZX bxrot, bxres, axres;

	Ring2Utils::inpower(bxrot, cipher.bx, params.rotGroup[rotSlots], params.q, params.N);
	Ring2Utils::inpower(bxres, cipher.ax, params.rotGroup[rotSlots], params.q, params.N);

	RLWE key = publicKey.leftRotKeyMap.at(rotSlots);

	Ring2Utils::mult(axres, bxres, key.ax, Pmod, params.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pmod, params.N);

	Ring2Utils::rightShiftAndEqual(axres, params.logq, params.N);
	Ring2Utils::rightShiftAndEqual(bxres, params.logq, params.N);

	Ring2Utils::addAndEqual(bxres, bxrot, cipher.mod, params.N);

	cipher.ax = axres;
	cipher.bx = bxres;
}

Cipher Scheme::leftRotateByPo2(Cipher& cipher, long logrotSlots) {
	long rotSlots = (1 << logrotSlots);
	return leftRotateFast(cipher, rotSlots);
}

void Scheme::leftRotateByPo2AndEqual(Cipher& cipher, long logrotSlots) {
	long rotSlots = (1 << logrotSlots);
	leftRotateAndEqualFast(cipher, rotSlots);
}

Cipher Scheme::rightRotateByPo2(Cipher& cipher, long logrotSlots) {
	long rotSlots = params.N/2 - (1 << logrotSlots);
	return leftRotateFast(cipher, rotSlots);
}

void Scheme::rightRotateByPo2AndEqual(Cipher& cipher, long logrotSlots) {
	long rotSlots = params.N/2 - (1 << logrotSlots);
	leftRotateAndEqualFast(cipher, rotSlots);
}

Cipher Scheme::leftRotate(Cipher& cipher, long rotSlots) {
	Cipher res = cipher;
	leftRotateAndEqual(res, rotSlots);
	return res;
}

void Scheme::leftRotateAndEqual(Cipher& cipher, long rotSlots) {
	long remrotSlots = rotSlots % cipher.slots;
	long logrotSlots = log2((double)remrotSlots) + 1;
	for (long i = 0; i < logrotSlots; ++i) {
		if(bit(remrotSlots, i)) {
			leftRotateByPo2AndEqual(cipher, i);
		}
	}
}

Cipher Scheme::rightRotate(Cipher& cipher, long rotSlots) {
	Cipher res = cipher;
	rightRotateAndEqual(res, rotSlots);
	return res;
}

void Scheme::rightRotateAndEqual(Cipher& cipher, long rotSlots) {
	long remrotSlots = rotSlots % cipher.slots;
	long logrotSlots = log2((double)remrotSlots) + 1;
	for (long i = 0; i < logrotSlots; ++i) {
		if(bit(remrotSlots, i)) {
			rightRotateByPo2AndEqual(cipher, i);
		}
	}
}
