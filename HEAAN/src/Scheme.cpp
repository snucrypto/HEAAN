/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Scheme.h"

#include "NTL/BasicThreadPool.h"
#include "StringUtils.h"
#include "SerializationUtils.h"

Scheme::Scheme(SecretKey& secretKey, Ring& ring, bool isSerialized) : ring(ring), isSerialized(isSerialized) {
	addEncKey(secretKey);
	addMultKey(secretKey);
};

void Scheme::addEncKey(SecretKey& secretKey) {
	ZZ* ex = new ZZ[ring.N];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.sampleUniform2(ax, ring.logQQ);
	ring.sampleGauss(ex);
	long np = ceil((1 + ring.logQQ + ring.logN + 2)/59.0);
	ring.mult(bx, secretKey.sx, ax, np, ring.QQ);
	ring.sub(bx, ex, bx, ring.QQ);

	np = ceil((2 * ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* rax = ring.toNTT(ax, np);
	uint64_t* rbx = ring.toNTT(bx, np);

	delete[] ax;
	delete[] bx;
	delete[] ex;

	Key* key = new Key(rax, rbx, ring.N, np);
	if(isSerialized) {
		string path = "serkey/ENCRYPTION.txt";
		SerializationUtils::writeKey(key, path);
		serKeyMap.insert(pair<long, string>(ENCRYPTION, path));
		delete key;
	} else {
		keyMap.insert(pair<long, Key*>(ENCRYPTION, new Key(rax, rbx, ring.N, np)));
	}
}

void Scheme::addMultKey(SecretKey& secretKey) {
	ZZ* ex = new ZZ[ring.N];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];
	ZZ* sxsx = new ZZ[ring.N];

	long np = ceil((2 + ring.logN + 2)/59.0);
	ring.mult(sxsx, secretKey.sx, secretKey.sx, np, ring.Q);
	ring.leftShiftAndEqual(sxsx, ring.logQ, ring.QQ);
	ring.sampleUniform2(ax, ring.logQQ);
	ring.sampleGauss(ex);
	ring.addAndEqual(ex, sxsx, ring.QQ);

	np = ceil((1 + ring.logQQ + ring.logN + 2)/59.0);
	ring.mult(bx, secretKey.sx, ax, np, ring.QQ);
	ring.sub(bx, ex, bx, ring.QQ);

	np = ceil((2 * ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* rax = ring.toNTT(ax, np);
	uint64_t* rbx = ring.toNTT(bx, np);

	delete[] ax;
	delete[] bx;
	delete[] ex;
	delete[] sxsx;

	Key* key = new Key(rax, rbx, ring.N, np);
	if(isSerialized) {
		string path = "serkey/MULTIPLICATION.txt";
		SerializationUtils::writeKey(key, path);
		serKeyMap.insert(pair<long, string>(MULTIPLICATION, path));
		delete key;
	} else {
		keyMap.insert(pair<long, Key*>(MULTIPLICATION, new Key(rax, rbx, ring.N, np)));
	}
}

void Scheme::addConjKey(SecretKey& secretKey) {
	ZZ* ex = new ZZ[ring.N];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];
	ZZ* sxconj = new ZZ[ring.N];

	ring.conjugate(sxconj, secretKey.sx);
	ring.leftShiftAndEqual(sxconj, ring.logQ, ring.QQ);
	ring.sampleUniform2(ax, ring.logQQ);
	ring.sampleGauss(ex);
	ring.addAndEqual(ex, sxconj, ring.QQ);

	long np = ceil((1 + ring.logQQ + ring.logN + 2)/59.0);
	ring.mult(bx, secretKey.sx, ax, np, ring.QQ);
	ring.sub(bx, ex, bx, ring.QQ);

	np = ceil((2 * ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* rax = ring.toNTT(ax, np);
	uint64_t* rbx = ring.toNTT(bx, np);

	delete[] ax;
	delete[] bx;
	delete[] ex;
	delete[] sxconj;

	Key* key = new Key(rax, rbx, ring.N, np);
	if(isSerialized) {
		string path = "serkey/CONJUGATION.txt";
		SerializationUtils::writeKey(key, path);
		serKeyMap.insert(pair<long, string>(CONJUGATION, path));
		delete key;
	} else {
		keyMap.insert(pair<long, Key*>(CONJUGATION, new Key(rax, rbx, ring.N, np)));
	}
}

void Scheme::addLeftRotKey(SecretKey& secretKey, long r) {
	ZZ* ex = new ZZ[ring.N];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];
	ZZ* spow = new ZZ[ring.N];

	ring.leftRotate(spow, secretKey.sx, r);
	ring.leftShiftAndEqual(spow, ring.logQ, ring.QQ);
	ring.sampleUniform2(ax, ring.logQQ);
	ring.sampleGauss(ex);
	ring.addAndEqual(ex, spow, ring.QQ);

	long np = ceil((1 + ring.logQQ + ring.logN + 2)/59.0);
	ring.mult(bx, secretKey.sx, ax, np, ring.QQ);
	ring.sub(bx, ex, bx, ring.QQ);

	np = ceil((2 * ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* rax = ring.toNTT(ax, np);
	uint64_t* rbx = ring.toNTT(bx, np);

	delete[] ax;
	delete[] bx;
	delete[] ex;
	delete[] spow;

	Key* key = new Key(rax, rbx, ring.N, np);
	if(isSerialized) {
		string path = "serkey/ROTATION_" + to_string(r) + ".txt";
		SerializationUtils::writeKey(key, path);
		serLeftRotKeyMap.insert(pair<long, string>(r, path));
		delete key;
	} else {
		leftRotKeyMap.insert(pair<long, Key*>(r, new Key(rax, rbx, ring.N, np)));
	}
}

void Scheme::addRightRotKey(SecretKey& secretKey, long r) {
	long idx = ring.Nh - r;
	if(leftRotKeyMap.find(idx) == leftRotKeyMap.end() && serLeftRotKeyMap.find(idx) == serLeftRotKeyMap.end()) {
		addLeftRotKey(secretKey, idx);
	}
}

void Scheme::addLeftRotKeys(SecretKey& secretKey) {
	for (long i = 0; i < ring.logN - 1; ++i) {
		long idx = 1 << i;
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end() && serLeftRotKeyMap.find(idx) == serLeftRotKeyMap.end()) {
			addLeftRotKey(secretKey, idx);
		}
	}
}

void Scheme::addRightRotKeys(SecretKey& secretKey) {
	for (long i = 0; i < ring.logN - 1; ++i) {
		long idx = ring.Nh - (1 << i);
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end() && serLeftRotKeyMap.find(idx) == serLeftRotKeyMap.end()) {
			addLeftRotKey(secretKey, idx);
		}
	}
}

void Scheme::addBootKey(SecretKey& secretKey, long logl, long logp) {
	ring.addBootContext(logl, logp);

	addConjKey(secretKey);
	addLeftRotKeys(secretKey);

	long loglh = logl/2;
	long k = 1 << loglh;
	long m = 1 << (logl - loglh);

	for (long i = 1; i < k; ++i) {
		if(leftRotKeyMap.find(i) == leftRotKeyMap.end() && serLeftRotKeyMap.find(i) == serLeftRotKeyMap.end()) {
			addLeftRotKey(secretKey, i);
		}
	}

	for (long i = 1; i < m; ++i) {
		long idx = i * k;
		if(leftRotKeyMap.find(idx) == leftRotKeyMap.end() && serLeftRotKeyMap.find(idx) == serLeftRotKeyMap.end()) {
			addLeftRotKey(secretKey, idx);
		}
	}
}

void Scheme::serializeRotKey(long r, string path) {
	map<long, Key*>::iterator itr = leftRotKeyMap.find(r);
	if (itr != leftRotKeyMap.end()) {
		SerializationUtils::writeKey(itr->second, path);
		delete itr->second;
		leftRotKeyMap.erase(itr);
	}
}

void Scheme::deserializeRotKey(long r, string path) {
	leftRotKeyMap.insert(pair<long, Key*>(r, SerializationUtils::readKey(path)));
}

Plaintext Scheme::encode(double* vals, long n, long logp, long logq) {
	ZZ* mx = new ZZ[ring.N];
	ring.encode(mx, vals, n, logp + ring.logQ);
	return Plaintext(mx, logp, logq, ring.N, n);
}

Plaintext Scheme::encode(complex<double>* vals, long n, long logp, long logq) {
	ZZ* mx = new ZZ[ring.N];
	ring.encode(mx, vals, n, logp + ring.logQ);
	return Plaintext(mx, logp, logq, ring.N, n);
}

complex<double>* Scheme::decode(Plaintext& msg) {
	complex<double>* res = new complex<double>[msg.n];
	ring.decode(msg.mx, res, msg.n, msg.logp, msg.logq);
	return res;
}

Plaintext Scheme::encodeSingle(double val, long logp, long logq) {
	ZZ* mx = new ZZ[ring.N];
	mx[0] = EvaluatorUtils::scaleUpToZZ(val, logp + ring.logQ);
	return Plaintext(mx, logp, logq, ring.N, 1);
}

Plaintext Scheme::encodeSingle(complex<double> val, long logp, long logq) {
	ZZ* mx = new ZZ[ring.N];
	mx[0] = EvaluatorUtils::scaleUpToZZ(val.real(), logp + ring.logQ);
	mx[ring.Nh] = EvaluatorUtils::scaleUpToZZ(val.imag(), logp + ring.logQ);
	return Plaintext(mx, logp, logq, ring.N, 1);
}

complex<double> Scheme::decodeSingle(Plaintext& msg) {
	ZZ q = ring.qpows[msg.logq];

	complex<double> res;
	ZZ tmp = msg.mx[0] % q;
	if(NumBits(tmp) == msg.logq) tmp -= q;
	res.real(EvaluatorUtils::scaleDownToReal(tmp, msg.logp));

	tmp = msg.mx[ring.Nh] % q;
	if(NumBits(tmp) == msg.logq) tmp -= q;
	res.imag(EvaluatorUtils::scaleDownToReal(tmp, msg.logp));

	return res;
}

Ciphertext Scheme::encryptMsg(Plaintext& msg) {
	ZZ qQ = ring.qpows[msg.logq + ring.logQ];

	ZZ* ex = new ZZ[ring.N];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];
	ZZ* vx = new ZZ[ring.N];

	ring.sampleZO(vx);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(ENCRYPTION)) : keyMap.at(ENCRYPTION);

	long np = ceil((1 + ring.logQQ + ring.logN + 2)/59.0);
	ring.multNTT(ax, vx, key->rax, np, qQ);
	ring.sampleGauss(ex);
	ring.addAndEqual(ax, ex, qQ);

	ring.multNTT(bx, vx, key->rbx, np, qQ);
	ring.sampleGauss(ex);
	ring.addAndEqual(bx, ex, qQ);

	ring.addAndEqual(bx, msg.mx, qQ);

	ring.rightShiftAndEqual(ax, ring.logQ);
	ring.rightShiftAndEqual(bx, ring.logQ);

	delete[] ex;
	delete[] vx;
	if(isSerialized) delete key;

	return Ciphertext(ax, bx, msg.logp, msg.logq, msg.N, msg.n);
}

Plaintext Scheme::decryptMsg(SecretKey& secretKey, Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ* mx = new ZZ[ring.N];

	long np = ceil((1 + cipher.logq + ring.logN + 2)/59.0);
	ring.mult(mx, cipher.ax, secretKey.sx, np, q);
	ring.addAndEqual(mx, cipher.bx, q);
	return Plaintext(mx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

Ciphertext Scheme::encrypt(complex<double>* vals, long n, long logp, long logq) {
	Plaintext msg = encode(vals, n, logp, logq);
	return encryptMsg(msg);
}

Ciphertext Scheme::encrypt(double* vals, long n, long logp, long logq) {
	Plaintext msg = encode(vals, n, logp, logq);
	return encryptMsg(msg);
}

Ciphertext Scheme::encryptZeros(long n, long logp, long logq) {
	Ciphertext res = encryptSingle(0.0, logp, logq);
	res.n = n;
	return res;
}

complex<double>* Scheme::decrypt(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decode(msg);
}

Ciphertext Scheme::encryptSingle(complex<double> val, long logp, long logq) {
	Plaintext msg = encodeSingle(val, logp, logq);
	return encryptMsg(msg);
}

Ciphertext Scheme::encryptSingle(double val, long logp, long logq) {
	Plaintext msg = encodeSingle(val, logp, logq);
	return encryptMsg(msg);
}

complex<double> Scheme::decryptSingle(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decodeSingle(msg);
}

//-----------------------------------------

Ciphertext Scheme::negate(Ciphertext& cipher) {
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.negate(ax, cipher.ax);
	ring.negate(bx, cipher.bx);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::negateAndEqual(Ciphertext& cipher) {
	ring.negateAndEqual(cipher.ax);
	ring.negateAndEqual(cipher.bx);
}

Ciphertext Scheme::add(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.add(ax, cipher1.ax, cipher2.ax, q);
	ring.add(bx, cipher1.bx, cipher2.bx, q);

	return Ciphertext(ax, bx, cipher1.logp, cipher1.logq, cipher1.N, cipher1.n);
}

void Scheme::addAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];

	ring.addAndEqual(cipher1.ax, cipher2.ax, q);
	ring.addAndEqual(cipher1.bx, cipher2.bx, q);
}

//-----------------------------------------

Ciphertext Scheme::addConst(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	Ciphertext res = cipher;
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(res.bx[0], cipher.bx[0], cnstZZ, q);
	return res;
}

Ciphertext Scheme::addConst(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	Ciphertext res = cipher;
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(res.bx[0], cipher.bx[0], cnstZZ, q);
	return res;
}

Ciphertext Scheme::addConst(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	Ciphertext res = cipher;
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.real(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);
	AddMod(res.bx[0], cipher.bx[0], cnstZZ, q);
	return res;
}

void Scheme::addConstAndEqual(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(cipher.bx[0], cipher.bx[0], cnstZZ, q);
}

void Scheme::addConstAndEqual(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ cnstZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(cipher.bx[0], cipher.bx[0], cnstZZ, q);
}

void Scheme::addConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ cnstrZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.real(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);
	ZZ cnstiZZ = logp < 0 ? EvaluatorUtils::scaleUpToZZ(cnst.imag(), cipher.logp) : EvaluatorUtils::scaleUpToZZ(cnst.imag(), logp);
	AddMod(cipher.bx[0], cipher.bx[0], cnstrZZ, q);
	AddMod(cipher.bx[ring.Nh], cipher.bx[ring.Nh], cnstiZZ, q);
}

//-----------------------------------------

Ciphertext Scheme::sub(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.sub(ax, cipher1.ax, cipher2.ax, q);
	ring.sub(bx, cipher1.bx, cipher2.bx, q);

	return Ciphertext(ax, bx, cipher1.logp, cipher1.logq, cipher1.N, cipher1.n);
}

void Scheme::subAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];

	ring.subAndEqual(cipher1.ax, cipher2.ax, q);
	ring.subAndEqual(cipher1.bx, cipher2.bx, q);
}

void Scheme::subAndEqual2(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];

	ring.subAndEqual2(cipher1.ax, cipher2.ax, q);
	ring.subAndEqual2(cipher1.bx, cipher2.bx, q);
}

Ciphertext Scheme::imult(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.multByMonomial(ax, cipher.ax, ring.Nh);
	ring.multByMonomial(bx, cipher.bx, ring.Nh);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

Ciphertext Scheme::idiv(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.multByMonomial(ax, cipher.ax, 3 * ring.Nh);
	ring.multByMonomial(bx, cipher.bx, 3 * ring.Nh);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::imultAndEqual(Ciphertext& cipher) {
	ring.multByMonomialAndEqual(cipher.ax, ring.Nh);
	ring.multByMonomialAndEqual(cipher.bx, ring.Nh);
}

void Scheme::idivAndEqual(Ciphertext& cipher) {
	ring.multByMonomialAndEqual(cipher.ax, 3 * ring.Nh);
	ring.multByMonomialAndEqual(cipher.bx, 3 * ring.Nh);
}

Ciphertext Scheme::mult(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];
	ZZ qQ = ring.qpows[cipher1.logq + ring.logQ];

	long np = ceil((2 + cipher1.logq + cipher2.logq + ring.logN + 2)/59.0);

	uint64_t* ra1 = ring.toNTT(cipher1.ax, np);
	uint64_t* rb1 = ring.toNTT(cipher1.bx, np);
	uint64_t* ra2 = ring.toNTT(cipher2.ax, np);
	uint64_t* rb2 = ring.toNTT(cipher2.bx, np);

	ZZ* axax = new ZZ[ring.N];
	ZZ* bxbx = new ZZ[ring.N];
	ring.multDNTT(axax, ra1, ra2, np, q);
	ring.multDNTT(bxbx, rb1, rb2, np, q);

	ZZ* axbx = new ZZ[ring.N];
	ring.addNTTAndEqual(ra1, rb1, np);
	ring.addNTTAndEqual(ra2, rb2, np);
	ring.multDNTT(axbx, ra1, ra2, np, q);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(MULTIPLICATION)) : keyMap.at(MULTIPLICATION);

	ZZ* axmult = new ZZ[ring.N];
	ZZ* bxmult = new ZZ[ring.N];

	np = ceil((cipher1.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* raa = ring.toNTT(axax, np);
	ring.multDNTT(axmult, raa, key->rax, np, qQ);
	ring.multDNTT(bxmult, raa, key->rbx, np, qQ);

	ring.rightShiftAndEqual(axmult, ring.logQ);
	ring.rightShiftAndEqual(bxmult, ring.logQ);

	ring.addAndEqual(axmult, axbx, q);
	ring.subAndEqual(axmult, bxbx, q);
	ring.subAndEqual(axmult, axax, q);
	ring.addAndEqual(bxmult, bxbx, q);

	delete[] axax;
	delete[] bxbx;
	delete[] axbx;
	delete[] ra1;
	delete[] ra2;
	delete[] rb1;
	delete[] rb2;
	delete[] raa;

	if(isSerialized) delete key;

	return Ciphertext(axmult, bxmult, cipher1.logp + cipher2.logp, cipher1.logq, cipher1.N, cipher1.n);
}

void Scheme::multAndEqual(Ciphertext& cipher1, Ciphertext& cipher2) {
	ZZ q = ring.qpows[cipher1.logq];
	ZZ qQ = ring.qpows[cipher1.logq + ring.logQ];

	long np = ceil((2 + cipher1.logq + cipher2.logq + ring.logN + 2)/59.0);

	uint64_t* ra1 = ring.toNTT(cipher1.ax, np);
	uint64_t* rb1 = ring.toNTT(cipher1.bx, np);
	uint64_t* ra2 = ring.toNTT(cipher2.ax, np);
	uint64_t* rb2 = ring.toNTT(cipher2.bx, np);

	ZZ* axax = new ZZ[ring.N];
	ZZ* bxbx = new ZZ[ring.N];
	ring.multDNTT(axax, ra1, ra2, np, q);
	ring.multDNTT(bxbx, rb1, rb2, np, q);

	ZZ* axbx = new ZZ[ring.N];
	ring.addNTTAndEqual(ra1, rb1, np);
	ring.addNTTAndEqual(ra2, rb2, np);
	ring.multDNTT(axbx, ra1, ra2, np, q);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(MULTIPLICATION)) : keyMap.at(MULTIPLICATION);

	np = ceil((cipher1.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* raa = ring.toNTT(axax, np);
	ring.multDNTT(cipher1.ax, raa, key->rax, np, qQ);
	ring.multDNTT(cipher1.bx, raa, key->rbx, np, qQ);

	ring.rightShiftAndEqual(cipher1.ax, ring.logQ);
	ring.rightShiftAndEqual(cipher1.bx, ring.logQ);

	ring.addAndEqual(cipher1.ax, axbx, q);
	ring.subAndEqual(cipher1.ax, bxbx, q);
	ring.subAndEqual(cipher1.ax, axax, q);
	ring.addAndEqual(cipher1.bx, bxbx, q);

	cipher1.logp += cipher2.logp;

	delete[] axax;
	delete[] bxbx;
	delete[] axbx;
	delete[] ra1;
	delete[] ra2;
	delete[] rb1;
	delete[] rb2;
	delete[] raa;

	if(isSerialized) delete key;
}

//-----------------------------------------

Ciphertext Scheme::square(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ qQ = ring.qpows[cipher.logq + ring.logQ];

	long np = ceil((2 * cipher.logq + ring.logN + 2)/59.0);

	uint64_t* ra = ring.toNTT(cipher.ax, np);
	uint64_t* rb = ring.toNTT(cipher.bx, np);

	ZZ* axax = new ZZ[ring.N];
	ZZ* axbx = new ZZ[ring.N];
	ZZ* bxbx = new ZZ[ring.N];

	ZZ* bxmult = new ZZ[ring.N];
	ZZ* axmult = new ZZ[ring.N];

	ring.squareNTT(bxbx, rb, np, q);
	ring.squareNTT(axax, ra, np, q);
	ring.multDNTT(axbx, ra, rb, np, q);
	ring.addAndEqual(axbx, axbx, q);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(MULTIPLICATION)) : keyMap.at(MULTIPLICATION);

	np = ceil((cipher.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* raa = ring.toNTT(axax, np);
	ring.multDNTT(axmult, raa, key->rax, np, qQ);
	ring.multDNTT(bxmult, raa, key->rbx, np, qQ);

	ring.rightShiftAndEqual(axmult, ring.logQ);
	ring.rightShiftAndEqual(bxmult, ring.logQ);

	ring.addAndEqual(axmult, axbx, q);
	ring.addAndEqual(bxmult, bxbx, q);

	delete[] axbx;
	delete[] axax;
	delete[] bxbx;

	delete[] ra;
	delete[] rb;
	delete[] raa;

	if(isSerialized) delete key;

	return Ciphertext(axmult, bxmult, 2 * cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::squareAndEqual(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ qQ = ring.qpows[cipher.logq + ring.logQ];

	long np = ceil((2 + 2 * cipher.logq + ring.logN + 2)/59.0);

	uint64_t* ra = ring.toNTT(cipher.ax, np);
	uint64_t* rb = ring.toNTT(cipher.bx, np);

	ZZ* axax = new ZZ[ring.N];
	ZZ* axbx = new ZZ[ring.N];
	ZZ* bxbx = new ZZ[ring.N];

	ZZ* bxmult = new ZZ[ring.N];
	ZZ* axmult = new ZZ[ring.N];

	ring.squareNTT(bxbx, rb, np, q);
	ring.squareNTT(axax, ra, np, q);

	ring.multDNTT(axbx, ra, rb, np, q);
	ring.addAndEqual(axbx, axbx, q);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(MULTIPLICATION)) : keyMap.at(MULTIPLICATION);

	np = ceil((cipher.logq + ring.logQQ + ring.logN + 2)/59.0);

	uint64_t* raa = ring.toNTT(axax, np);
	ring.multDNTT(cipher.ax, raa, key->rax, np, qQ);
	ring.multDNTT(cipher.bx, raa, key->rbx, np, qQ);

	ring.rightShiftAndEqual(cipher.ax, ring.logQ);
	ring.rightShiftAndEqual(cipher.bx, ring.logQ);

	ring.addAndEqual(cipher.ax, axbx, q);
	ring.addAndEqual(cipher.bx, bxbx, q);
	cipher.logp *= 2;

	delete[] axbx;
	delete[] axax;
	delete[] bxbx;

	delete[] ra;
	delete[] rb;
	delete[] raa;

	if(isSerialized) delete key;
}

//-----------------------------------------

Ciphertext Scheme::multByConst(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	ring.multByConst(ax, cipher.ax, cnstZZ, q);
	ring.multByConst(bx, cipher.bx, cnstZZ, q);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.N, cipher.n);
}

Ciphertext Scheme::multByConst(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);

	ring.multByConst(ax, cipher.ax, cnstZZ, q);
	ring.multByConst(bx, cipher.bx, cnstZZ, q);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.N, cipher.n);
}

Ciphertext Scheme::multByConstVec(Ciphertext& cipher, complex<double>* cnstVec, long logp) {
	return NULL;
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, double cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	ring.multByConstAndEqual(cipher.ax, cnstZZ, q);
	ring.multByConstAndEqual(cipher.bx, cnstZZ, q);
	cipher.logp += logp;
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, RR& cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	ring.multByConstAndEqual(cipher.ax, cnstZZ, q);
	ring.multByConstAndEqual(cipher.bx, cnstZZ, q);
	cipher.logp += logp;
}

void Scheme::multByConstAndEqual(Ciphertext& cipher, complex<double> cnst, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst.real(), logp);

	ring.multByConstAndEqual(cipher.ax, cnstZZ, q);
	ring.multByConstAndEqual(cipher.bx, cnstZZ, q);
	cipher.logp += logp;
}

Ciphertext Scheme::multByPoly(Ciphertext& cipher, ZZ* poly, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	long bnd = ring.maxBits(poly, ring.N);
	long np = ceil((cipher.logq + bnd + ring.logN + 2)/59.0);
	uint64_t* rpoly = ring.toNTT(poly, np);
	ring.multNTT(ax, cipher.ax, rpoly, np, q);
	ring.multNTT(bx, cipher.bx, rpoly, np, q);
	delete[] rpoly;
	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.N, cipher.n);
}

Ciphertext Scheme::multByPolyNTT(Ciphertext& cipher, uint64_t* rpoly, long bnd, long logp) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	long np = ceil((cipher.logq + bnd + ring.logN + 2)/59.0);
	ring.multNTT(ax, cipher.ax, rpoly, np, q);
	ring.multNTT(bx, cipher.bx, rpoly, np, q);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.N, cipher.n);

}

void Scheme::multByPolyAndEqual(Ciphertext& cipher, ZZ* poly, long logp) {
	ZZ q = ring.qpows[cipher.logq];

	long bnd = ring.maxBits(poly, ring.N);
	long np = ceil((cipher.logq + bnd + ring.logN + 2)/59.0);
	uint64_t* rpoly = ring.toNTT(poly, np);
	ring.multNTTAndEqual(cipher.ax, rpoly, np, q);
	ring.multNTTAndEqual(cipher.bx, rpoly, np, q);
	delete[] rpoly;

	cipher.logp += logp;
}

void Scheme::multByPolyNTTAndEqual(Ciphertext& cipher, uint64_t* rpoly, long bnd, long logp) {
	ZZ q = ring.qpows[cipher.logq];

	long np = ceil((cipher.logq + bnd + ring.logN + 2)/59.0);
	ring.multNTTAndEqual(cipher.ax, rpoly, np, q);
	ring.multNTTAndEqual(cipher.bx, rpoly, np, q);

	cipher.logp += logp;
}

//-----------------------------------------

Ciphertext Scheme::multByMonomial(Ciphertext& cipher, const long degree) {
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.multByMonomial(ax, cipher.ax, degree);
	ring.multByMonomial(bx, cipher.bx, degree);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::multByMonomialAndEqual(Ciphertext& cipher, const long degree) {
	ring.multByMonomialAndEqual(cipher.ax, degree);
	ring.multByMonomialAndEqual(cipher.bx, degree);
}

//-----------------------------------------

Ciphertext Scheme::leftShift(Ciphertext& cipher, long bits) {
	ZZ q = ring.qpows[cipher.logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.leftShift(ax, cipher.ax, bits, q);
	ring.leftShift(bx, cipher.bx, bits, q);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::leftShiftAndEqual(Ciphertext& cipher, long bits) {
	ZZ q = ring.qpows[cipher.logq];
	ring.leftShiftAndEqual(cipher.ax, bits, q);
	ring.leftShiftAndEqual(cipher.bx, bits, q);
}

void Scheme::doubleAndEqual(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	ring.doubleAndEqual(cipher.ax, q);
	ring.doubleAndEqual(cipher.bx, q);
}

Ciphertext Scheme::divByPo2(Ciphertext& cipher, long bits) {
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.rightShift(ax, cipher.ax, bits);
	ring.rightShift(bx, cipher.bx, bits);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq - bits, cipher.N, cipher.n);
}

void Scheme::divByPo2AndEqual(Ciphertext& cipher, long bits) {
	ring.rightShiftAndEqual(cipher.ax, bits);
	ring.rightShiftAndEqual(cipher.bx, bits);
	cipher.logq -= bits;
}


//-----------------------------------------

Ciphertext Scheme::reScaleBy(Ciphertext& cipher, long dlogq) {
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.rightShift(ax, cipher.ax, dlogq);
	ring.rightShift(bx, cipher.bx, dlogq);

	return Ciphertext(ax, bx, cipher.logp - dlogq, cipher.logq - dlogq, cipher.N, cipher.n);
}

Ciphertext Scheme::reScaleTo(Ciphertext& cipher, long logq) {
	long dlogq = cipher.logq - logq;
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.rightShift(ax, cipher.ax, dlogq);
	ring.rightShift(bx, cipher.bx, dlogq);


	return Ciphertext(ax, bx, cipher.logp - dlogq, logq, cipher.N, cipher.n);
}

void Scheme::reScaleByAndEqual(Ciphertext& cipher, long dlogq) {
	ring.rightShiftAndEqual(cipher.ax, dlogq);
	ring.rightShiftAndEqual(cipher.bx, dlogq);
	cipher.logq -= dlogq;
	cipher.logp -= dlogq;
}

void Scheme::reScaleToAndEqual(Ciphertext& cipher, long logq) {
	long dlogq = cipher.logq - logq;
	ring.rightShiftAndEqual(cipher.ax, dlogq);
	ring.rightShiftAndEqual(cipher.bx, dlogq);
	cipher.logq = logq;
	cipher.logp -= dlogq;
}

Ciphertext Scheme::modDownBy(Ciphertext& cipher, long dlogq) {
	ZZ q = ring.qpows[cipher.logq - dlogq];
	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.mod(ax, cipher.ax, q);
	ring.mod(bx, cipher.bx, q);
	return Ciphertext(ax, bx, cipher.logp, cipher.logq - dlogq, cipher.N, cipher.n);
}

void Scheme::modDownByAndEqual(Ciphertext& cipher, long dlogq) {
	ZZ q = ring.qpows[cipher.logq - dlogq];
	ring.modAndEqual(cipher.ax, q);
	ring.modAndEqual(cipher.bx, q);
	cipher.logq -= dlogq;
}

Ciphertext Scheme::modDownTo(Ciphertext& cipher, long logq) {
	ZZ q = ring.qpows[logq];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.mod(ax, cipher.ax, q);
	ring.mod(bx, cipher.bx, q);
	return Ciphertext(ax, bx, cipher.logp, logq, cipher.N, cipher.n);
}

void Scheme::modDownToAndEqual(Ciphertext& cipher, long logq) {
	ZZ q = ring.qpows[logq];
	cipher.logq = logq;
	ring.modAndEqual(cipher.ax, q);
	ring.modAndEqual(cipher.bx, q);
}


//----------------------------------------------------------------------------------
//   ROTATIONS & CONJUGATIONS
//----------------------------------------------------------------------------------


Ciphertext Scheme::leftRotateFast(Ciphertext& cipher, long r) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ qQ = ring.qpows[cipher.logq + ring.logQ];

	ZZ* bxrot = new ZZ[ring.N];
	ZZ* axrot = new ZZ[ring.N];

	ZZ* ax = new ZZ[ring.N];
	ZZ* bx = new ZZ[ring.N];

	ring.leftRotate(bxrot, cipher.bx, r);
	ring.leftRotate(axrot, cipher.ax, r);

	Key* key = isSerialized ? SerializationUtils::readKey(serLeftRotKeyMap.at(r)) : leftRotKeyMap.at(r);

	long np = ceil((cipher.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* rarot = ring.toNTT(axrot, np);
	ring.multDNTT(ax, rarot, key->rax, np, qQ);
	ring.multDNTT(bx, rarot, key->rbx, np, qQ);

	ring.rightShiftAndEqual(ax, ring.logQ);
	ring.rightShiftAndEqual(bx, ring.logQ);

	ring.addAndEqual(bx, bxrot, q);

	delete[] bxrot;
	delete[] axrot;
	delete[] rarot;

	if(isSerialized) delete key;

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::leftRotateFastAndEqual(Ciphertext& cipher, long r) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ qQ = ring.qpows[cipher.logq + ring.logQ];

	ZZ* bxrot = new ZZ[ring.N];
	ZZ* axrot = new ZZ[ring.N];

	ring.leftRotate(bxrot, cipher.bx, r);
	ring.leftRotate(axrot, cipher.ax, r);
	Key* key = isSerialized ? SerializationUtils::readKey(serLeftRotKeyMap.at(r)) : leftRotKeyMap.at(r);
	long np = ceil((cipher.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* rarot = ring.toNTT(axrot, np);
	ring.multDNTT(cipher.ax, rarot, key->rax, np, qQ);
	ring.multDNTT(cipher.bx, rarot, key->rbx, np, qQ);

	ring.rightShiftAndEqual(cipher.ax, ring.logQ);
	ring.rightShiftAndEqual(cipher.bx, ring.logQ);

	ring.addAndEqual(cipher.bx, bxrot, q);

	delete[] bxrot;
	delete[] axrot;
	delete[] rarot;

	if(isSerialized) delete key;
}

Ciphertext Scheme::rightRotateFast(Ciphertext& cipher, long r) {
	long rr = ring.Nh - r;
	return leftRotateFast(cipher, rr);
}

void Scheme::rightRotateFastAndEqual(Ciphertext& cipher, long r) {
	long rr = ring.Nh - r;
	leftRotateFastAndEqual(cipher, rr);
}

Ciphertext Scheme::leftRotate(Ciphertext& cipher, long r) {
	Ciphertext res = cipher;
	leftRotateAndEqual(res, r);
	return res;
}

void Scheme::leftRotateAndEqual(Ciphertext& cipher, long r) {
	long rem = r % cipher.n;
	long logr = log2((double)rem) + 1;
	for (long i = 0; i < logr; ++i) {
		if(bit(rem, i)) {
			leftRotateFastAndEqual(cipher, (1 << i));
		}
	}
}

Ciphertext Scheme::rightRotate(Ciphertext& cipher, long r) {
	Ciphertext res = cipher;
	rightRotateAndEqual(res, r);
	return res;
}

void Scheme::rightRotateAndEqual(Ciphertext& cipher, long r) {
	long rem = r % cipher.n;
	long logr = log2((double)rem) + 1;
	for (long i = 0; i < logr; ++i) {
		if(bit(rem, i)) {
			rightRotateFastAndEqual(cipher, (1 << i));
		}
	}
}

Ciphertext Scheme::conjugate(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ qQ = ring.qpows[cipher.logq + ring.logQ];

	ZZ* bxconj = new ZZ[ring.N];
	ZZ* axconj = new ZZ[ring.N];

	ZZ* bx = new ZZ[ring.N];
	ZZ* ax = new ZZ[ring.N];

	ring.conjugate(bxconj, cipher.bx);
	ring.conjugate(axconj, cipher.ax);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(CONJUGATION)) : keyMap.at(CONJUGATION);

	long np = ceil((cipher.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* raconj = ring.toNTT(axconj, np);
	ring.multDNTT(ax, raconj, key->rax, np, qQ);
	ring.multDNTT(bx, raconj, key->rbx, np, qQ);

	ring.rightShiftAndEqual(ax, ring.logQ);
	ring.rightShiftAndEqual(bx, ring.logQ);

	ring.addAndEqual(bx, bxconj, q);

	delete[] bxconj;
	delete[] axconj;
	delete[] raconj;

	if(isSerialized) delete key;

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.N, cipher.n);
}

void Scheme::conjugateAndEqual(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	ZZ qQ = ring.qpows[cipher.logq + ring.logQ];

	ZZ* bxconj = new ZZ[ring.N];
	ZZ* axconj = new ZZ[ring.N];

	ring.conjugate(bxconj, cipher.bx);
	ring.conjugate(axconj, cipher.ax);

	Key* key = isSerialized ? SerializationUtils::readKey(serKeyMap.at(CONJUGATION)) : keyMap.at(CONJUGATION);

	long np = ceil((cipher.logq + ring.logQQ + ring.logN + 2)/59.0);
	uint64_t* raconj = ring.toNTT(axconj, np);
	ring.multDNTT(cipher.ax, raconj, key->rax, np, qQ);
	ring.multDNTT(cipher.bx, raconj, key->rbx, np, qQ);

	ring.rightShiftAndEqual(cipher.ax, ring.logQ);
	ring.rightShiftAndEqual(cipher.bx, ring.logQ);

	ring.addAndEqual(cipher.bx, bxconj, q);

	delete[] bxconj;
	delete[] axconj;
	delete[] raconj;

	if(isSerialized) delete key;
}


//----------------------------------------------------------------------------------
//   BOOTSTRAPPING
//----------------------------------------------------------------------------------


void Scheme::normalizeAndEqual(Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];

	for (long i = 0; i < ring.N; ++i) {
		if(NumBits(cipher.ax[i]) == cipher.logq) cipher.ax[i] -= q;
		if(NumBits(cipher.bx[i]) == cipher.logq) cipher.bx[i] -= q;
	}
}

void Scheme::coeffToSlotAndEqual(Ciphertext& cipher) {
	long slots = cipher.n;
	long logSlots = log2(slots);
	long logk = logSlots / 2;
	long k = 1 << logk;

	Ciphertext* rotvec = new Ciphertext[k];
	rotvec[0] = cipher;

	NTL_EXEC_RANGE(k - 1, first, last);
	for (long j = first; j < last; ++j) {
		rotvec[j + 1] = leftRotateFast(rotvec[0], j + 1);
	}
	NTL_EXEC_RANGE_END;

	BootContext bootContext = ring.bootContextMap.at(logSlots);

	Ciphertext* tmpvec = new Ciphertext[k];

	NTL_EXEC_RANGE(k, first, last);
	for (long j = first; j < last; ++j) {
		tmpvec[j] = multByPolyNTT(rotvec[j], bootContext.rpvec[j], bootContext.bndvec[j], bootContext.logp);
	}
	NTL_EXEC_RANGE_END;

	for (long j = 1; j < k; ++j) {
		addAndEqual(tmpvec[0], tmpvec[j]);
	}
	cipher = tmpvec[0];
	for (long ki = k; ki < slots; ki += k) {
		NTL_EXEC_RANGE(k, first, last);
		for (long j = first; j < last; ++j) {
			tmpvec[j] = multByPolyNTT(rotvec[j], bootContext.rpvec[j + ki], bootContext.bndvec[j + ki], bootContext.logp);
		}
		NTL_EXEC_RANGE_END;
		for (long j = 1; j < k; ++j) {
			addAndEqual(tmpvec[0], tmpvec[j]);
		}
		leftRotateFastAndEqual(tmpvec[0], ki);
		addAndEqual(cipher, tmpvec[0]);
	}
	reScaleByAndEqual(cipher, bootContext.logp);
	delete[] rotvec;
	delete[] tmpvec;
}

void Scheme::slotToCoeffAndEqual(Ciphertext& cipher) {
	long slots = cipher.n;
	long logSlots = log2(slots);
	long logk = logSlots / 2;
	long k = 1 << logk;

	Ciphertext* rotvec = new Ciphertext[k];
	rotvec[0] = cipher;

	NTL_EXEC_RANGE(k-1, first, last);
	for (long j = first; j < last; ++j) {
		rotvec[j + 1] = leftRotateFast(rotvec[0], j + 1);
	}
	NTL_EXEC_RANGE_END;

	BootContext bootContext = ring.bootContextMap.at(logSlots);

	Ciphertext* tmpvec = new Ciphertext[k];

	NTL_EXEC_RANGE(k, first, last);
	for (long j = first; j < last; ++j) {
		tmpvec[j] = multByPolyNTT(rotvec[j], bootContext.rpvecInv[j], bootContext.bndvecInv[j], bootContext.logp);
	}
	NTL_EXEC_RANGE_END;

	for (long j = 1; j < k; ++j) {
		addAndEqual(tmpvec[0], tmpvec[j]);
	}
	cipher = tmpvec[0];

	for (long ki = k; ki < slots; ki+=k) {
		NTL_EXEC_RANGE(k, first, last);
		for (long j = first; j < last; ++j) {
			tmpvec[j] = multByPolyNTT(rotvec[j], bootContext.rpvecInv[j + ki], bootContext.bndvecInv[j + ki], bootContext.logp);
		}
		NTL_EXEC_RANGE_END;

		for (long j = 1; j < k; ++j) {
			addAndEqual(tmpvec[0], tmpvec[j]);
		}

		leftRotateFastAndEqual(tmpvec[0], ki);
		addAndEqual(cipher, tmpvec[0]);
	}
	reScaleByAndEqual(cipher, bootContext.logp);
	delete[] rotvec;
	delete[] tmpvec;
}

void Scheme::exp2piAndEqual(Ciphertext& cipher, long logp) {
	Ciphertext cipher2 = square(cipher);
	reScaleByAndEqual(cipher2, logp); // cipher2.logq : logq - logp

	Ciphertext cipher4 = square(cipher2);
	reScaleByAndEqual(cipher4, logp); // cipher4.logq : logq -2logp
	RR c = 1/(2*Pi);
	Ciphertext cipher01 = addConst(cipher, c, logp); // cipher01.logq : logq

	c = 2*Pi;
	multByConstAndEqual(cipher01, c, logp);
	reScaleByAndEqual(cipher01, logp); // cipher01.logq : logq - logp

	c = 3/(2*Pi);
	Ciphertext cipher23 = addConst(cipher, c, logp); // cipher23.logq : logq

	c = 4*Pi*Pi*Pi/3;
	multByConstAndEqual(cipher23, c, logp);
	reScaleByAndEqual(cipher23, logp); // cipher23.logq : logq - logp

	multAndEqual(cipher23, cipher2);
	reScaleByAndEqual(cipher23, logp); // cipher23.logq : logq - 2logp

	addAndEqual(cipher23, cipher01); // cipher23.logq : logq - 2logp

	c = 5/(2*Pi);
	Ciphertext cipher45 = addConst(cipher, c, logp); // cipher45.logq : logq

	c = 4*Pi*Pi*Pi*Pi*Pi/15;
	multByConstAndEqual(cipher45, c, logp);
	reScaleByAndEqual(cipher45, logp); // cipher45.logq : logq - logp

	c = 7/(2*Pi);
	addConstAndEqual(cipher, c, logp); // cipher.logq : logq

	c = 8*Pi*Pi*Pi*Pi*Pi*Pi*Pi/315;
	multByConstAndEqual(cipher, c, logp);
	reScaleByAndEqual(cipher, logp); // cipher.logq : logq - logp

	multAndEqual(cipher, cipher2);
	reScaleByAndEqual(cipher, logp); // cipher.logq : logq - 2logp

	modDownByAndEqual(cipher45, logp); // cipher45.logq : logq - 2logp
	addAndEqual(cipher, cipher45); // cipher.logq : logq - 2logp

	multAndEqual(cipher, cipher4);
	reScaleByAndEqual(cipher, logp); // cipher.logq : logq - 3logp

	modDownByAndEqual(cipher23, logp);
	addAndEqual(cipher, cipher23); // cipher.logq : logq - 3logp
}

void Scheme::evalExpAndEqual(Ciphertext& cipher, long logT, long logI) {
	long slots = cipher.n;
	long logSlots = log2(slots);
	BootContext bootContext = ring.bootContextMap.at(logSlots);
	if(logSlots < ring.logNh) {
		Ciphertext tmp = conjugate(cipher);
		subAndEqual(cipher, tmp);
		divByPo2AndEqual(cipher, logT + 1); // bitDown: logT + 1
		exp2piAndEqual(cipher, bootContext.logp); // bitDown: logT + 1 + 3(logq + logI)
		for (long i = 0; i < logI + logT; ++i) {
			squareAndEqual(cipher);
			reScaleByAndEqual(cipher, bootContext.logp);
		}
		tmp = conjugate(cipher);

		subAndEqual(cipher, tmp);

		tmp = multByPolyNTT(cipher, bootContext.rp1, bootContext.bnd1, bootContext.logp);
		Ciphertext tmprot = leftRotateFast(tmp, slots);
		addAndEqual(tmp, tmprot);
		multByPolyNTTAndEqual(cipher, bootContext.rp2, bootContext.bnd2, bootContext.logp);
		tmprot = leftRotateFast(cipher, slots);
		addAndEqual(cipher, tmprot);
		addAndEqual(cipher, tmp);
		// bitDown: logT + 1 + 3(logq + logI) + (logI + logT)(logq + logI)
	} else {
		Ciphertext tmp = conjugate(cipher);
		Ciphertext c2 = sub(cipher, tmp);
		addAndEqual(cipher, tmp);
		imultAndEqual(cipher);
		divByPo2AndEqual(cipher, logT + 1); // cipher bitDown: logT + 1
		reScaleByAndEqual(c2, logT + 1); // c2 bitDown: logT + 1
		exp2piAndEqual(cipher, bootContext.logp); // cipher bitDown: logT + 1 + 3(logq + logI)
		exp2piAndEqual(c2, bootContext.logp); // c2 bitDown: logT + 1 + 3(logq + logI)
		for (long i = 0; i < logI + logT; ++i) {
			squareAndEqual(c2);
			squareAndEqual(cipher);
			reScaleByAndEqual(c2, bootContext.logp);
			reScaleByAndEqual(cipher, bootContext.logp);
		}
		tmp = conjugate(c2);
		subAndEqual(c2, tmp);
		tmp = conjugate(cipher);
		subAndEqual(cipher, tmp);
		imultAndEqual(cipher);
		subAndEqual2(c2, cipher);
		RR c = 0.25/Pi;
		multByConstAndEqual(cipher, c, bootContext.logp);
		// bitDown: logT + 1 + 3(logq + logI) + (logI + logT)(logq + logI)
	}
	reScaleByAndEqual(cipher, bootContext.logp + logI);
	// if (logSlots == 0 && !cipher.isComplex) bitDown: logT + 3(logq + logI) + (logI + logT)(logq + logI) + logq + 2logI
	// else bitDown: logT + 1 + 3(logq + logI) + (logI + logT)(logq + logI) + logq + 2logI
}

void Scheme::bootstrapAndEqual(Ciphertext& cipher, long logq, long logQ, long logT, long logI) {
	long logSlots = log2(cipher.n);
	long logp = cipher.logp;

	modDownToAndEqual(cipher, logq);
	normalizeAndEqual(cipher);

	cipher.logq = logQ;
	cipher.logp = logq + 4;
	for (long i = logSlots; i < ring.logNh; ++i) {
		Ciphertext rot = leftRotateFast(cipher, (1 << i));
		addAndEqual(cipher, rot);
	}

	divByPo2AndEqual(cipher, ring.logNh); // bitDown: context.logNh - logSlots
	coeffToSlotAndEqual(cipher);
	evalExpAndEqual(cipher, logT, logI); // bitDown: context.logNh + (logI + logT + 5) * logq + (logI + logT + 6) * logI + logT + 1
	slotToCoeffAndEqual(cipher);

	cipher.logp = logp;
}
