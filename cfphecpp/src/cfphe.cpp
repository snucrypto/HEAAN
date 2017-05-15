#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>

#include "czz/CZZ.h"
#include "scheme/Cipher.h"
#include "scheme/Message.h"
#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SchemeAlgo.h"
#include "scheme/SecKey.h"
#include "utils/CKsi.h"
#include "utils/NumUtils.h"
#include "utils/Ring2Utils.h"
#include "utils/StringUtils.h"
#include "utils/TimeUtils.h"

using namespace std;
using namespace NTL;

void testtest() {
	cout << "!!! START TEST TEST !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 4;
	long logl = 1;
	long logp = 8;
	long L = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long slots = (1 << (logn-1)) - 1;
	CZZ m1, m2, madd, mmult, mmulte, mmultms;

	m1 = params.cksi.pows[2][1];
	m2 = params.cksi.pows[2][0];

	vector<CZZ> m1vec;
	vector<CZZ> m2vec;
	vector<CZZ> mmultvec;

	m1vec.push_back(m2);
	for (long i = 1; i < slots; ++i) {
		m1vec.push_back(m1);
	}

	vector<CZZ> m1conj = scheme.conj(m1vec);

	vector<CZZ> fftinv1 = NumUtils::fftInv(m1conj, params.cksi);
	for (int i = 0; i < params.n; ++i) {

		fftinv1[i] = fftinv1[i] + CZZ(random() / 100000, random() / 100000);
	}
	timeutils.start("fft");
	vector<CZZ> fft1 = NumUtils::fft(fftinv1, params.cksi);
	timeutils.stop("fft");
	timeutils.start("fft but");
	vector<CZZ> fftbut = NumUtils::fftbutterfly(fftinv1, params.cksi);
	timeutils.stop("fft but");
	vector<CZZ> fftbutInv = NumUtils::fftbutterflyInv(fftbut, params.cksi);
	cout << "---------------------" << endl;
	vector<CZZ> fftinv11 = NumUtils::fftInv(fft1, params.cksi);

	cout << "---------------------" << endl;
	StringUtils::show(m1conj);
	StringUtils::show(fft1);
	StringUtils::show(fftbut);
	cout << "---------------------" << endl;
	StringUtils::show(fftinv1);
	StringUtils::show(fftinv11);
	StringUtils::show(fftbutInv);
	cout << "---------------------" << endl;

	cout << "!!! STOP TEST TEST !!!" << endl;
}

void testDumb() {
	cout << "!!! START TEST DUMB !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 5;
	long logl = 1;
	long logp = 30;
	long L = 4;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long slots = (1 << (logn-1)) - 1;
	CZZ m1, m2, madd, mmult, mmulte, mmultms;

	m1 = params.cksi.pows[3][0];
	m2 = params.cksi.pows[3][1];

	vector<CZZ> m1vec;
	vector<CZZ> m2vec;
	vector<CZZ> mmultvec;


	for (long i = 0; i < slots; ++i) {
		m1vec.push_back(m1);
		m2vec.push_back(m2);
		mmultvec.push_back(m1vec[i] * m2vec[i]);
	}

	vector<CZZ> m1conj = scheme.conj(m1vec);
	vector<CZZ> m2conj = scheme.conj(m2vec);
	vector<CZZ> mmultconj = scheme.conj(mmultvec);

	vector<CZZ> fftinv1 = NumUtils::fftInv(m1conj, params.cksi);
	vector<CZZ> fftinv2 = NumUtils::fftInv(m2conj, params.cksi);
	ZZX poly1, poly2;
	poly1.SetLength(params.n);
	poly2.SetLength(params.n);
	for (long i = 0; i < params.n; ++i) {
		poly1.rep[i] = fftinv1[i].r;
		poly2.rep[i] = fftinv2[i].r;
	}

	poly1.normalize();
	poly2.normalize();

	ZZX poly;
	poly.SetLength(params.n);
	Ring2Utils::mult(poly, poly1, poly2, params.q, params.n);
	vector<CZZ> fftinvmult;
	for (long i = 0; i < params.n; ++i) {
		CZZ v(poly.rep[i], ZZ(0));
		scheme.trueValue(v, params.q);
		fftinvmult.push_back(v);
	}
	vector<CZZ> dmultconj = NumUtils::fft(fftinvmult, params.cksi);
	for (long i = 0; i < params.n; ++i) {
		scheme.trueValue(dmultconj[i], params.q);
	}

	StringUtils::show(m1conj);
	StringUtils::show(m2conj);
	cout<< "------------------" << endl;
	StringUtils::show(fftinv1);
	StringUtils::show(fftinv2);
	cout<< "------------------" << endl;
	StringUtils::show(fftinvmult);
	cout<< "------------------" << endl;
	StringUtils::show(mmultconj);
	StringUtils::show(dmultconj);
	cout<< "------------------" << endl;


	cout << "!!! STOP TEST DUMB !!!" << endl;
}

void testEncode() {
	cout << "!!! START TEST ENCODE !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 1;
	long logp = 30;
	long L = 10;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long logSlots = 12;
	long mvecsize = (1 << logSlots) - 1;

	CZZ m;
	vector<CZZ> mvec;
	for (long i = 0; i < mvecsize; ++i) {
		m = params.cksi.pows[5][i % 3];
		mvec.push_back(m);
	}

	vector<CZZ> mconj = scheme.conj(mvec);
	Message msg = scheme.encode(mconj);
	Cipher cipher = scheme.encrypt(msg);
	Message denc = scheme.decrypt(cipher);
	vector<CZZ> dconj = scheme.decode(denc);
	vector<CZZ> dvec = scheme.dconj(dconj);

	for (long i = 0; i < mvecsize; ++i) {
		cout << "---------------------" << endl;
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
		cout << "---------------------" << endl;
	}

	cout << "!!! STOP TEST ENCODE !!!" << endl;
}

void testOperations() {
	cout << "!!! START TEST OPERATIONS !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 5;
	long logl = 1;
	long logp = 30;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long slots = (1 << (logn-1)) - 1;
	CZZ m1, m2, madd, mmult, mmulte, mmultms;

	m1 = params.cksi.pows[4][1];
	m2 = params.cksi.pows[4][2];

	vector<CZZ> m1vec;
	vector<CZZ> m2vec;
	vector<CZZ> maddvec;
	vector<CZZ> mmultvec;
	vector<CZZ> mmultevec;
	vector<CZZ> mmultmsvec;


	for (long i = 0; i < slots; ++i) {
		m1vec.push_back(m1);
		m2vec.push_back(m2);
		maddvec.push_back(m1vec[i] + m2vec[i]);
		mmultvec.push_back(m1vec[i] * m2vec[i]);
		mmultevec.push_back(m1vec[i] * m2vec[i]);
		mmultmsvec.push_back(m1vec[i] * m2vec[i] / params.p);
	}

	vector<CZZ> m1conj = scheme.conj(m1vec);
	vector<CZZ> m2conj = scheme.conj(m2vec);
	vector<CZZ> maddconj = scheme.conj(maddvec);
	vector<CZZ> mmultconj = scheme.conj(mmultvec);
	vector<CZZ> mmulteconj = scheme.conj(mmultevec);
	vector<CZZ> mmultmsconj = scheme.conj(mmultmsvec);

	Message msg1 = scheme.encode(m1conj);
	Message msg2 = scheme.encode(m2conj);
	Message msgadd = scheme.encode(maddconj);
	Message msgmult = scheme.encode(mmultconj);
	Message msgmulte = scheme.encode(mmulteconj);
	Message msgmultms = scheme.encode(mmultmsconj);

	Cipher cipher1 = scheme.encrypt(msg1);
	Cipher cipher2 = scheme.encrypt(msg2);
	Cipher cmulte = scheme.encrypt(msg1);
	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher cadd = scheme.add(cipher1, cipher2);
	timeutils.stop("add");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult");
	Cipher cmult = scheme.mult(cipher1, cipher2);
	timeutils.stop("mult");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and equal");
	scheme.multAndEqual(cmulte, cipher2);
	timeutils.stop("mult and equal");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and mod switch");
	Cipher cmultms = scheme.multAndModSwitch(cipher1, cipher2);
	timeutils.stop("mult and mod switch");
	cout << "------------------" << endl;

	Message dmsg1 = scheme.decrypt(cipher1);
	Message dmsg2 = scheme.decrypt(cipher2);
	Message dmsgadd = scheme.decrypt(cadd);
	Message dmsgmult = scheme.decrypt(cmult);
	Message dmsgmulte = scheme.decrypt(cmulte);
	Message dmsgmultms = scheme.decrypt(cmultms);

	Cipher cmultx = scheme.encrypt(msgmult);
	Message dmsgmultx = scheme.decrypt(cmultx);
	vector<CZZ> dconjx = scheme.decode(dmsgmultx);
	ZZX polymultx;
	Ring2Utils::mult(polymultx, msg1.poly, msg2.poly, params.q, params.n);
	Message msgmultx(polymultx, msg1.logSlots);
	vector<CZZ> mconjx = scheme.decode(msgmultx);

	vector<CZZ> d1conj = scheme.decode(dmsg1);
	vector<CZZ> d2conj = scheme.decode(dmsg2);
	vector<CZZ> daddconj = scheme.decode(dmsgadd);
	vector<CZZ> dmultconj = scheme.decode(dmsgmult);
	vector<CZZ> dmulteconj = scheme.decode(dmsgmulte);
	vector<CZZ> dmultmsconj = scheme.decode(dmsgmultms);

	vector<CZZ> d1vec = scheme.dconj(d1conj);
	vector<CZZ> d2vec = scheme.dconj(d2conj);
	vector<CZZ> daddvec = scheme.dconj(daddconj);
	vector<CZZ> dmultvec = scheme.dconj(dmultconj);
	vector<CZZ> dmultevec = scheme.dconj(dmulteconj);
	vector<CZZ> dmultmsvec = scheme.dconj(dmultmsconj);

	StringUtils::show(m1conj);
	StringUtils::show(d1conj);

	StringUtils::show(m2conj);
	StringUtils::show(d2conj);

	StringUtils::show(mmultconj);
	StringUtils::show(dconjx);

	StringUtils::show(dmultconj);
	StringUtils::show(mconjx);

	cout << "------------------" << endl;
	cout << "m1:  " << m1vec[0].toString() << endl;
	cout << "d1:  " << d1vec[0].toString() << endl;
	cout << "msg:  " << msg1.poly << endl;
	cout << "dmsg:  " << dmsg1.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "m2:  " << m2vec[0].toString() << endl;
	cout << "d2:  " << d2vec[0].toString() << endl;
	cout << "msg2:  " << msg2.poly << endl;
	cout << "dmsg2:  " << dmsg2.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "madd:  " << maddvec[0].toString() << endl;
	cout << "dadd:  " << daddvec[0].toString() << endl;
	cout << "msgadd:  " << msgadd.poly << endl;
	cout << "dmsgadd:  " << dmsgadd.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmult:  " << mmultvec[0].toString() << endl;
	cout << "dmult:  " << dmultvec[0].toString() << endl;
	cout << "msgmult:  " << msgmult.poly << endl;
	cout << "msgmultx:  " << msgmultx.poly << endl;
	cout << "dmsgmult:  " << dmsgmult.poly << endl;
	cout << "dmsgmultx:  " << dmsgmultx.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmulte:  " << mmultevec[0].toString() << endl;
	cout << "dmulte:  " << dmultevec[0].toString() << endl;
//	cout << "msgmulte:  " << msgmulte.poly << endl;
//	cout << "dmsgmulte:  " << dmsgmulte.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmultms:  " << mmultmsvec[0].toString() << endl;
	cout << "dmultms:  " << dmultmsvec[0].toString() << endl;
//	cout << "msgmultms:  " << msgmultms.poly << endl;
//	cout << "dmsgmultms:  " << dmsgmultms.poly << endl;
	cout << "------------------" << endl;

	cout << "!!! STOP TEST OPERATIONS !!!" << endl;

}

void testPow() {
	cout << "!!! START TEST POW !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 15;
	long logl = 1;
	long logp = 56;
	long L = 11;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long logN = 5;
	long deg = 10;

	vector<CZZ> m2k;
	vector<CZZ> d2k;
	vector<Cipher> c2k;
	CZZ e, m, m2;

	m = params.cksi.pows[logN][2];
	m2k.push_back(m);
	for (long i = 0; i < deg; ++i) {
		m2 = (m2k[i] * m2k[i]) >> params.logp;
		m2k.push_back(m2);
	}

	vector<CZZ> mconj = scheme.conj(m);
	Message msg = scheme.encode(mconj);
	Cipher c = scheme.encrypt(msg);

	cout << "------------------" << endl;
	timeutils.start("Power of 2");
	algo.powerOf2(c2k, c, deg);
	timeutils.stop("Power of 2");
	cout << "------------------" << endl;

	for (long i = 0; i < c2k.size(); ++i) {
		Message dmsg = scheme.decrypt(c2k[i]);
		vector<CZZ> ddconj = scheme.decode(dmsg);
		vector<CZZ> dvec = scheme.dconj(ddconj);
		d2k.push_back(dvec[0]);
	}

	for (long i = 0; i < deg + 1; ++i) {
		e = m2k[i] - d2k[i];
		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i].toString() << endl;
		cout << "d: " << i << " " << d2k[i].toString() << endl;
		cout << "e: " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POW !!!" << endl;
}

void testProd2() {
	cout << "!!! START TEST PROD !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 15;
	long logl = 3;
	long logp = 56;
	long L = 11;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long deg = 6;
	long size = 1 << deg;
	long logN = 5;

	CZZ m, e;
	vector<CZZ> ms;
	vector<vector<CZZ>> ms2k;
	vector<vector<CZZ>> ds2k;
	vector<Cipher> cs;
	vector<vector<Cipher>> cs2k;

	for (long i = 0; i < size; ++i) {
		m = params.cksi.pows[logN][i % 3];
		vector<CZZ> mconj = scheme.conj(m);
		Message msg = scheme.encode(mconj);
		Cipher c = scheme.encrypt(msg);
		ms.push_back(m);
		cs.push_back(c);
	}

	ms2k.push_back(ms);
	for (long i = 1; i < deg + 1; ++i) {
		vector<CZZ> m2k;
		long idx = 0;
		long m2ksize = ms2k[i-1].size();
		while(idx < m2ksize) {
			CZZ m2 = (ms2k[i - 1][idx] * ms2k[i - 1][idx+1]) >> params.logp;
			m2k.push_back(m2);
			idx += 2;
		}
		ms2k.push_back(m2k);
	}

	cout << "------------------" << endl;
	timeutils.start("Prod 2");
	algo.prod2(cs2k, cs, deg);
	timeutils.stop("Prod 2");
	cout << "------------------" << endl;

	for (long i = 0; i < deg + 1; ++i) {
		vector<CZZ> d2k;
		for (long j = 0; j < cs2k[i].size(); ++j) {
			Message dmsg = scheme.decrypt(cs2k[i][j]);
			vector<CZZ> dconj = scheme.decode(dmsg);
			vector<CZZ> dvec = scheme.dconj(dconj);
			d2k.push_back(dvec[0]);
		}
		ds2k.push_back(d2k);
	}

	for (long i = 0; i < deg + 1; ++i) {
		for (long j = 0; j < cs2k[i].size(); ++j) {
			e = ms2k[i][j] - ds2k[i][j];
			cout << "------------------" << endl;
			cout << "m: " << i << " " << j << " " << ms2k[i][j].toString() << endl;
			cout << "d: " << i << " " << j << " " << ds2k[i][j].toString() << endl;
			cout << "e: " << i << " " << j << " " << e.toString() << endl;
//			cout << "eBnds: " << i << " " << j << " " << cs2k[i][j].eBnd << endl;
//			cout << "mBnds: " << i << " " << j << " " << cs2k[i][j].mBnd << endl;
			cout << "------------------" << endl;
		}
	}
	cout << "!!! END TEST PROD !!!" << endl;
}

void testInv() {
	cout << "!!! START TEST INV !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 2;
	long logp = 25;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long error = 13;
	long r = 6;

	ZZ halfp;
	CZZ m, mbar, minv, e;
	vector<CZZ> d2k;
	vector<Cipher> c2k, v2k;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;
	halfp = params.p / 2;

	vector<CZZ> mbarconj = scheme.conj(mbar);
	Message msgbar = scheme.encode(mbarconj);

	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(msgbar);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("Inverse");
	algo.inverse(c2k, v2k, c, r);
	timeutils.stop("Inverse");
	cout << "------------------" << endl;

	for (long i = 0; i < v2k.size(); ++i) {
		Message dmsg = scheme.decrypt(v2k[i]);
		vector<CZZ> dconj = scheme.decode(dmsg);
		vector<CZZ> dvec = scheme.dconj(dconj);
		d2k.push_back(dvec[0]);
	}

	for (long i = 0; i < r-1; ++i) {
		e = minv - d2k[i];
		cout << "------------------" << endl;
		cout << "minv:  " << i << " " << minv.toString() << endl;
		cout << "ds:    " << i << " " << d2k[i].toString() << endl;
		cout << "es:    " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << v2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << v2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST INV !!!" << endl;
}

void testFFT() {
	cout << "!!! START TEST FFT !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 4;
	long logp = 30;
	long L = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long logN = 4;
	long N = 1 << logN;
	long deg = 5;

	CZZ zero, tmp;

	vector<CZZ> mp1, mp2, mpx;
	vector<CZZ> mfft1, mfft2, mfftx;
	vector<Cipher> cp1, cp2, cpx;
	vector<Cipher> cfft1, cfft2;
	vector<Cipher> cfftbut1, cfftbut2, cpxbut;
	vector<CZZ> dpx;
	vector<CZZ> dpxbut;

	for (long i = 0; i < deg; ++i) {
		mp1.push_back(params.cksi.pows[logN][i]);
		mp2.push_back(params.cksi.pows[logN][i]);
	}

	for (long i = deg; i < N / 2 - 1; ++i) {
		mp1.push_back(zero);
		mp2.push_back(zero);
	}

	mp1 = scheme.conj(mp1);
	mp2 = scheme.conj(mp2);

	mfft1 = NumUtils::fft(mp1, params.cksi);
	mfft2 = NumUtils::fft(mp2, params.cksi);

	for (long i = 0; i < N; ++i) {
		tmp = (mfft1[i] * mfft2[i]) >> params.logp;
		mfftx.push_back(tmp);
	}

	mpx = NumUtils::fftInv(mfftx, params.cksi);

	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < N; ++i) {
		vector<CZZ> mconj1 = scheme.conj(mp1[i]);
		vector<CZZ> mconj2 = scheme.conj(mp2[i]);

		Message msgp1 = scheme.encode(mconj1);
		Message msgp2 = scheme.encode(mconj2);

		Cipher c1 = scheme.encrypt(msgp1);
		Cipher c2 = scheme.encrypt(msgp2);

		cp1.push_back(c1);
		cp2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 1");
	cfft1 = algo.fft(cp1);
	timeutils.stop("cfft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 2");
	cfft2 = algo.fft(cp2);
	timeutils.stop("cfft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft but 1");
	cfftbut1 = algo.fftButterfly(cp1);
	timeutils.stop("cfft but 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft but 2");
	cfftbut2 = algo.fftButterfly(cp2);
	timeutils.stop("cfft but 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul fft");
	for (long i = 0; i < N; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("mul fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul fft but");
	for (long i = 0; i < N; ++i) {
		scheme.multModSwitchAndEqual(cfftbut1[i], cfftbut2[i]);
	}
	timeutils.stop("mul fft but");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftx inv");
	cpx = algo.fftInv(cfft1);
	timeutils.stop("cfftx inv");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftx inv");
	cpxbut = algo.fftButterflyInv(cfftbut1);
	timeutils.stop("cfftx inv");
	cout << "------------------" << endl;

	for (long i = 0; i < N; ++i) {
		Message dx = scheme.decrypt(cpx[i]);
		vector<CZZ> dconj = scheme.decode(dx);
		vector<CZZ> dmp = scheme.dconj(dconj);
		dpx.push_back(dmp[0]);
	}

	for (long i = 0; i < N; ++i) {
		Message dx = scheme.decrypt(cpxbut[i]);
		vector<CZZ> dconj = scheme.decode(dx);
		vector<CZZ> dmp = scheme.dconj(dconj);
		dpxbut.push_back(dmp[0]);
	}

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: mpx    = " << mpx[i].toString() << endl;
		cout << i << " step: dpx    = " << dpx[i].toString() << endl;
		cout << i << " step: dpxbut = " << dpxbut[i].toString() << endl;

		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl;

}

int main() {
//	----------------------------
//	testtest();
//	testDumb();
//	testEncode();
//	testOperations();
//	testPow();
//	testProd2();
//	testInv();
	testFFT();
//	----------------------------

	return 0;
}
