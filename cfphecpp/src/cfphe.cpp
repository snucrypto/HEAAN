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
#include "utils/TimeUtils.h"

using namespace std;
using namespace NTL;

void testtest() {
	CKsi cksi;
	cksi.setLogp(30);
	cksi.precompute(4);
	long i;
	long logSlots = 3;
	long slots = 1 << logSlots;
	vector<CZZ> mvec;

	for (i = 0; i < slots; ++i) {
		CZZ m;
		m = cksi.pows[3][i % 3];
		mvec.push_back(m);
	}

	vector<CZZ> fvec = NumUtils::fft(mvec, cksi, -1);
	vector<CZZ> dvec = NumUtils::fft(fvec, cksi, 1);

	for (i = 0; i < slots; ++i) {
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
	}

}

void testEncode() {
	TimeUtils timeutils;
	long n = 1 << 13;
	long logp = 30;
	long L = 1;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long logSlots = 3;
	Params params(n, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);

	long slots = (1 << logSlots);
	vector<CZZ> mvec;
	long i;
	params.cksi.precompute(4);

	for (i = 0; i < slots; ++i) {
		CZZ m;
		m = params.cksi.pows[3][i % 3];
		mvec.push_back(m);
	}

	cout << "---------------" << endl;
	Cipher cipher = scheme.encrypt(logSlots, mvec, scheme.params.p);

	vector<CZZ> dvec = scheme.decrypt(logSlots, cipher);

	for (i = 0; i < slots; ++i) {
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
	}
}

void testSimple() {

	cout << "!!! START TEST SIMPLE !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long n = 1 << 14;
	long logp = 50;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	cout << "------------------" << endl;
	timeutils.start("GenParams");
	Params params(n, logp, L, sigma, rho, h);
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
	long n = 1 << 13;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(n, logp, L, sigma, rho, h);
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
	long n = 1 << 12;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(n, logp, L, sigma, rho, h);
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
	long n = 1 << 14;
	long logp = 50;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(n, logp, L, sigma, rho, h);
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

void testFFT() {
	cout << "!!! START TEST FFT !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long n = 1 << 14;
	long logp = 30;
	long L = 8;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(n, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme, timeutils);
	//----------------------------

	long deg = 2;
	long logN = 3;
	long N = 1 << logN;


	params.cksi.precompute(logN+1);

	vector<CZZ> px, p1, p2;

	for (long i = 0; i < deg; ++i) {
		CZZ m1 = params.cksi.pows[logN][1];
		CZZ m2 = params.cksi.pows[logN][1];
		p1.push_back(m1);
		p2.push_back(m2);
	}

	CZZ zero;
	for (long i = deg; i < N; ++i) {
		p1.push_back(zero);
		p2.push_back(zero);
	}

	CZZX zp, zp1, zp2;

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

	vector<Cipher> cp1, cp2;

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
	timeutils.start("fft 1");
	vector<Cipher> cfft1 = algo.fft(cp1, params.cksi, 1);
	timeutils.stop("fft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft 2");
	vector<Cipher> cfft2 = algo.fft(cp2, params.cksi, 1);
	timeutils.stop("fft 2");
	cout << "------------------" << endl;

	vector<CZZ> dfft, mfft, rfft;

	cout << "------------------" << endl;
	timeutils.start("mul and decrypt fft");
	for (long i = 0; i < cfft1.size(); ++i) {
		Cipher ms = scheme.multAndModSwitch(cfft1[i], cfft2[i]);
		CZZ d = scheme.decrypt(ms);
		dfft.push_back(d);
	}
	timeutils.stop("mul and decrypt fft");
	cout << "------------------" << endl;

	vector<CZZ> fft1 = NumUtils::fft(p1, params.cksi, 1);
	vector<CZZ> fft2 = NumUtils::fft(p2, params.cksi, 1);

	for (long i = 0; i < N; ++i) {
		CZZ mm = fft1[i] * fft2[i];
		mm.r /= scheme.params.p;
		mm.i /= scheme.params.p;
		mfft.push_back(mm);
	}

	vector<CZZ> fft = NumUtils::fft(px, params.cksi, 1);
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

int main() {
//	testtest();
	testEncode();
//	testSimple();
//	testPow();
//	testProd();
//	testInv();
//	testFFT();
	return 0;
}
