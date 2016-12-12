#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>
#include <random>

#include "czz/CZZ.h"
#include "czz/CZZX.h"
#include "scheme/Cipher.h"
#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SchemeAlgo.h"
#include "scheme/SecKey.h"
#include "utils/NumUtils.h"
#include "utils/Ring2Utils.h"
#include "utils/TimeUtils.h"

using namespace std;
using namespace NTL;

void testMult() {
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long logN = 4;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);

	params.cksi.precompute(logN + 1);
	CZZ mx, m1, m2;
	m1 = params.cksi.pows[logN][2];
	Cipher c1 = scheme.encrypt(m1, scheme.params.p);

	m2 = params.cksi.pows[logN][2];
	Cipher c2 = scheme.encrypt(m2, scheme.params.p);

	mx = m1 * m2 / scheme.params.p;

	Cipher cx = scheme.mult(c1, c2);

	CZZ dx, dm, d1, d2;

	d1 = scheme.decrypt(c1);
	d2 = scheme.decrypt(c2);
	dm = d1 * d2;

	dx = scheme.decrypt(cx);

	cout << "m1: " << m1.toString() << endl;
	cout << "m2: " << m2.toString() << endl;
	cout << "mx: " << mx.toString() << endl;
	cout << "d1: " << d1.toString() << endl;
	cout << "d2: " << d2.toString() << endl;
	cout << "dm: " << dm.toString() << endl;
	cout << "dx: " << dx.toString() << endl;
}

void testtest() {
	CKsi cksi;
	cksi.setLogp(30);
	long i;
	long logSlots = 3;
	long slots = 1 << logSlots;
	vector<CZZ> mvec;

	cksi.precompute(logSlots + 1);
	for (i = 0; i < slots; ++i) {
		CZZ m;
		m = cksi.pows[logSlots][i % 3];
		mvec.push_back(m);
	}

	vector<CZZ> fvec = NumUtils::fftInv(mvec, cksi);
	vector<CZZ> dvec = NumUtils::fft(fvec, cksi);

	for (i = 0; i < slots; ++i) {
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
	}

}

void testEncodeAll() {
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);

	vector<CZZ> mvec;
	long i;
	params.cksi.precompute(params.logn + 1);

	for (i = 0; i < params.n; ++i) {
		CZZ m;
		m = params.cksi.pows[params.logn][i % 3];
		mvec.push_back(m);
	}


	vector<CZZ> fft = NumUtils::fftInv(mvec, params.cksi);
	vector<CZZ> fft2 = NumUtils::fft(fft, params.cksi);
	cout << "---------------" << endl;
	Cipher cipher = scheme.encryptAll(mvec, scheme.params.p);
	vector<CZZ> dvec = scheme.decryptAll(cipher);

	for (i = 0; i < params.n; ++i) {
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
		cout << "---------------------" << endl;

	}
}

void testEncode() {
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long logSlots = 4;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);

	long slots = (1 << logSlots);
	vector<CZZ> mvec;
	long i;
	params.cksi.precompute(logSlots + 1);

	for (i = 0; i < slots; ++i) {
		CZZ m;
		m = params.cksi.pows[logSlots][i % 3];
		mvec.push_back(m);
	}

	cout << "---------------" << endl;
	Cipher cipher = scheme.encrypt(logSlots, mvec, scheme.params.p);

	vector<CZZ> dvec = scheme.decrypt(logSlots, cipher);

	for (i = 0; i < slots; ++i) {
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
		cout << "---------------------" << endl;
	}
}

void testEncodeAndSquare() {
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long logSlots = 6;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);

	long slots = (1 << logSlots);
	vector<CZZ> mvec;
	long i;
	params.cksi.precompute(logSlots + 1);

	for (i = 0; i < slots; ++i) {
		CZZ m;
		m = params.cksi.pows[logSlots][i];
		mvec.push_back(m);
	}

	vector<CZZ> m2;
	for (i = 0; i < slots; ++i) {
		m2.push_back((mvec[i] * mvec[i]) >> params.logp);
	}

	cout << "---------------" << endl;
	Cipher cipher = scheme.encrypt(logSlots, mvec, scheme.params.p);
	Cipher c2 = scheme.squareAndModSwitch(cipher);
	vector<CZZ> d2 = scheme.decrypt(logSlots, c2);

	for (i = 0; i < slots; ++i) {
		cout << "mi: " << i << " : " << m2[i].toString() << endl;
		cout << "di: " << i << " : " << d2[i].toString() << endl;
		cout << "---------------------" << endl;
	}
}

void testEncodeAndMult() {
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long logSlots = 4;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);

	long i;
	long slots = (1 << logSlots);
	params.cksi.precompute(logSlots + 1);

	vector<CZZ> mvec1, mvec2;
	for (i = 0; i < slots; ++i) {
		CZZ m1, m2;
		m1 = params.cksi.pows[logSlots][1];
		m2 = params.cksi.pows[logSlots][i];
		mvec1.push_back(m1);
		mvec2.push_back(m2);
	}

	vector<CZZ> mmult;
	for (i = 0; i < slots; ++i) {
		mmult.push_back((mvec1[i] * mvec2[i]) >> params.logp);
	}

	cout << "---------------" << endl;
	Cipher c1 = scheme.encrypt(logSlots, mvec1, scheme.params.p);
	Cipher c2 = scheme.encrypt(logSlots, mvec2, scheme.params.p);
	Cipher cmult = scheme.multAndModSwitch(c1, c2);
	vector<CZZ> dmult = scheme.decrypt(logSlots, cmult);

	for (i = 0; i < slots; ++i) {
		cout << "mmulti: " << i << " : " << mmult[i].toString() << endl;
		cout << "dmulti: " << i << " : " << dmult[i].toString() << endl;
		cout << "---------------------" << endl;
	}
}

void testSimple() {

	cout << "!!! START TEST SIMPLE !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	cout << "------------------" << endl;
	timeutils.start("GenParams");
	Params params(logn, logp, L, sigma, rho, h);
	timeutils.stop("GenParams");
	cout << "------------------" << endl;
	timeutils.start("GenSecKey");
	SecKey secretKey(params);
	timeutils.stop("GenSecKey");
	cout << "------------------" << endl;
	timeutils.start("GenPubKey");
	PubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");
	cout << "------------------" << endl;
	timeutils.start("GenScheme");
	Scheme scheme(params, secretKey, publicKey);
	timeutils.stop("GenScheme");
	cout << "------------------" << endl;
	//----------------------------

	long scale = 2;
	params.cksi.precompute(4);
	CZZ m1, m2;

	m1 = params.cksi.pows[3][3];
	m2 = m1 * m1;
	m2.r >>= scheme.params.logp;
	m2.i >>= scheme.params.logp;

	cout << "------------------" << endl;
	timeutils.start("enc");
	Cipher c1 = scheme.encrypt(m1, scheme.params.p);
	timeutils.stop("enc");
	cout << "------------------" << endl;
	timeutils.start("dec");
	CZZ xxx = scheme.decrypt(c1);
	timeutils.stop("dec");
	cout << "------------------" << endl;
	Cipher c2 = scheme.squareAndModSwitch(c1);
	cout << "------------------" << endl;
	timeutils.start("mul by const");
	Cipher s1 = scheme.multByConstant(c1, m1);
	timeutils.stop("mul by const");
	cout << "------------------" << endl;
	Cipher s2 = scheme.modSwitch(s1, s1.level + scale);
	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher x1 = scheme.add(c1, c1);
	timeutils.stop("add");
	cout << "------------------" << endl;
	timeutils.start("mult");
	Cipher y1 = scheme.mult(c1, c1);
	timeutils.stop("mult");
	cout << "------------------" << endl;
	timeutils.start("mul by const");
	Cipher t1 = scheme.multByConstant(s2, m1);
	timeutils.stop("mul by const");
	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher x2 = scheme.add(s2, s2);
	timeutils.stop("add");
	cout << "------------------" << endl;
	timeutils.start("mult");
	Cipher y2 = scheme.mult(s2, s2);
	timeutils.stop("mult");
	cout << "------------------" << endl;

	CZZ d1 = scheme.decrypt(c1);
	CZZ d2 = scheme.decrypt(c2);
	CZZ v2 = scheme.decrypt(s2);

	CZZ e1 = d1 - m1;
	CZZ e2 = d2 - m2;
	CZZ f2 = v2 - m2;

	cout << "------------------" << endl;
	cout << "m1: " << m1.toString() << endl;
	cout << "d1: " << d1.toString() << endl;
	cout << "e1: " << e1.toString() << endl;
	cout << "d1 norm: " << d1.norm() << endl;
	cout << "------------------" << endl;
	cout << "m2: " << m2.toString() << endl;
	cout << "d2: " << d2.toString() << endl;
	cout << "e2: " << e2.toString() << endl;
	cout << "d2 norm: " << d2.norm() << endl;
	cout << "------------------" << endl;
	cout << "m2: " << m2.toString() << endl;
	cout << "v2: " << v2.toString() << endl;
	cout << "f2: " << f2.toString() << endl;
	cout << "v2 norm: " << v2.norm() << endl;
	cout << "------------------" << endl;

	cout << "!!! END TEST SIMPLE !!!" << endl; // prints !!!Hello World!!!
}

void testPow() {
	cout << "!!! START TEST POW !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long deg = 5;

	CZZ m;
	m.r = 1;
	m.r <<= scheme.params.logp;


	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m, scheme.params.p);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	vector<Cipher> c2k;
	algo.powerOf2(c2k, c, deg);

	vector<CZZ> d2k;
	scheme.decrypt(d2k, c2k);

	for (long i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;
		ZZ e = scheme.params.p - d2k[i].r;
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << scheme.params.p << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e << endl;
		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POW !!!" << endl; // prints !!!Hello World!!!
}

void testProd() {
	cout << "!!! START TEST PROD !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long deg = 5;

	CZZ m;
	m.r = 1;
	m.r <<= scheme.params.logp;

	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m, scheme.params.p);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	vector<Cipher> c2k;
	algo.prodOfSame(c2k, c, deg);

	vector<CZZ> d2k;
	scheme.decrypt(d2k, c2k);

	ZZ e;
	for (long i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;
		e = scheme.params.p - d2k[i].r;
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << scheme.params.p << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e << endl;
		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}
	cout << "!!! END TEST PROD !!!" << endl; // prints !!!Hello World!!!
}


void testInv() {
	cout << "!!! START TEST INV !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 14;
	long logp = 50;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long error = 13;
	long r = 6;
	CZZ m, mbar, minv, e;
	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= scheme.params.logp;
	m.r -= mbar.r;
	minv.r = scheme.params.p * scheme.params.p / m.r;
	ZZ halfp = scheme.params.p / 2;

	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(mbar, halfp);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	vector<Cipher> c2k;
	vector<Cipher> v2k;
	algo.inverse(c2k, v2k, c, r);

	vector<CZZ> d2k;
	scheme.decrypt(d2k, v2k);

	for (long i = 0; i < r-1; ++i) {
		cout << "---------" << i << "---------" << endl;
		e = minv - d2k[i];
		cout << "------------------" << endl;
		cout << "minv: " << i << " " << minv.toString() << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e.toString() << endl;
		cout << "eBnds: " << i << " " << v2k[i].eBnd << endl;
		cout << "mBnds: " << i << " " << v2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST INV !!!" << endl; // prints !!!Hello World!!!
}

void testSimpleFFT() {
	cout << "!!! START TEST SIMPLE FFT !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long deg = 3;
	long logN = 4;
	long N = 1 << logN;


	params.cksi.precompute(logN+1);
	vector<CZZ> px;

	for (long i = 0; i < deg; ++i) {
		px.push_back(params.cksi.pows[logN][i % 3]);
	}

	CZZ zero;
	for (long i = deg; i < N; ++i) {
		px.push_back(zero);
	}

	vector<Cipher> cpx;

	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < N; ++i) {
		Cipher cx = scheme.encrypt(px[i], scheme.params.p);
		cpx.push_back(cx);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft");
	vector<Cipher> cfftx = algo.fft(cpx, params.cksi);
	timeutils.stop("fft");
	cout << "------------------" << endl;

	vector<CZZ> dfft, rfft;

	cout << "------------------" << endl;
	timeutils.start("decrypt fft");
	for (long i = 0; i < cfftx.size(); ++i) {
		CZZ d = scheme.decrypt(cfftx[i]);
		dfft.push_back(d);
	}
	timeutils.stop("decrypt fft");
	cout << "------------------" << endl;

	vector<CZZ> fft = NumUtils::fft(px, params.cksi);
	for (long i = 0; i < N; ++i) {
		rfft.push_back(fft[i]);
	}

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: rfft  = " << rfft[i].toString() << endl;
		cout << i << " step: dfft = " << dfft[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST SIMPLE FFT !!!" << endl; // prints !!!Hello World!!!

}

void testFFT() {
	cout << "!!! START TEST FFT !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 8;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long logN = 5;
	long N = 1 << logN;
	long deg = 6;


	params.cksi.precompute(logN+1);

	CZZ zero;
	vector<CZZ> px, p1, p2;
	CZZX zp, zp1, zp2;
	vector<Cipher> cp1, cp2;
	vector<CZZ> dfft, mfft, rfft;


	for (long i = 0; i < deg; ++i) {
		CZZ m1 = params.cksi.pows[logN][i];
		CZZ m2 = params.cksi.pows[logN][i];
		p1.push_back(m1);
		p2.push_back(m2);
	}

	for (long i = deg; i < N; ++i) {
		p1.push_back(zero);
		p2.push_back(zero);
	}


	for (long i = 0; i < N; ++i) {
		SetCoeff(zp1, i, p1[i]);
		SetCoeff(zp2, i, p2[i]);
	}
	zp1.normalize();
	zp2.normalize();
	zp = zp1 * zp2;

	for (long i = 0; i < N; ++i) {
		px.push_back(coeff(zp, i));
	}


	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < N; ++i) {
		Cipher c1 = scheme.encrypt(p1[i], scheme.params.p);
		Cipher c2 = scheme.encrypt(p2[i], scheme.params.p);
		cp1.push_back(c1);
		cp2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 1");
	vector<Cipher> cfft1 = algo.fft(cp1, params.cksi);
	timeutils.stop("cfft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 2");
	vector<Cipher> cfft2 = algo.fft(cp2, params.cksi);
	timeutils.stop("cfft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft 1");
	vector<CZZ> fft1 = NumUtils::fft(p1, params.cksi);
	timeutils.stop("fft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft 2");
	vector<CZZ> fft2 = NumUtils::fft(p2, params.cksi);
	timeutils.stop("fft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul and decrypt fft");
	for (long i = 0; i < cfft1.size(); ++i) {
		cout << "-----------------" << endl;
		Cipher mx = scheme.mult(cfft1[i], cfft2[i]);
		Cipher ms = scheme.multAndModSwitch(cfft1[i], cfft2[i]);
		CZZ ds, dx, dm, d1, d2;
		d1 = scheme.decrypt(cfft1[i]);
		cout << "d1: " << NumBits(d1.r) << "  " << d1.toString() << endl;
		d2 = scheme.decrypt(cfft2[i]);
		cout << "d2: " << NumBits(d2.r) << "  " << d2.toString() << endl;
		dm = (d1 * d2) >> params.logp;
		cout << "dm: " << NumBits(dm.r) << "  " << dm.toString() << endl;
		dx = scheme.decrypt(mx);
		dx >>= params.logp;
		cout << "dx: " << NumBits(dx.r) << "  " << dx.toString() << endl;
		ds = scheme.decrypt(ms);
		cout << "dx: " << NumBits(ds.r) << "  " << ds.toString() << endl;
		dfft.push_back(ds);
	}
	timeutils.stop("mul and decrypt fft");
	cout << "------------------" << endl;


	for (long i = 0; i < N; ++i) {
		CZZ mm = fft1[i] * fft2[i];
		mm.r /= scheme.params.p;
		mm.i /= scheme.params.p;
		mfft.push_back(mm);
	}

	vector<CZZ> fft = NumUtils::fft(px, params.cksi);
	for (long i = 0; i < N; ++i) {
		CZZ rm = fft[i];
		rm.r /= scheme.params.p;
		rm.i /= scheme.params.p;
		rfft.push_back(rm);
	}

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: rfft  = " << rfft[i].toString() << endl;
		cout << i << " step: mfft = " << mfft[i].toString() << endl;
		cout << i << " step: dfft = " << dfft[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl; // prints !!!Hello World!!!
}

void testFullFFT() {
	cout << "!!! START TEST FULL FFT !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logp = 30;
	long L = 12;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long logN = 4;
	long N = 1 << logN;
	long deg = 5;


	params.cksi.precompute(logN+1);

	CZZ zero, tmp;

	vector<CZZ> mp1, mp2, mpx;
	vector<CZZ> mfft1, mfft2, mfftx;
	vector<Cipher> cp1, cp2, cpx;
	vector<Cipher> cfft1, cfft2, cfftx;
	vector<CZZ> dpx;

	for (long i = 0; i < deg; ++i) {
		mp1.push_back(params.cksi.pows[logN][i]);
		mp2.push_back(params.cksi.pows[logN][i]);
	}

	for (long i = deg; i < N; ++i) {
		mp1.push_back(zero);
		mp2.push_back(zero);
	}

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
		Cipher c1 = scheme.encrypt(mp1[i], scheme.params.p);
		Cipher c2 = scheme.encrypt(mp2[i], scheme.params.p);
		cp1.push_back(c1);
		cp2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 1");
	cfft1 = algo.fft(cp1, params.cksi);
	timeutils.stop("cfft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 2");
	cfft2 = algo.fft(cp2, params.cksi);
	timeutils.stop("cfft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul fft");
	for (long i = 0; i < N; ++i) {
		Cipher cfftxi = scheme.multAndModSwitch(cfft1[i], cfft2[i]);
		cfftx.push_back(cfftxi);
	}
	timeutils.stop("mul fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftx inv");
	cpx = algo.fftInv(cfftx, params.cksi);
	timeutils.stop("cfftx inv");
	cout << "------------------" << endl;

	for (long i = 0; i < N; ++i) {
		tmp = scheme.decrypt(cpx[i]);
		dpx.push_back(tmp);
	}

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: cpx  = " << mpx[i].toString() << endl;
		cout << i << " step: dpx = " << dpx[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FULL FFT !!!" << endl; // prints !!!Hello World!!!

}

int main() {
//	testMult();
//	testtest();
//	testEncodeAll();
//	testEncode();
//	testEncodeAndSquare();
//	testEncodeAndMult();
//	testSimple();
//	testPow();
//	testProd();
//	testInv();
//	testSimpleFFT();
//	testFFT();
	testFullFFT();
	return 0;
}
