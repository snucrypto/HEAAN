/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Scheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Ring2Utils.h"
#include "StringUtils.h"

//-----------------------------------------

Scheme::Scheme(Context& context) : context(context) {
}

Scheme::Scheme(SecretKey& secretKey, Context& context) : context(context) {
	addEncKey(secretKey);
	addMultKey(secretKey);
};

//----------------------------------------------------------------------------------
//   KEYS GENERATION
//----------------------------------------------------------------------------------


void Scheme::addEncKey(SecretKey& secretKey) {
	ZZX ex, ax, bx;

	NumUtils::sampleUniform2(ax, context.N, context.logQQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.QQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.QQ, context.N);

	keyMap.insert(pair<long, Key>(ENCRYPTION, Key(ax, bx)));
}

void Scheme::addMultKey(SecretKey& secretKey) {
	ZZX ex, ax, bx, sxsx;

	Ring2Utils::mult(sxsx, secretKey.sx, secretKey.sx, context.Q, context.N);
	Ring2Utils::leftShiftAndEqual(sxsx, context.logQ, context.QQ, context.N);
	NumUtils::sampleUniform2(ax, context.N, context.logQQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ex, sxsx, context.QQ, context.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.QQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.QQ, context.N);

	keyMap.insert(pair<long, Key>(MULTIPLICATION, Key(ax, bx)));
}

void Scheme::addConjKey(SecretKey& secretKey) {
	ZZX ex, ax, bx, sxconj;

	Ring2Utils::conjugate(sxconj, secretKey.sx, context.N);
	Ring2Utils::leftShiftAndEqual(sxconj, context.logQ, context.QQ, context.N);
	NumUtils::sampleUniform2(ax, context.N, context.logQQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ex, sxconj, context.QQ, context.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.QQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.QQ, context.N);

	keyMap.insert(pair<long, Key>(CONJUGATION, Key(ax, bx)));
}

void Scheme::addLeftRotKey(SecretKey& secretKey, long rot) {
	ZZX ex, ax, bx, sxrot;

	Ring2Utils::inpower(sxrot, secretKey.sx, context.rotGroup[rot], context.Q, context.N);
	Ring2Utils::leftShiftAndEqual(sxrot, context.logQ, context.QQ, context.N);
	NumUtils::sampleUniform2(ax, context.N, context.logQQ);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ex, sxrot, context.QQ, context.N);
	Ring2Utils::mult(bx, secretKey.sx, ax, context.QQ, context.N);
	Ring2Utils::sub(bx, ex, bx, context.QQ, context.N);

	leftRotKeyMap.insert(pair<long, Key>(rot, Key(ax, bx)));
}

void Scheme::addLeftRotKeys(SecretKey& secretKey) {
	for (long i = 0; i < context.logNh; ++i) {
		long idx = 1 << i;
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end()) {
			addLeftRotKey(secretKey, idx);
		}
	}
}

void Scheme::addRightRotKeys(SecretKey& secretKey) {
	for (long i = 0; i < context.logNh; ++i) {
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


//----------------------------------------------------------------------------------
//   ENCODING & DECODING
//----------------------------------------------------------------------------------


Plaintext Scheme::encode(double* vals, long slots, long logp, long logq) {
	ZZX mx = context.encode(vals, slots, logp + context.logQ);
	return Plaintext(mx, logp, logq, slots, false);
}

Plaintext Scheme::encode(complex<double>* vals, long slots, long logp, long logq) {
	ZZX mx = context.encode(vals, slots, logp + context.logQ);
	return Plaintext(mx, logp, logq, slots, true);
}

complex<double>* Scheme::decode(Plaintext& msg) {
	return context.decode(msg.mx, msg.slots, msg.logp, msg.logq);
}

Plaintext Scheme::encodeSingle(complex<double> val, long logp, long logq) {
	ZZX mx = context.encodeSingle(val, logp + context.logQ);
	return Plaintext(mx, logp, logq, 1, true);
}

Plaintext Scheme::encodeSingle(double val, long logp, long logq) {
	ZZX mx = context.encodeSingle(val, logp + context.logQ);
	return Plaintext(mx, logp, logq, 1, false);
}

complex<double> Scheme::decodeSingle(Plaintext& msg) {
	return context.decodeSingle(msg.mx, msg.logp, msg.logq, msg.isComplex);
}


//----------------------------------------------------------------------------------
//   ENCRYPTION & DECRYPTION
//----------------------------------------------------------------------------------


Ciphertext Scheme::encryptMsg(Plaintext& msg) {
	ZZX ax, bx, vx, ex;
	Key key = keyMap.at(ENCRYPTION);
	ZZ qQ = context.qpowvec[msg.logq + context.logQ];

	NumUtils::sampleZO(vx, context.N);
	Ring2Utils::mult(ax, vx, key.ax, qQ, context.N);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(ax, ex, qQ, context.N);

	Ring2Utils::mult(bx, vx, key.bx, qQ, context.N);
	NumUtils::sampleGauss(ex, context.N, context.sigma);
	Ring2Utils::addAndEqual(bx, ex, qQ, context.N);

	Ring2Utils::addAndEqual(bx, msg.mx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bx, context.logQ, context.N);

	return Ciphertext(ax, bx, msg.logp, msg.logq, msg.slots, msg.isComplex);
}

Plaintext Scheme::decryptMsg(SecretKey& secretKey, Ciphertext& cipher) {
	ZZ q = context.qpowvec[cipher.logq];

	ZZX mx;
	Ring2Utils::mult(mx, cipher.ax, secretKey.sx, q, context.N);
	Ring2Utils::addAndEqual(mx, cipher.bx, q, context.N);

	return Plaintext(mx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::encrypt(double* vals, long slots, long logp, long logq) {
	Plaintext msg = encode(vals, slots, logp, logq);
	return encryptMsg(msg);
}

Ciphertext Scheme::encrypt(complex<double>* vals, long slots, long logp, long logq) {
	Plaintext msg = encode(vals, slots, logp, logq);
	return encryptMsg(msg);
}

Ciphertext Scheme::encryptZeros(long slots, long logp, long logq) {
	Ciphertext czeros = encryptSingle(0.0, logp, logq);
	czeros.isComplex = true;
	czeros.slots = slots;
	return czeros;
}
complex<double>* Scheme::decrypt(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decode(msg);
}

Ciphertext Scheme::encryptSingle(double val, long logp, long logq) {
	Plaintext msg = encodeSingle(val, logp,  logq);
	return encryptMsg(msg);
}

Ciphertext Scheme::encryptSingle(complex<double> val, long logp, long logq) {
	Plaintext msg = encodeSingle(val, logp,  logq);
	return encryptMsg(msg);
}

complex<double> Scheme::decryptSingle(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decodeSingle(msg);
}


//----------------------------------------------------------------------------------
//   HOMOMORPHIC OPERATIONS
//----------------------------------------------------------------------------------


Ciphertext Scheme::negate(Ciphertext& cipher) {
	return Ciphertext(-cipher.ax, -cipher.bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::negateAndEqual(Ciphertext& cipher) {
	cipher.ax = -cipher.ax;
	cipher.bx = -cipher.bx;
}

Ciphertext Scheme::add(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];
	ZZX ax, bx;

	Ring2Utils::add(ax, cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::add(bx, cipher1.bx, cipher2.bx, q, context.N);

	return Ciphertext(ax, bx, cipher1.logp, cipher1.logq, cipher1.slots, cipher1.isComplex);
}

void Scheme::addAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];

	Ring2Utils::addAndEqual(cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::addAndEqual(cipher1.bx, cipher2.bx, q, context.N);
}

Ciphertext Scheme::addConst(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];

	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);

	AddMod(bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::addConst(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];

	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);

	AddMod(bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::addConst(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	ZZ cnstrZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.real(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);
	ZZ cnstiZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.imag(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.imag(), logp);

	AddMod(bx.rep[0], cipher.bx.rep[0], cnstrZZ, q);
	AddMod(bx.rep[context.Nh], cipher.bx.rep[context.Nh], cnstiZZ, q);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::addConstAndEqual(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
}

void Scheme::addConstAndEqual(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
}

void Scheme::addConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];

	ZZ cnstrZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.real(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);
	ZZ cnstiZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.imag(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.imag(), logp);

	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnstrZZ, q);
	AddMod(cipher.bx.rep[context.Nh], cipher.bx.rep[context.Nh], cnstiZZ, q);
}

//-----------------------------------------

Ciphertext Scheme::sub(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];
	ZZX ax, bx;

	Ring2Utils::sub(ax, cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::sub(bx, cipher1.bx, cipher2.bx, q, context.N);

	return Ciphertext(ax, bx, cipher1.logp, cipher1.logq, cipher1.slots, cipher1.isComplex);
}

void Scheme::subAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];

	Ring2Utils::subAndEqual(cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::subAndEqual(cipher1.bx, cipher2.bx, q, context.N);
}

void Scheme::subAndEqual2(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];

	Ring2Utils::subAndEqual2(cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::subAndEqual2(cipher1.bx, cipher2.bx, q, context.N);
}

Ciphertext Scheme::imult(Ciphertext& cipher) {
	ZZX ax, bx;

	Ring2Utils::multByMonomial(ax, cipher.ax, context.Nh, context.N);
	Ring2Utils::multByMonomial(bx, cipher.bx, context.Nh, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::idiv(Ciphertext& cipher) {
	ZZX ax, bx;

	Ring2Utils::multByMonomial(ax, cipher.ax, 3 * context.Nh, context.N);
	Ring2Utils::multByMonomial(bx, cipher.bx, 3 * context.Nh, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::imultAndEqual(Ciphertext& cipher) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, context.Nh, context.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, context.Nh, context.N);
}

void Scheme::idivAndEqual(Ciphertext& cipher) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, 3 * context.Nh, context.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, 3 * context.Nh, context.N);
}

Ciphertext Scheme::mult(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];
	ZZ qQ = context.qpowvec[cipher1.logq + context.logQ];

	ZZX axbx1, axbx2, axax, bxbx, axmult, bxmult;
	Key key = keyMap.at(MULTIPLICATION);

	Ring2Utils::add(axbx1, cipher1.ax, cipher1.bx, q, context.N);
	Ring2Utils::add(axbx2, cipher2.ax, cipher2.bx, q, context.N);
	Ring2Utils::multAndEqual(axbx1, axbx2, q, context.N);

	Ring2Utils::mult(axax, cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::mult(bxbx, cipher1.bx, cipher2.bx, q, context.N);

	Ring2Utils::mult(axmult, axax, key.ax, qQ, context.N);
	Ring2Utils::mult(bxmult, axax, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(axmult, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxmult, context.logQ, context.N);

	Ring2Utils::addAndEqual(axmult, axbx1, q, context.N);
	Ring2Utils::subAndEqual(axmult, bxbx, q, context.N);
	Ring2Utils::subAndEqual(axmult, axax, q, context.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, q, context.N);

	return Ciphertext(axmult, bxmult, cipher1.logp + cipher2.logp, cipher1.logq, cipher1.slots, cipher1.isComplex);
}

void Scheme::multAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = context.qpowvec[cipher1.logq];
	ZZ qQ = context.qpowvec[cipher1.logq + context.logQ];
	ZZX axbx1, axbx2, axax, bxbx;
	Key key = keyMap.at(MULTIPLICATION);

	Ring2Utils::add(axbx1, cipher1.ax, cipher1.bx, q, context.N);
	Ring2Utils::add(axbx2, cipher2.ax, cipher2.bx, q, context.N);
	Ring2Utils::multAndEqual(axbx1, axbx2, q, context.N);

	Ring2Utils::mult(axax, cipher1.ax, cipher2.ax, q, context.N);
	Ring2Utils::mult(bxbx, cipher1.bx, cipher2.bx, q, context.N);

	Ring2Utils::mult(cipher1.ax, axax, key.ax, qQ, context.N);
	Ring2Utils::mult(cipher1.bx, axax, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(cipher1.ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(cipher1.bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(cipher1.ax, axbx1, q, context.N);
	Ring2Utils::subAndEqual(cipher1.ax, bxbx, q, context.N);
	Ring2Utils::subAndEqual(cipher1.ax, axax, q, context.N);
	Ring2Utils::addAndEqual(cipher1.bx, bxbx, q, context.N);

	cipher1.logp += cipher2.logp;
}

Ciphertext Scheme::square(Ciphertext& cipher) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ qQ = context.qpowvec[cipher.logq + context.logQ];
	ZZX axax, axbx, bxbx, bxmult, axmult;
	Key key = keyMap.at(MULTIPLICATION);

	Ring2Utils::square(bxbx, cipher.bx, q, context.N);
	Ring2Utils::mult(axbx, cipher.ax, cipher.bx, q, context.N);
	Ring2Utils::addAndEqual(axbx, axbx, q, context.N);
	Ring2Utils::square(axax, cipher.ax, q, context.N);

	Ring2Utils::mult(axmult, axax, key.ax, qQ, context.N);
	Ring2Utils::mult(bxmult, axax, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(axmult, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bxmult, context.logQ, context.N);

	Ring2Utils::addAndEqual(axmult, axbx, q, context.N);
	Ring2Utils::addAndEqual(bxmult, bxbx, q, context.N);

	return Ciphertext(axmult, bxmult, cipher.logp * 2, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::squareAndEqual(Ciphertext& cipher) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ qQ = context.qpowvec[cipher.logq + context.logQ];
	ZZX bxbx, axbx, axax;
	Key key = keyMap.at(MULTIPLICATION);

	Ring2Utils::square(bxbx, cipher.bx, q, context.N);
	Ring2Utils::mult(axbx, cipher.bx, cipher.ax, q, context.N);
	Ring2Utils::addAndEqual(axbx, axbx, q, context.N);
	Ring2Utils::square(axax, cipher.ax, q, context.N);

	Ring2Utils::mult(cipher.ax, axax, key.ax, qQ, context.N);
	Ring2Utils::mult(cipher.bx, axax, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(cipher.ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(cipher.ax, axbx, q, context.N);
	Ring2Utils::addAndEqual(cipher.bx, bxbx, q, context.N);
	cipher.logp *= 2;
}

Ciphertext Scheme::multByConst(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZX ax, bx;

	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring2Utils::multByConst(ax, cipher.ax, cnstZZ, q, context.N);
	Ring2Utils::multByConst(bx, cipher.bx, cnstZZ, q, context.N);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::multByConst(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZX ax, bx;

	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring2Utils::multByConst(ax, cipher.ax, cnstZZ, q, context.N);
	Ring2Utils::multByConst(bx, cipher.bx, cnstZZ, q, context.N);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::multByConst(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];

	ZZX axr, bxr, axi, bxi;

	ZZ cnstrZZ = EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);
	ZZ cnstiZZ = EvaluatorUtils::scaleUpToZZ(cnst.imag(), logp);

	Ring2Utils::multByMonomial(axi, cipher.ax, context.Nh, context.N);
	Ring2Utils::multByMonomial(bxi, cipher.bx, context.Nh, context.N);

	Ring2Utils::multByConst(axr, cipher.ax, cnstrZZ, q, context.N);
	Ring2Utils::multByConst(bxr, cipher.bx, cnstrZZ, q, context.N);

	Ring2Utils::multByConstAndEqual(axi, cnstiZZ, q, context.N);
	Ring2Utils::multByConstAndEqual(bxi, cnstiZZ, q, context.N);

	Ring2Utils::addAndEqual(axr, axi, q, context.N);
	Ring2Utils::addAndEqual(bxr, bxi, q, context.N);

	return Ciphertext(axr, bxr, cipher.logp + logp, cipher.logq, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::multByConstVec(Ciphertext& cipher, complex<double>* cnstVec, long slots, long logp) {
	ZZX cmx = context.encode(cnstVec, slots, logp);
	return multByPoly(cipher, cmx, logp);
}

Ciphertext Scheme::multByConstVec(Ciphertext& cipher, double* cnstVec, long slots, long logp) {
	ZZX cmx = context.encode(cnstVec, slots, logp);
	return multByPoly(cipher, cmx, logp);
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring2Utils::multByConstAndEqual(cipher.ax, cnstZZ, q, context.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, cnstZZ, q, context.N);
	cipher.logp += logp;
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring2Utils::multByConstAndEqual(cipher.ax, cnstZZ, q, context.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, cnstZZ, q, context.N);
	cipher.logp += logp;
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZX axi, bxi;

	ZZ cnstrZZ = EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);
	ZZ cnstiZZ = EvaluatorUtils::scaleUpToZZ(cnst.imag(), logp);

	Ring2Utils::multByMonomial(axi, cipher.ax, context.Nh, context.N);
	Ring2Utils::multByMonomial(bxi, cipher.bx, context.Nh, context.N);

	Ring2Utils::multByConstAndEqual(cipher.ax, cnstrZZ, q, context.N);
	Ring2Utils::multByConstAndEqual(cipher.bx, cnstrZZ, q, context.N);

	Ring2Utils::multByConstAndEqual(axi, cnstiZZ, q, context.N);
	Ring2Utils::multByConstAndEqual(bxi, cnstiZZ, q, context.N);

	Ring2Utils::addAndEqual(cipher.ax, axi, q, context.N);
	Ring2Utils::addAndEqual(cipher.bx, bxi, q, context.N);
	cipher.logp += logp;
}

void Scheme::multByConstVecAndEqual(Ciphertext& cipher, complex<double>* cnstVec, long slots, long logp) {
	ZZX cmx = context.encode(cnstVec, slots, logp);
	multByPolyAndEqual(cipher, cmx, logp);
}

void Scheme::multByConstVecAndEqual(Ciphertext& cipher, double* cnstVec, long slots, long logp) {
	ZZX cmx = context.encode(cnstVec, slots, logp);
	multByPolyAndEqual(cipher, cmx, logp);
}

Ciphertext Scheme::multByPoly(Ciphertext& cipher, ZZX& poly, long logp) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZX ax, bx;

	Ring2Utils::mult(ax, cipher.ax, poly, q, context.N);
	Ring2Utils::mult(bx, cipher.bx, poly, q, context.N);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::multByPolyAndEqual(Ciphertext& cipher, ZZX& poly, long logp) {
	ZZ q = context.qpowvec[cipher.logq];

	Ring2Utils::multAndEqual(cipher.ax, poly, q, context.N);
	Ring2Utils::multAndEqual(cipher.bx, poly, q, context.N);
	cipher.logp += logp;
}

Ciphertext Scheme::multByMonomial(Ciphertext& cipher, const long degree) {
	ZZX ax, bx;

	Ring2Utils::multByMonomial(ax, cipher.ax, degree, context.N);
	Ring2Utils::multByMonomial(bx, cipher.bx, degree, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::multByMonomialAndEqual(Ciphertext& cipher, const long degree) {
	Ring2Utils::multByMonomialAndEqual(cipher.ax, degree, context.N);
	Ring2Utils::multByMonomialAndEqual(cipher.bx, degree, context.N);
}

Ciphertext Scheme::multByPo2(Ciphertext& cipher, long deg) {
	ZZ q = context.qpowvec[cipher.logq];

	ZZX ax, bx;

	Ring2Utils::leftShift(ax, cipher.ax, deg, q, context.N);
	Ring2Utils::leftShift(bx, cipher.bx, deg, q, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::multByPo2AndEqual(Ciphertext& cipher, long deg) {
	ZZ q = context.qpowvec[cipher.logq];

	Ring2Utils::leftShiftAndEqual(cipher.ax, deg, q, context.N);
	Ring2Utils::leftShiftAndEqual(cipher.bx, deg, q, context.N);
}

void Scheme::multBy2AndEqual(Ciphertext& cipher) {
	ZZ q = context.qpowvec[cipher.logq];

	Ring2Utils::doubleAndEqual(cipher.ax, q, context.N);
	Ring2Utils::doubleAndEqual(cipher.bx, q, context.N);
}

Ciphertext Scheme::divByPo2(Ciphertext& cipher, long degree) {
	ZZX ax, bx;

	Ring2Utils::rightShift(ax, cipher.ax, degree, context.N);
	Ring2Utils::rightShift(bx, cipher.bx, degree, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq - degree, cipher.slots, cipher.isComplex);
}

void Scheme::divByPo2AndEqual(Ciphertext& cipher, long degree) {
	Ring2Utils::rightShiftAndEqual(cipher.ax, degree, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, degree, context.N);

	cipher.logq -= degree;
}


//----------------------------------------------------------------------------------
//   RESCALING & MODULUS DOWN
//----------------------------------------------------------------------------------


Ciphertext Scheme::reScaleBy(Ciphertext& cipher, long bitsDown) {
	ZZX ax, bx;

	Ring2Utils::rightShift(ax, cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShift(bx, cipher.bx, bitsDown, context.N);

	return Ciphertext(ax, bx, cipher.logp - bitsDown, cipher.logq - bitsDown, cipher.slots, cipher.isComplex);
}

Ciphertext Scheme::reScaleTo(Ciphertext& cipher, long newlogq) {
	ZZX ax, bx;
	long bitsDown = cipher.logq - newlogq;

	Ring2Utils::rightShift(ax, cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShift(bx, cipher.bx, bitsDown, context.N);

	return Ciphertext(ax, bx, cipher.logp - bitsDown, newlogq, cipher.slots, cipher.isComplex);
}

void Scheme::reScaleByAndEqual(Ciphertext& cipher, long bitsDown) {
	Ring2Utils::rightShiftAndEqual(cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, bitsDown, context.N);

	cipher.logq -= bitsDown;
	cipher.logp -= bitsDown;
}

void Scheme::reScaleToAndEqual(Ciphertext& cipher, long logq) {
	long bitsDown = cipher.logq - logq;
	cipher.logq = logq;
	cipher.logp -= bitsDown;

	Ring2Utils::rightShiftAndEqual(cipher.ax, bitsDown, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, bitsDown, context.N);
}

Ciphertext Scheme::modDownBy(Ciphertext& cipher, long bitsDown) {
	ZZX bx, ax;
	long newlogq = cipher.logq - bitsDown;
	ZZ q = context.qpowvec[newlogq];

	Ring2Utils::mod(ax, cipher.ax, q, context.N);
	Ring2Utils::mod(bx, cipher.bx, q, context.N);

	return Ciphertext(ax, bx, cipher.logp, newlogq, cipher.slots, cipher.isComplex);
}

void Scheme::modDownByAndEqual(Ciphertext& cipher, long bitsDown) {
	cipher.logq -= bitsDown;
	ZZ q = context.qpowvec[cipher.logq];

	Ring2Utils::modAndEqual(cipher.ax, q, context.N);
	Ring2Utils::modAndEqual(cipher.bx, q, context.N);
}

Ciphertext Scheme::modDownTo(Ciphertext& cipher, long logq) {
	ZZX bx, ax;
	ZZ q = context.qpowvec[logq];

	Ring2Utils::mod(ax, cipher.ax, q, context.N);
	Ring2Utils::mod(bx, cipher.bx, q, context.N);
	return Ciphertext(ax, bx, cipher.logp, logq, cipher.slots);
}

void Scheme::modDownToAndEqual(Ciphertext& cipher, long logq) {
	cipher.logq = logq;
	ZZ q = context.qpowvec[logq];

	Ring2Utils::modAndEqual(cipher.ax, q, context.N);
	Ring2Utils::modAndEqual(cipher.bx, q, context.N);
}


//----------------------------------------------------------------------------------
//   ROTATIONS & CONJUGATIONS
//----------------------------------------------------------------------------------


Ciphertext Scheme::leftRotateFast(Ciphertext& cipher, long rotSlots) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ qQ = context.qpowvec[cipher.logq + context.logQ];

	ZZX bxrot, ax, bx;
	Key key = leftRotKeyMap.at(rotSlots);

	Ring2Utils::inpower(bxrot, cipher.bx, context.rotGroup[rotSlots], context.Q, context.N);
	Ring2Utils::inpower(bx, cipher.ax, context.rotGroup[rotSlots], context.Q, context.N);

	Ring2Utils::mult(ax, bx, key.ax, qQ, context.N);
	Ring2Utils::multAndEqual(bx, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(bx, bxrot, q, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::leftRotateAndEqualFast(Ciphertext& cipher, long rotSlots) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ qQ = context.qpowvec[cipher.logq + context.logQ];
	ZZX bxrot;
	Key key = leftRotKeyMap.at(rotSlots);

	Ring2Utils::inpower(bxrot, cipher.bx, context.rotGroup[rotSlots], context.Q, context.N);
	Ring2Utils::inpower(cipher.bx, cipher.ax, context.rotGroup[rotSlots], context.Q, context.N);

	Ring2Utils::mult(cipher.ax, cipher.bx, key.ax, qQ, context.N);
	Ring2Utils::multAndEqual(cipher.bx, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(cipher.ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(cipher.bx, bxrot, q, context.N);
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
	long rotSlots = context.Nh - (1 << logrotSlots);
	return leftRotateFast(cipher, rotSlots);
}

void Scheme::rightRotateByPo2AndEqual(Ciphertext& cipher, long logrotSlots) {
	long rotSlots = context.Nh - (1 << logrotSlots);
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
	ZZ q = context.qpowvec[cipher.logq];
	ZZ qQ = context.qpowvec[cipher.logq + context.logQ];

	ZZX bxconj, ax, bx;
	Key key = keyMap.at(CONJUGATION);

	Ring2Utils::conjugate(bxconj, cipher.bx, context.N);
	Ring2Utils::conjugate(bx, cipher.ax, context.N);

	Ring2Utils::mult(ax, bx, key.ax, qQ, context.N);
	Ring2Utils::multAndEqual(bx, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(bx, bxconj, q, context.N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots, cipher.isComplex);
}

void Scheme::conjugateAndEqual(Ciphertext& cipher) {
	ZZ q = context.qpowvec[cipher.logq];
	ZZ qQ = context.qpowvec[cipher.logq + context.logQ];
	ZZX bxconj;
	Key key = keyMap.at(CONJUGATION);

	Ring2Utils::conjugate(bxconj, cipher.bx, context.N);
	Ring2Utils::conjugate(cipher.bx, cipher.ax, context.N);

	Ring2Utils::mult(cipher.ax, cipher.bx, key.ax, qQ, context.N);
	Ring2Utils::multAndEqual(cipher.bx, key.bx, qQ, context.N);

	Ring2Utils::rightShiftAndEqual(cipher.ax, context.logQ, context.N);
	Ring2Utils::rightShiftAndEqual(cipher.bx, context.logQ, context.N);

	Ring2Utils::addAndEqual(cipher.bx, bxconj, q, context.N);
}
