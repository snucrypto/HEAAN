#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "scheme/Cipher.h"
#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SecKey.h"
#include "utils/Ring2Utils.h"
#include "utils/TimeUtils.h"

using namespace std;
using namespace NTL;

void testSimple() {

	cout << "!!! START TEST SIMPLE !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

	long L = 6;
	long n = 1 << 14;
	long logp = 51;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;

	long scale = 2;

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

	Ksi ksi(16, params.logp);
	CZZ m1;
	CZZ m2;
	m1 = ksi.pows[1];

	m2 = m1 * m1;
	m2.r >>= params.logp;
	m2.i >>= params.logp;

	timeutils.start("enc");
	Cipher c1 = scheme.encrypt(m1, params.p);
	timeutils.stop("enc");



	Cipher c2 = scheme.squareAndModSwitch(c1);

	timeutils.start("mul by const");
	Cipher s1 = scheme.mulByConstant(c1, m1);
	timeutils.stop("mul by const");

	Cipher s2 = scheme.modSwitch(s1, s1.level + scale);

	timeutils.start("add");
	Cipher x1 = scheme.add(c1, c1);
	timeutils.stop("add");

	timeutils.start("mult");
	Cipher y1 = scheme.mul(c1, c1);
	timeutils.stop("mult");


	timeutils.start("mul by const");
	Cipher t1 = scheme.mulByConstant(s2, m1);
	timeutils.stop("mul by const");

	timeutils.start("add");
	Cipher x2 = scheme.add(s2, s2);
	timeutils.stop("add");

	timeutils.start("mult");
	Cipher y2 = scheme.mul(s2, s2);
	timeutils.stop("mult");



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

	long i;
	TimeUtils timeutils;

	long deg = 11;
	long L = 11;
	long n = 1 << 15;
	long logp = 56;
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

	CZZ m;

	m.r = 1;
	m.r <<= params.logp;

	vector<Cipher> c2k;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m, params.p);
	timeutils.stop("Encrypt c");

	c2k.push_back(c);

	for (i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mul ");
		Cipher c2 = scheme.square(c2k[i - 1]);
		timeutils.stop("Mul ");

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2k.push_back(cs);
		cout << "------------------" << endl;
	}

	vector<CZZ> d2k;

	timeutils.start("Decrypt c");
	CZZ d = scheme.decrypt(c);
	timeutils.stop("Decrypt c");

	d2k.push_back(d);

	for (i = 1; i < deg; ++i) {
		CZZ ds = scheme.decrypt(c2k[i]);

//		ZZ qi = scheme.getQi(c2k[i].level);
//		ds.i %= qi;
//		ds.r %= qi;
//		if(ds.r < 0) ds.r += qi;
//		if(2 * ds.i > qi) ds.i -= qi;
//		if(2 * ds.r < params.p) ds.r += qi;

		d2k.push_back(ds);
	}

	for (i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;
		ZZ e = params.p - d2k[i].r;
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << params.p << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e << endl;
		cout << "Bs: " << i << " " << c2k[i].B << endl;
		cout << "ns: " << i << " " << c2k[i].nu << endl;
		cout << "------------------" << endl;
	}
	cout << "!!! END TEST POW !!!" << endl; // prints !!!Hello World!!!
}

void testProd() {
	cout << "!!! START TEST PROD !!!" << endl; // prints !!!Hello World!!!

	long i;
	TimeUtils timeutils;

	long deg = 5;
	long L = 5;
	long n = 1 << 12;
	long logp = 30;
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

	CZZ m;

	m.r = 1;
	m.r <<= params.logp;

	vector<Cipher> c2k;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m, params.p);
	timeutils.stop("Encrypt c");


	c2k.push_back(c);

	for (i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mul ");
		Cipher c2 = scheme.mul(c2k[i - 1], c2k[i - 1]);
		timeutils.stop("Mul ");

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2k.push_back(cs);

		cout << "------------------" << endl;
	}

	vector<CZZ> d2k;

	timeutils.start("Decrypt c");
	CZZ d = scheme.decrypt(c);
	timeutils.stop("Decrypt c");

	d2k.push_back(d);

	for (i = 1; i < deg; ++i) {
		CZZ ds = scheme.decrypt(c2k[i]);
		d2k.push_back(ds);
	}


	for (i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;
		ZZ e = params.p - d2k[i].r;
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << params.p << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e << endl;
		cout << "Bs: " << i << " " << c2k[i].B << endl;
		cout << "ns: " << i << " " << c2k[i].nu << endl;
		cout << "------------------" << endl;
	}
	cout << "!!! END TEST PROD !!!" << endl; // prints !!!Hello World!!!
}


void testInv() {
	cout << "!!! START TEST INV !!!" << endl; // prints !!!Hello World!!!

	long i;
	TimeUtils timeutils;

	long r = 6;
	long L = 6;
	long n = 1 << 14;
	long logp = 50;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long error = 13;

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

	CZZ m;
	CZZ mbar;
	CZZ minv;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;

	vector<Cipher> c2k;
	vector<Cipher> v2k;

	ZZ halfp;
	halfp = params.p / 2;

	CZZ czzp;
	czzp.r = params.p;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(mbar, halfp);
	timeutils.stop("Encrypt c");
	c2k.push_back(c);
	Cipher cp = scheme.addConstant(c, czzp);
	Cipher v = scheme.modEmbed(cp, cp.level + 1);
	v2k.push_back(v);

	for (i = 1; i < r-1; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mul ");
		Cipher c2 = scheme.square(c2k[i - 1]);
		timeutils.stop("Mul ");

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2k.push_back(cs);

		cout << "------------------" << endl;

		Cipher c2p = scheme.addConstant(cs, czzp);

		timeutils.start("Mul v");
		Cipher v2 = scheme.mul(v2k[i-1], c2p);
		timeutils.stop("Mul v");

		timeutils.start("MS v");
		Cipher vs = scheme.modSwitch(v2, i + 2);
		timeutils.stop("MS v");
		v2k.push_back(vs);

		cout << "------------------" << endl;
	}

	vector<CZZ> d2k;

	timeutils.start("Decrypt c");
	CZZ d = scheme.decrypt(v);
	timeutils.stop("Decrypt c");

	ZZ qi = scheme.getQi(v.level);
	d.i %= qi;
	d.r %= qi;

	if(2 * d.i > params.p) d.i -= qi;
	if(2 * d.r < params.p) d.r += qi;

	d2k.push_back(d);

	for (i = 1; i < r-1; ++i) {
		CZZ ds = scheme.decrypt(v2k[i]);
		d2k.push_back(ds);
	}

	vector<CZZ> e2k;

	for (i = 0; i < r-1; ++i) {
		cout << "---------" << i << "---------" << endl;
		e2k.push_back(minv - d2k[i]);
		cout << "------------------" << endl;
		cout << "minv: " << i << " " << minv.toString() << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e2k[i].toString() << endl;
		cout << "Bs: " << i << " " << v2k[i].B << endl;
		cout << "ns: " << i << " " << v2k[i].nu << endl;
		cout << "------------------" << endl;
	}
	cout << "!!! END TEST INV !!!" << endl; // prints !!!Hello World!!!
}

void testFFT() {
	cout << "!!! START TEST FFT !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

	long L = 6;
	long n = 1 << 14;
	long logp = 48;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long B = 64;

	long mulFactor = 20;


	long logN = 8;

	long N = 1 << logN;

	long deg = 3;
	long i;

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

	vector<Ksi> ksis;
	timeutils.start("making ksi");
	for (i = 0; i < logN + 1; ++i) {
		Ksi ksi((1 << i), params.logp);
		ksis.push_back(ksi);
	}
	timeutils.stop("making ksi");

	vector<CZZ> poly1;
	vector<CZZ> poly2;

	for (i = 0; i < deg; ++i) {
		CZZ m1 = ksis[3].pows[i];
		CZZ m2 = ksis[3].pows[deg-i];
		poly1.push_back(m1);
		poly2.push_back(m2);
	}

	CZZ zero;
	for (i = deg; i < N; ++i) {
		poly1.push_back(zero);
		poly2.push_back(zero);
	}

	vector<Cipher> cpoly1;
	vector<Cipher> cpoly2;

	timeutils.start("Encrypting polynomials");
	for (i = 0; i < N; ++i) {
		Cipher c1 = scheme.encrypt(poly1[i], params.p);
		Cipher c2 = scheme.encrypt(poly2[i], params.p);
		cpoly1.push_back(c1);
		cpoly2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");

	timeutils.start("fft 1");
	vector<Cipher> cfft1 = scheme.fft(cpoly1, ksis);
	timeutils.stop("fft 1");

	timeutils.start("fft 2");
	vector<Cipher> cfft2 = scheme.fft(cpoly2, ksis);
	timeutils.stop("fft 2");

	timeutils.start("mul and decrypt fft");
	vector<CZZ> dfft;
	for (i = 0; i < cfft1.size(); ++i) {
		Cipher ms = scheme.mul(cfft1[i], cfft2[i]);
		CZZ d = scheme.decrypt(ms);
		d.r /= params.p;
		d.i /= params.p;
		dfft.push_back(d);
	}
	timeutils.stop("mul and decrypt fft");


	CZZX zpoly1;
	CZZX zpoly2;
	CZZX zpoly;

	for (i = 0; i < N; ++i) {
		SetCoeff(zpoly1, i, poly1[i]);
		SetCoeff(zpoly2, i, poly2[i]);
	}
	zpoly1.normalize();
	zpoly2.normalize();
	zpoly = zpoly1 * zpoly2;

	vector<CZZ> poly;

	for (i = 0; i < N; ++i) {
		poly.push_back(coeff(zpoly, i));
	}

	vector<CZZ> fft1 = scheme.fft(poly1, ksis);
	vector<CZZ> fft2 = scheme.fft(poly2, ksis);

	vector<CZZ> mfft;
	for (i = 0; i < N; ++i) {
		CZZ mm = fft1[i] * fft2[i];
		mm.r /= params.p;
		mm.i /= params.p;
		mfft.push_back(mm);
	}

	vector<CZZ> fft = scheme.fft(poly, ksis);

	vector<CZZ> rfft;
	for (i = 0; i < N; ++i) {
		CZZ rm = fft[i];
		rm.r /= params.p;
		rm.i /= params.p;
		rfft.push_back(rm);
	}

	for (i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: rfft  = " << rfft[i].toString() << endl;
		cout << i << " step: mfft = " << mfft[i].toString() << endl;
		cout << i << " step: dfft = " << dfft[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl; // prints !!!Hello World!!!
}

void testFFTEasy() {
	cout << "!!! START TEST FFT !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

	long L = 6;
	long n = 1 << 14;
	long logp = 50;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long B = 64;
	long logN = 8;
	long N = 1 << logN;
	long logpprime = 32;
	ZZ pprime;
	power(pprime, 2, logpprime);
	long deg = 3;
	long i;

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

	vector<Ksi> ksis;
	timeutils.start("making ksi");
	for (i = 0; i < logN + 1; ++i) {
		Ksi ksi((1 << i), logpprime);
		ksis.push_back(ksi);
	}
	timeutils.stop("making ksi");

	vector<CZZ> poly1;
	vector<CZZ> poly2;

	for (i = 0; i < deg; ++i) {
		CZZ m1;
		m1.r = params.p;
		CZZ m2;
		m2.r = params.p;
		poly1.push_back(m1);
		poly2.push_back(m2);
	}

	CZZ zero;
	for (i = deg; i < N; ++i) {
		poly1.push_back(zero);
		poly2.push_back(zero);
	}

	vector<Cipher> cpoly1;
	vector<Cipher> cpoly2;

	timeutils.start("Encrypting polynomials");
	for (i = 0; i < N; ++i) {
		Cipher c1 = scheme.encrypt(poly1[i], params.p);
		Cipher c2 = scheme.encrypt(poly2[i], params.p);
		cpoly1.push_back(c1);
		cpoly2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");

	timeutils.start("fft 1");
	vector<Cipher> cfft1 = scheme.fftEasy(cpoly1, ksis, pprime, B);
	timeutils.stop("fft 1");

	timeutils.start("fft 2");
	vector<Cipher> cfft2 = scheme.fftEasy(cpoly1, ksis, pprime, B);
	timeutils.stop("fft 2");

	timeutils.start("mul and decrypt fft");

	for (i = 0; i < cfft1.size(); ++i) {
		scheme.modSwitchAndEqual(cfft1[i], cfft1[i].level + 2);
		scheme.modSwitchAndEqual(cfft2[i], cfft2[i].level + 2);
	}

	vector<CZZ> dfft;
	for (i = 0; i < cfft1.size(); ++i) {
		Cipher ms = scheme.mul(cfft1[i], cfft2[i]);
		CZZ d = scheme.decrypt(ms);
		d.r /= params.p;
		d.i /= params.p;
		dfft.push_back(d);
	}
	timeutils.stop("mul and decrypt fft");


	CZZX zpoly1;
	CZZX zpoly2;
	CZZX zpoly;

	for (i = 0; i < N; ++i) {
		SetCoeff(zpoly1, i, poly1[i]);
		SetCoeff(zpoly2, i, poly2[i]);
	}
	zpoly1.normalize();
	zpoly2.normalize();
	zpoly = zpoly1 * zpoly2;

	vector<CZZ> poly;

	for (i = 0; i < N; ++i) {
		poly.push_back(coeff(zpoly, i));
	}

	vector<CZZ> fft1 = scheme.fft(poly1, ksis);
	vector<CZZ> fft2 = scheme.fft(poly2, ksis);

	vector<CZZ> mfft;
	for (i = 0; i < N; ++i) {
		CZZ mm = fft1[i] * fft2[i];
		mm.r /= params.p;
		mm.i /= params.p;
		mfft.push_back(mm);
	}

	vector<CZZ> fft = scheme.fft(poly, ksis);

	vector<CZZ> rfft;
	for (i = 0; i < N; ++i) {
		CZZ rm = fft[i];
		rm.r /= params.p;
		rm.i /= params.p;
		rfft.push_back(rm);
	}

	for (i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: rfft  = " << rfft[i].toString() << endl;
		cout << i << " step: mfft = " << mfft[i].toString() << endl;
		cout << i << " step: dfft = " << dfft[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl; // prints !!!Hello World!!!
}


int main() {
//	testSimple();
	testPow();
//	testProd();
//	testInv();
//	testFFT();
//	testFFTEasy();
	return 0;
}
