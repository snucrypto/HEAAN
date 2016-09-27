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
#include "utils/CPolyRingUtils.h"
#include "utils/TimeUtils.h"

using namespace std;
using namespace NTL;

void testPow() {
	cout << "!!! START TEST POW !!!" << endl; // prints !!!Hello World!!!

	long i;
	TimeUtils timeutils;

	long deg = 5;
	long L = 5;
	long n = 1 << 12;
	long logp = 30;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long error = 6;

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
	m.r -= RandomBits_ZZ(error);

	vector<CZZ> m2k;


	m2k.push_back(m);

	for (i = 1; i < deg; ++i) {
		m2k.push_back((m2k[i-1] * m2k[i-1]) >> params.logp);
	}

	vector<Cipher> c2k;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m);
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

		ZZ qi = scheme.getQi(c2k[i].level);
		ds.i %= qi;
		ds.r %= qi;

		if(2 * ds.i > params.p) ds.i -= qi;
		if(2 * ds.r < params.p) ds.r += qi;

		d2k.push_back(ds);
	}

	vector<CZZ> e2k;

	for (i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;
		e2k.push_back(m2k[i] - d2k[i]);
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << m2k[i].toString() << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e2k[i].toString() << endl;
		cout << "Bs: " << i << " " << c2k[i].B << endl;
		cout << "ns: " << i << " " << c2k[i].nu << endl;
		cout << "------------------" << endl;
	}
	cout << "!!! END TEST POW !!!" << endl; // prints !!!Hello World!!!
}

void testInv() {
	cout << "!!! START TEST Inv !!!" << endl; // prints !!!Hello World!!!

	long i;
	TimeUtils timeutils;

	long deg = 5;
	long L = 5;
	long n = 1 << 12;
	long logp = 30;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	long error = 6;

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
	m.r -= RandomBits_ZZ(error);

	vector<CZZ> m2k;


	m2k.push_back(m);

	for (i = 1; i < deg; ++i) {
		m2k.push_back((m2k[i-1] * m2k[i-1]) >> params.logp);
	}

	vector<Cipher> c2k;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m);
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

		ZZ qi = scheme.getQi(c2k[i].level);
		ds.i %= qi;
		ds.r %= qi;

		if(2 * ds.i > params.p) ds.i -= qi;
		if(2 * ds.r < params.p) ds.r += qi;

		d2k.push_back(ds);
	}

	vector<CZZ> e2k;

	for (i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;
		e2k.push_back(m2k[i] - d2k[i]);
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << m2k[i].toString() << endl;
		cout << "ds: " << i << " " << d2k[i].toString() << endl;
		cout << "es: " << i << " " << e2k[i].toString() << endl;
		cout << "Bs: " << i << " " << c2k[i].B << endl;
		cout << "ns: " << i << " " << c2k[i].nu << endl;
		cout << "------------------" << endl;
	}
	cout << "!!! END TEST Inv !!!" << endl; // prints !!!Hello World!!!
}

void testFFT() {
	cout << "!!! START TEST FFT !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

	long L = 10;
	long n = 1 << 14;
	long logp = 30;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;

	long logN = 3;
	long N = 1 << logN;

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



	vector<CZZ> poly1;
	vector<CZZ> poly2;

	for (i = 0; i < N; ++i) {
		CZZ m1;
		CZZ m2;
		RandomBits(m1.r, 3);
		RandomBits(m2.r, 3);
		m1.r = params.p - m1.r;
		m2.r = params.p - m2.r;

		poly1.push_back(m1);
		poly2.push_back(m2);
	}

	vector<Cipher> cpoly1;
	vector<Cipher> cpoly2;

	timeutils.start("Encrypting polynomials");
	for (i = 0; i < N; ++i) {
		Cipher c1 = scheme.encrypt(poly1[i]);
		Cipher c2 = scheme.encrypt(poly2[i]);
		cpoly1.push_back(c1);
		cpoly2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");

	vector<Ksi> ksis;

	timeutils.start("making ksi");
	for (i = 0; i < logN + 1; ++i) {
		Ksi ksi((1 << i), params.logp);
		ksis.push_back(ksi);
	}
	timeutils.stop("making ksi");

	timeutils.start("fft 1");
	vector<Cipher> cfft1 = scheme.fft(cpoly1, ksis);
	timeutils.stop("fft 1");

	timeutils.start("fft 2");
	vector<Cipher> cfft2 = scheme.fft(cpoly2, ksis);
	timeutils.stop("fft 2");

	timeutils.start("mul and decrypt fft");
	vector<CZZ> dfft;
	for (i = 0; i < cfft1.size(); ++i) {
		Cipher ms = scheme.mulAndModSwitch(cfft1[i], cfft2[i]);
		CZZ d = scheme.decrypt(ms);
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

	CPolyRingUtils::mulPoly(zpoly, zpoly1, zpoly2, N);

	vector<CZZ> poly;

	for (i = 0; i < N; ++i) {
		poly.push_back(coeff(zpoly, i));
	}

	vector<CZZ> fft = scheme.fft(poly, ksis);

	for (i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: fft  = " << fft[i].toString() << endl;
		cout << i << " step: dfft = " << dfft[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl; // prints !!!Hello World!!!
}



int main() {
//	testFFT();
//	testInv();
	testPow();
	return 0;
}
