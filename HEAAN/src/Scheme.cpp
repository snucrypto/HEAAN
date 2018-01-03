#include "Scheme.h"

//-----------------------------------------

Scheme::Scheme(SecretKey& secretKey, Context& context) : context(context) {
	addEncKey(secretKey);
	addMultKey(secretKey);
};

void Scheme::addEncKey(SecretKey& secretKey) {
	ZZX ex, ax, bx;

	NumUtils::sampleUniform2(ax, context.N, context.logPQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.PQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.PQ, context.N);

	keyMap.insert(pair<long, Key>(ENCRYPTION, Key(ax, bx)));
}

void Scheme::addMultKey(SecretKey& secretKey) {
	ZZX ex, ax, bx, sxsx;

	Ring2Utils::mult(sxsx, secretKey.sx, secretKey.sx, context.Q, context.N);
	Ring2Utils::leftShiftAndEqual(sxsx, context.logQ, context.PQ, context.N);
	NumUtils::sampleUniform2(ax, context.N, context.logPQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ex, sxsx, context.PQ, context.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.PQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.PQ, context.N);

	keyMap.insert(pair<long, Key>(MULTIPLICATION, Key(ax, bx)));
}

void Scheme::addConjKey(SecretKey& secretKey) {
	ZZX ex, ax, bx, sxconj;

	Ring2Utils::conjugate(sxconj, secretKey.sx, context.N);
	Ring2Utils::leftShiftAndEqual(sxconj, context.logQ, context.PQ, context.N);
	NumUtils::sampleUniform2(ax, context.N, context.logPQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ex, sxconj, context.PQ, context.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.PQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.PQ, context.N);

	keyMap.insert(pair<long, Key>(CONJUGATION, Key(ax, bx)));
}

void Scheme::addLeftRotKey(SecretKey& secretKey, long rot) {
	ZZX ex, sxsx, ax, bx, spow;

	Ring2Utils::inpower(spow, secretKey.sx, context.rotGroup[rot], context.Q, context.N);
	Ring2Utils::leftShiftAndEqual(spow, context.logQ, context.PQ, context.N);
	NumUtils::sampleUniform2(ax, context.N, context.logPQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ex, spow, context.PQ, context.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.PQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.PQ, context.N);
	leftRotKeyMap.insert(pair<long, Key>(rot, Key(ax, bx)));
}

void Scheme::addLeftRotKeys(SecretKey& secretKey) {
	for (long i = 0; i < context.logN - 1; ++i) {
		long idx = 1 << i;
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end()) {
			addLeftRotKey(secretKey, idx);
		}
	}
}

void Scheme::addRightRotKeys(SecretKey& secretKey) {
	for (long i = 0; i < context.logN - 1; ++i) {
		long idx = context.N/2 - (1 << i);
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end()) {
			addLeftRotKey(secretKey, idx);
		}
	}
}

void Scheme::addSortKeys(SecretKey& secretKey, long size) {
	for (long i = 1; i < size; ++i) {
		if(leftRotKeyMap.find(i) == leftRotKeyMap.end()) {
			addLeftRotKey(secretKey, i);
		}
	}
}

Plaintext Scheme::encode(CZZ* vals, long slots, long logq, bool isComplex) {
	long doubleslots = slots << 1;
	ZZ q = power2_ZZ(logq);

	CZZ* gvals = new CZZ[doubleslots];

	for (long i = 0; i < slots; ++i) {
		long idx = (context.rotGroup[i] % (slots << 2) - 1) / 2;
		gvals[idx] = vals[i] << context.logQ;
		gvals[doubleslots - idx - 1] = vals[i].conjugate() << context.logQ;
	}

	ZZX mx;
	mx.SetLength(context.N);
	long idx = 0;
	long gap = context.N / doubleslots;

	NumUtils::fftSpecialInv(gvals, doubleslots, context.ksiPowsr, context.ksiPowsi, context.M);

	for (long i = 0; i < doubleslots; ++i) {
		mx.rep[idx] = gvals[i].r;
		idx += gap;
	}
	delete[] gvals;
	return Plaintext(mx, q, logq, slots, isComplex);
}

CZZ* Scheme::decode(Plaintext& msg) {
	long doubleslots = msg.slots << 1;

	CZZ* fftinv = new CZZ[doubleslots];
	long idx = 0;
	long gap = context.N / doubleslots;
	for (long i = 0; i < doubleslots; ++i) {
		ZZ tmp = msg.mx.rep[idx] % msg.q;
		if(NumBits(tmp) == msg.logq) tmp -= msg.q;
		fftinv[i] = CZZ(tmp, ZZ(0));
		idx += gap;
	}
	NumUtils::fftSpecial(fftinv, doubleslots, context.ksiPowsr, context.ksiPowsi, context.M);

	CZZ* res = new CZZ[msg.slots];

	for (long i = 0; i < msg.slots; ++i) {
		long idx = (context.rotGroup[i] % (msg.slots << 2) - 1) / 2;
		res[i] = fftinv[idx];
	}
	delete[] fftinv;
	return res;
}

Plaintext Scheme::encodeSingle(CZZ& val, long cbits, bool isComplex) {
	ZZX mx;
	mx.SetLength(context.N);
	ZZ mod = power2_ZZ(cbits);
	mx.rep[0] = val.r << context.logQ;
	if(isComplex) {
		mx.rep[context.N / 2] = val.i << context.logQ;
	}
	return Plaintext(mx, mod, cbits, 1, isComplex);
}

CZZ Scheme::decodeSingle(Plaintext& msg) {
	CZZ res;
	ZZ tmp;
	rem(tmp, msg.mx.rep[0], msg.q);
	if(NumBits(tmp) == msg.logq) tmp -= msg.q;
	res.r = tmp;

	if(msg.isComplex) {
		ZZ tmp;
		rem(tmp, msg.mx.rep[context.N / 2], msg.q);
		if(NumBits(tmp) == msg.logq) tmp -= msg.q;
		res.i = tmp;
	}
	return res;
}

Ciphertext Scheme::encryptMsgSK(SecretKey& secretKey, Plaintext& msg) {
	ZZX ax, bx, ex;

	NumUtils::sampleUniform2(ax, context.N, context.logPQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);

	Ring2Utils::mult(bx, secretKey.sx, ax, context.PQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.PQ, context.N);
	Ring2Utils::addAndEqual(bx, msg.mx, context.PQ, context.N);

	Ring2Utils::rightShiftAndEqual(ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bx, context.logQ, context.N);

	return Ciphertext(ax, bx, msg.q, msg.logq, msg.slots, msg.isComplex);
}

Ciphertext Scheme::encryptMsg(Plaintext& msg) {
	ZZ Pq = msg.q << context.logQ;

	ZZX ax, bx, vx, eax, ebx;

	NumUtils::sampleZO(vx, context.N);

	Key key = keyMap.at(ENCRYPTION);

	Ring2Utils::mult(ax, vx, key.ax, Pq, context.N);
	NumUtils::sampleGauss(eax, context.N, context.sigma);
	Ring2Utils::addAndEqual(ax, eax, Pq, context.N);

	Ring2Utils::mult(bx, vx, key.bx, Pq, context.N);
	NumUtils::sampleGauss(ebx, context.N, context.sigma);
	Ring2Utils::addAndEqual(bx, ebx, Pq, context.N);

	Ring2Utils::addAndEqual(bx, msg.mx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bx, context.logQ, context.N);

	return Ciphertext(ax, bx, msg.q, msg.logq, msg.slots, msg.isComplex);
}

Plaintext Scheme::decryptMsg(SecretKey& secretKey, Ciphertext& cipher) {
	ZZX mx;

	Ring2Utils::mult(mx, cipher.ax, secretKey.sx, cipher.q, context.N);
	Ring2Utils::addAndEqual(mx, cipher.bx, cipher.q, context.N);

	return Plaintext(mx, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::encryptSK(SecretKey& secretKey, CZZ* vals, long slots, long logq, bool isComplex) {
	Plaintext msg = encode(vals, slots, logq, isComplex);
	return encryptMsgSK(secretKey, msg);
}

Ciphertext Scheme::encrypt(CZZ* vals, long slots, long logq, bool isComplex) {
	Plaintext msg = encode(vals, slots, logq, isComplex);
	return encryptMsg(msg);
}

CZZ* Scheme::decrypt(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decode(msg);
}

Ciphertext Scheme::encryptSingle(CZZ& val, long logq, bool isComplex) {
	Plaintext msg = encodeSingle(val, logq, isComplex);
	return encryptMsg(msg);
}

CZZ Scheme::decryptSingle(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decodeSingle(msg);
}

//-----------------------------------------

Ciphertext Scheme::add(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZX ax, bx;

	Ring2Utils::add(ax, cipher1.ax, cipher2.ax, cipher1.q, context.N);
	Ring2Utils::add(bx, cipher1.bx, cipher2.bx, cipher1.q, context.N);

	return Ciphertext(ax, bx, cipher1.q, cipher1.logq, cipher1.slots, cipher1.isComplex);
}

void Scheme::addAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	Ring2Utils::addAndEqual(cipher1.ax, cipher2.ax, cipher1.q, context.N);
	Ring2Utils::addAndEqual(cipher1.bx, cipher2.bx, cipher1.q, context.N);
}

//-----------------------------------------

Ciphertext Scheme::addConst(Ciphertext& cipher, ZZ& cnst) {
	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	AddMod(bx.rep[0], cipher.bx.rep[0], cnst, cipher.q);

	return Ciphertext(ax, bx, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::addConstAndEqual(Ciphertext& cipher, ZZ& cnst) {
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnst, cipher.q);
}

//-----------------------------------------

Ciphertext Scheme::sub(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZX ax, bx;

	Ring2Utils::sub(ax, cipher1.ax, cipher2.ax, cipher1.q, context.N);
	Ring2Utils::sub(bx, cipher1.bx, cipher2.bx, cipher1.q, context.N);

	return Ciphertext(ax, bx, cipher1.q, cipher1.logq, cipher1.slots, cipher1.isComplex);
}

void Scheme::subAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	Ring2Utils::subAndEqual(cipher1.ax, cipher2.ax, cipher1.q, context.N);
	Ring2Utils::subAndEqual(cipher1.bx, cipher2.bx, cipher1.q, context.N);
}

void Scheme::subAndEqual2(Ciphertext& cipher1, Ciphertext& cipher2) {
	Ring2Utils::subAndEqual2(cipher1.ax, cipher2.ax, cipher1.q, context.N);
	Ring2Utils::subAndEqual2(cipher1.bx, cipher2.bx, cipher1.q, context.N);
}

Ciphertext Scheme::imult(Ciphertext& cipher) {
	ZZX bxres, axres;

	Ring2Utils::multByMonomial(axres, cipher.ax, context.N / 2, context.N);
	Ring2Utils::multByMonomial(bxres, cipher.bx, context.N / 2, context.N);

	return Ciphertext(axres, bxres, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::imultAndEqual(Ciphertext& cipher) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, context.N / 2, context.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, context.N / 2, context.N);
}

Ciphertext Scheme::mult(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ Pq = cipher1.q << context.logQ;

	ZZX axbx1 = Ring2Utils::add(cipher1.ax, cipher1.bx, cipher1.q, context.N);
	ZZX axbx2 = Ring2Utils::add(cipher2.ax, cipher2.bx, cipher1.q, context.N);

	Ring2Utils::multAndEqual(axbx1, axbx2, cipher1.q, context.N);

	ZZX bxbx = Ring2Utils::mult(cipher1.bx, cipher2.bx, cipher1.q, context.N);
	ZZX axax = Ring2Utils::mult(cipher1.ax, cipher2.ax, cipher1.q, context.N);

	Key key = keyMap.at(MULTIPLICATION);

	ZZX axmult = Ring2Utils::mult(axax, key.ax, Pq, context.N);
	ZZX bxmult = Ring2Utils::mult(axax, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axmult, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxmult, context.logQ, context.N);

	Ring2Utils::addAndEqual(axmult, axbx1, cipher1.q, context.N);
	Ring2Utils::subAndEqual(axmult, bxbx, cipher1.q, context.N);
	Ring2Utils::subAndEqual(axmult, axax, cipher1.q, context.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, cipher1.q, context.N);

	return Ciphertext(axmult, bxmult, cipher1.q, cipher1.logq, cipher1.slots, cipher1.isComplex);
}

void Scheme::multAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ Pq = cipher1.q << context.logQ;

	ZZX axbx1 = Ring2Utils::add(cipher1.ax, cipher1.bx, cipher1.q, context.N);
	ZZX axbx2 = Ring2Utils::add(cipher2.ax, cipher2.bx, cipher1.q, context.N);

	Ring2Utils::multAndEqual(axbx1, axbx2, cipher1.q, context.N);

	ZZX bxbx = Ring2Utils::mult(cipher1.bx, cipher2.bx, cipher1.q, context.N);
	ZZX axax = Ring2Utils::mult(cipher1.ax, cipher2.ax, cipher1.q, context.N);

	Key key = keyMap.at(MULTIPLICATION);

	cipher1.ax = Ring2Utils::mult(axax, key.ax, Pq, context.N);
	cipher1.bx = Ring2Utils::mult(axax, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(cipher1.ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(cipher1.bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(cipher1.ax, axbx1, cipher1.q, context.N);
	Ring2Utils::subAndEqual(cipher1.ax, bxbx, cipher1.q, context.N);
	Ring2Utils::subAndEqual(cipher1.ax, axax, cipher1.q, context.N);
	Ring2Utils::addAndEqual(cipher1.bx, bxbx, cipher1.q, context.N);
}

//-----------------------------------------

Ciphertext Scheme::square(Ciphertext& cipher) {
	ZZ Pq = cipher.q << context.logQ;

	ZZX axax, axbx, bxbx, bxmult, axmult;

	Ring2Utils::square(bxbx, cipher.bx, cipher.q, context.N);
	Ring2Utils::mult(axbx, cipher.ax, cipher.bx, cipher.q, context.N);
	Ring2Utils::addAndEqual(axbx, axbx, cipher.q, context.N);
	Ring2Utils::square(axax, cipher.ax, cipher.q, context.N);

	Key key = keyMap.at(MULTIPLICATION);

	Ring2Utils::mult(axmult, axax, key.ax, Pq, context.N);
	Ring2Utils::mult(bxmult, axax, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axmult, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxmult, context.logQ, context.N);

	Ring2Utils::addAndEqual(axmult, axbx, cipher.q, context.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, cipher.q, context.N);

	return Ciphertext(axmult, bxmult, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::squareAndEqual(Ciphertext& cipher) {
	ZZ Pq = cipher.q << context.logQ;

	ZZX bxbx, axbx, axax, bxmult, axmult;

	Ring2Utils::square(bxbx, cipher.bx, cipher.q, context.N);
	Ring2Utils::mult(axbx, cipher.bx, cipher.ax, cipher.q, context.N);
	Ring2Utils::addAndEqual(axbx, axbx, cipher.q, context.N);
	Ring2Utils::square(axax, cipher.ax, cipher.q, context.N);

	Key key = keyMap.at(MULTIPLICATION);

	Ring2Utils::mult(axmult, axax, key.ax, Pq, context.N);
	Ring2Utils::mult(bxmult, axax, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axmult, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxmult, context.logQ, context.N);

	Ring2Utils::addAndEqual(axmult, axbx, cipher.q, context.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, cipher.q, context.N);

	cipher.bx = bxmult;
	cipher.ax = axmult;
}

//-----------------------------------------

Ciphertext Scheme::multByConst(Ciphertext& cipher, ZZ& cnst) {
	ZZX ax, bx;

	Ring2Utils::multByConst(ax, cipher.ax, cnst, cipher.q, context.N);
	Ring2Utils::multByConst(bx, cipher.bx, cnst, cipher.q, context.N);

	return Ciphertext(ax, bx, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, ZZ& cnst) {
	Ring2Utils::multByConstAndEqual(cipher.ax, cnst, cipher.q, context.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, cnst, cipher.q, context.N);
}

Ciphertext Scheme::multByPoly(Ciphertext& cipher, ZZX& poly) {
	ZZX axres, bxres;

	Ring2Utils::mult(axres, cipher.ax, poly, cipher.q, context.N);
	Ring2Utils::mult(bxres, cipher.bx, poly, cipher.q, context.N);

	return Ciphertext(axres, bxres, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::multByPolyAndEqual(Ciphertext& cipher, ZZX& poly) {
	Ring2Utils::multAndEqual(cipher.ax, poly, cipher.q, context.N);
	Ring2Utils::multAndEqual(cipher.bx, poly, cipher.q, context.N);
}

//-----------------------------------------

Ciphertext Scheme::multByMonomial(Ciphertext& cipher, const long degree) {
	ZZX ax, bx;

	Ring2Utils::multByMonomial(ax, cipher.ax, degree, context.N);
	Ring2Utils::multByMonomial(bx, cipher.bx, degree, context.N);

	return Ciphertext(ax, bx, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::multByMonomialAndEqual(Ciphertext& cipher, const long degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, degree, context.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, degree, context.N);
}

//-----------------------------------------

Ciphertext Scheme::leftShift(Ciphertext& cipher, long bits) {
	ZZX ax, bx;

	Ring2Utils::leftShift(ax, cipher.ax, bits, cipher.q, context.N);
	Ring2Utils::leftShift(bx, cipher.bx, bits, cipher.q, context.N);

	return Ciphertext(ax, bx, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::leftShiftAndEqual(Ciphertext& cipher, long bits) {
	Ring2Utils::leftShiftAndEqual(cipher.ax, bits, cipher.q, context.N);
	Ring2Utils::leftShiftAndEqual(cipher.bx, bits, cipher.q, context.N);
}

void Scheme::doubleAndEqual(Ciphertext& cipher) {
	Ring2Utils::doubleAndEqual(cipher.ax, cipher.q, context.N);
	Ring2Utils::doubleAndEqual(cipher.bx, cipher.q, context.N);
}

//-----------------------------------------

Ciphertext Scheme::reScaleBy(Ciphertext& cipher, long bitsDown) {
	ZZX ax, bx;
	long newcbits = cipher.logq - bitsDown;
	ZZ newmod = cipher.q >> bitsDown;

	Ring2Utils::rightShift(ax, cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShift(bx, cipher.bx, bitsDown, context.N);

	return Ciphertext(ax, bx, newmod, newcbits, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::reScaleTo(Ciphertext& cipher, long newlogq) {
	ZZX ax, bx;
	long bitsDown = cipher.logq - newlogq;
	ZZ newmod = power2_ZZ(newlogq);

	Ring2Utils::rightShift(ax, cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShift(bx, cipher.bx, bitsDown, context.N);

	return Ciphertext(ax, bx, newmod, newlogq, cipher.slots, cipher.isComplex);
}

void Scheme::reScaleByAndEqual(Ciphertext& cipher, long bitsDown) {
	Ring2Utils::rightShiftAndEqual(cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, bitsDown, context.N);

	cipher.logq -= bitsDown;
	cipher.q >>= bitsDown;
}

void Scheme::reScaleToAndEqual(Ciphertext& cipher, long newlogq) {
	long bitsDown = cipher.logq - newlogq;

	Ring2Utils::rightShiftAndEqual(cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, bitsDown, context.N);

	cipher.logq = newlogq;
	cipher.q = power2_ZZ(newlogq);
}

Ciphertext Scheme::modDownBy(Ciphertext& cipher, long bitsDown) {
	ZZX bx, ax;
	ZZ newmod = cipher.q >> bitsDown;
	long newcbits = cipher.logq - bitsDown;

	Ring2Utils::mod(ax, cipher.ax, newmod, context.N);
	Ring2Utils::mod(bx, cipher.bx, newmod, context.N);

	return Ciphertext(ax, bx, newmod, newcbits, cipher.slots, cipher.isComplex);
}

void Scheme::modDownByAndEqual(Ciphertext& cipher, long bitsDown) {
	cipher.q >>= bitsDown;
	cipher.logq -= bitsDown;

	Ring2Utils::modAndEqual(cipher.ax, cipher.q, context.N);
	Ring2Utils::modAndEqual(cipher.bx, cipher.q, context.N);
}

Ciphertext Scheme::modDownTo(Ciphertext& cipher, long newcbits) {
	ZZX bx, ax;
	ZZ newmod = power2_ZZ(newcbits);

	Ring2Utils::mod(ax, cipher.ax, newmod, context.N);
	Ring2Utils::mod(bx, cipher.bx, newmod, context.N);

	return Ciphertext(ax, bx, newmod, newcbits, cipher.slots);
}

void Scheme::modDownToAndEqual(Ciphertext& cipher, long newcbits) {
	if(cipher.logq < newcbits) {
		invalid_argument("cbits of cipher should be larger than newcbits");
	}
	if(cipher.logq == newcbits) {
		return;
	}
	cipher.q = power2_ZZ(newcbits);
	cipher.logq = newcbits;

	Ring2Utils::modAndEqual(cipher.ax, cipher.q, context.N);
	Ring2Utils::modAndEqual(cipher.bx, cipher.q, context.N);
}

Ciphertext Scheme::leftRotateFast(Ciphertext& cipher, long rotSlots) {
	ZZ Pq = cipher.q << context.logQ;

	ZZX bxrot, bxres, axres;

	Ring2Utils::inpower(bxrot, cipher.bx, context.rotGroup[rotSlots], context.Q, context.N);
	Ring2Utils::inpower(bxres, cipher.ax, context.rotGroup[rotSlots], context.Q, context.N);

	Key key = leftRotKeyMap.at(rotSlots);

	Ring2Utils::mult(axres, bxres, key.ax, Pq, context.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axres, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxres, context.logQ, context.N);

	Ring2Utils::addAndEqual(bxres, bxrot, cipher.q, context.N);

	return Ciphertext(axres, bxres, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::leftRotateAndEqualFast(Ciphertext& cipher, long rotSlots) {
	ZZ Pq = cipher.q << context.logQ;

	ZZX bxrot, bxres, axres;

	Ring2Utils::inpower(bxrot, cipher.bx, context.rotGroup[rotSlots], context.Q, context.N);
	Ring2Utils::inpower(bxres, cipher.ax, context.rotGroup[rotSlots], context.Q, context.N);

	Key key = leftRotKeyMap.at(rotSlots);

	Ring2Utils::mult(axres, bxres, key.ax, Pq, context.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axres, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxres, context.logQ, context.N);

	Ring2Utils::addAndEqual(bxres, bxrot, cipher.q, context.N);

	cipher.ax = axres;
	cipher.bx = bxres;
}

Ciphertext Scheme::leftRotateByPo2(Ciphertext& cipher, long logrotSlots) {
	long rotSlots = (1 << logrotSlots);
	return leftRotateFast(cipher, rotSlots);
}

void Scheme::leftRotateByPo2AndEqual(Ciphertext& cipher, long logrotSlots) {
	long rotSlots = (1 << logrotSlots);
	leftRotateAndEqualFast(cipher, rotSlots);
}

Ciphertext Scheme::rightRotateByPo2(Ciphertext& cipher, long logrotSlots) {
	long rotSlots = context.N/2 - (1 << logrotSlots);
	return leftRotateFast(cipher, rotSlots);
}

void Scheme::rightRotateByPo2AndEqual(Ciphertext& cipher, long logrotSlots) {
	long rotSlots = context.N/2 - (1 << logrotSlots);
	leftRotateAndEqualFast(cipher, rotSlots);
}

Ciphertext Scheme::leftRotate(Ciphertext& cipher, long rotSlots) {
	Ciphertext res = cipher;
	leftRotateAndEqual(res, rotSlots);
	return res;
}

void Scheme::leftRotateAndEqual(Ciphertext& cipher, long rotSlots) {
	long remrotSlots = rotSlots % cipher.slots;
	long logrotSlots = log2((double)remrotSlots) + 1;
	for (long i = 0; i < logrotSlots; ++i) {
		if(bit(remrotSlots, i)) {
			leftRotateByPo2AndEqual(cipher, i);
		}
	}
}

Ciphertext Scheme::rightRotate(Ciphertext& cipher, long rotSlots) {
	Ciphertext res = cipher;
	rightRotateAndEqual(res, rotSlots);
	return res;
}

void Scheme::rightRotateAndEqual(Ciphertext& cipher, long rotSlots) {
	long remrotSlots = rotSlots % cipher.slots;
	long logrotSlots = log2((double)remrotSlots) + 1;
	for (long i = 0; i < logrotSlots; ++i) {
		if(bit(remrotSlots, i)) {
			rightRotateByPo2AndEqual(cipher, i);
		}
	}
}

Ciphertext Scheme::conjugate(Ciphertext& cipher) {
	ZZ Pq = cipher.q << context.logQ;

	ZZX bxconj, bxres, axres;

	Ring2Utils::conjugate(bxconj, cipher.bx, context.N);
	Ring2Utils::conjugate(bxres, cipher.ax, context.N);

	Key key = keyMap.at(CONJUGATION);

	Ring2Utils::mult(axres, bxres, key.ax, Pq, context.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axres, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxres, context.logQ, context.N);

	Ring2Utils::addAndEqual(bxres, bxconj, cipher.q, context.N);

	return Ciphertext(axres, bxres, cipher.q, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::conjugateAndEqual(Ciphertext& cipher) {
	ZZ Pq = cipher.q << context.logQ;

	ZZX bxconj, bxres, axres;

	Ring2Utils::conjugate(bxconj, cipher.bx, context.N);
	Ring2Utils::conjugate(bxres, cipher.ax, context.N);

	Key key = keyMap.at(CONJUGATION);

	Ring2Utils::mult(axres, bxres, key.ax, Pq, context.N);
	Ring2Utils::multAndEqual(bxres, key.bx, Pq, context.N);

	Ring2Utils::rightShiftAndEqual(axres, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxres, context.logQ, context.N);

	Ring2Utils::addAndEqual(bxres, bxconj, cipher.q, context.N);

	cipher.ax = axres;
	cipher.bx = bxres;
}
