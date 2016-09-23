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

void testInv() {
	cout << "!!! START TEST INVERSE !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

	long deg = 10;
	long L = 10;
	long n = 1 << 14;
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

	ZZ m;
	m = 1;
	m <<= params.logp;
//	m -= 3;


	vector<ZZ> m2k;
	vector<ZZ> m2ks;
	vector<ZZ> m2ke;

	long i;

	m2k.push_back(m);
	m2ks.push_back(m);
	m2ke.push_back(m);

	for (i = 1; i < deg; ++i) {
		m2k.push_back(m2ks[i-1] * m2ks[i-1]);
		m2ks.push_back(m2k[i] >> params.logp);
		m2ke.push_back(m2k[i]);
	}

	vector<Cipher> c2k;
	vector<Cipher> c2ks;
	vector<Cipher> c2ke;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m);
	timeutils.stop("Encrypt c");

	c2k.push_back(c);
	c2ks.push_back(c);
	c2ke.push_back(c);

	for (i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mul ");
		Cipher c2 = scheme.mul(c2ks[i - 1], c2ks[i - 1]);
		timeutils.stop("Mul ");
		c2k.push_back(c2);

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2ks.push_back(cs);

		cout << "------------------" << endl;

		timeutils.start("ME ");
		Cipher ce = scheme.modEmbed(c2, i + 1);
		timeutils.stop("ME ");
		c2ke.push_back(ce);

		cout << "------------------" << endl;
	}


	vector<ZZ> d2k;
	vector<ZZ> d2ks;
	vector<ZZ> d2ke;

	timeutils.start("Decrypt c");
	ZZ d = scheme.decrypt(c);
	timeutils.stop("Decrypt c");

	d2k.push_back(d);
	d2ks.push_back(d);
	d2ke.push_back(d);

	for (i = 1; i < deg; ++i) {
		ZZ d2 = scheme.decrypt(c2k[i]);
		d2k.push_back(d2);

		ZZ ds = scheme.decrypt(c2ks[i]);
		d2ks.push_back(ds);

		ZZ de = scheme.decrypt(c2ke[i]);
		d2ke.push_back(de);
	}

	vector<ZZ> e2k;
	vector<ZZ> e2ks;
	vector<ZZ> e2ke;

	for (i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		e2k.push_back(m2k[i] - d2k[i]);
		e2ks.push_back(m2ks[i] - d2ks[i]);
		e2ke.push_back(m2ke[i] - d2ke[i]);

		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i] << endl;
		cout << "d: " << i << " " << d2k[i] << endl;
		cout << "e: " << i << " " << e2k[i] << endl;

		cout << "B: " << i << " " << c2k[i].B << endl;
		cout << "n: " << i << " " << c2k[i].nu << endl;

		cout << "------------------" << endl;
		cout << "ms: " << i << " " << m2ks[i] << endl;
		cout << "ds: " << i << " " << d2ks[i] << endl;
		cout << "es: " << i << " " << e2ks[i] << endl;

		cout << "Bs: " << i << " " << c2ks[i].B << endl;
		cout << "ns: " << i << " " << c2ks[i].nu << endl;

		cout << "------------------" << endl;
		cout << "me: " << i << " " << m2ke[i] << endl;
		cout << "de: " << i << " " << d2ke[i] << endl;
		cout << "ee: " << i << " " << e2ke[i] << endl;

		cout << "Be: " << i << " " << c2ke[i].B << endl;
		cout << "ne: " << i << " " << c2ke[i].nu << endl;

		cout << "------------------" << endl;
	}

	cout << params.logq << endl;

	cout << "!!! END TEST INVERSE !!!" << endl; // prints !!!Hello World!!!
}

void test1() {
	cout << "!!! START TEST 1 !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

	long deg = 10;
	long L = 10;
	long n = 1 << 14;
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

	ZZ m;
	m = 1;
	m <<= params.logp;


	vector<ZZ> m2k;
	vector<ZZ> m2ks;
	vector<ZZ> m2ke;

	long i;

	m2k.push_back(m);
	m2ks.push_back(m);
	m2ke.push_back(m);

	for (i = 1; i < deg; ++i) {
		m2k.push_back(m2ks[i-1] * m2ks[i-1]);
		m2ks.push_back(m2k[i] >> params.logp);
		m2ke.push_back(m2k[i]);
	}

	vector<Cipher> c2k;
	vector<Cipher> c2ks;
	vector<Cipher> c2ke;

	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(m);
	timeutils.stop("Encrypt c");

	c2k.push_back(c);
	c2ks.push_back(c);
	c2ke.push_back(c);

	for (i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mul ");
		Cipher c2 = scheme.mul(c2ks[i - 1], c2ks[i - 1]);
		timeutils.stop("Mul ");
		c2k.push_back(c2);

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2ks.push_back(cs);

		cout << "------------------" << endl;

		timeutils.start("ME ");
		Cipher ce = scheme.modEmbed(c2, i + 1);
		timeutils.stop("ME ");
		c2ke.push_back(ce);

		cout << "------------------" << endl;
	}


	vector<ZZ> d2k;
	vector<ZZ> d2ks;
	vector<ZZ> d2ke;

	timeutils.start("Decrypt c");
	ZZ d = scheme.decrypt(c);
	timeutils.stop("Decrypt c");

	d2k.push_back(d);
	d2ks.push_back(d);
	d2ke.push_back(d);

	for (i = 1; i < deg; ++i) {
		ZZ d2 = scheme.decrypt(c2k[i]);
		d2k.push_back(d2);

		ZZ ds = scheme.decrypt(c2ks[i]);
		d2ks.push_back(ds);

		ZZ de = scheme.decrypt(c2ke[i]);
		d2ke.push_back(de);
	}

	vector<ZZ> e2k;
	vector<ZZ> e2ks;
	vector<ZZ> e2ke;

	for (i = 0; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		e2k.push_back(m2k[i] - d2k[i]);
		e2ks.push_back(m2ks[i] - d2ks[i]);
		e2ke.push_back(m2ke[i] - d2ke[i]);

		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i] << endl;
		cout << "d: " << i << " " << d2k[i] << endl;
		cout << "e: " << i << " " << e2k[i] << endl;

		cout << "B: " << i << " " << c2k[i].B << endl;
		cout << "n: " << i << " " << c2k[i].nu << endl;

		cout << "------------------" << endl;
		cout << "ms: " << i << " " << m2ks[i] << endl;
		cout << "ds: " << i << " " << d2ks[i] << endl;
		cout << "es: " << i << " " << e2ks[i] << endl;

		cout << "Bs: " << i << " " << c2ks[i].B << endl;
		cout << "ns: " << i << " " << c2ks[i].nu << endl;

		cout << "------------------" << endl;
		cout << "me: " << i << " " << m2ke[i] << endl;
		cout << "de: " << i << " " << d2ke[i] << endl;
		cout << "ee: " << i << " " << e2ke[i] << endl;

		cout << "Be: " << i << " " << c2ke[i].B << endl;
		cout << "ne: " << i << " " << c2ke[i].nu << endl;

		cout << "------------------" << endl;
	}

	cout << params.logq << endl;

	cout << "!!! END TEST 1 !!!" << endl; // prints !!!Hello World!!!
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



	vector<ZZ> poly1;
	vector<ZZ> poly2;

	for (i = 0; i < N; ++i) {
		ZZ m1;
		ZZ m2;
		RandomBits(m1, 3);
		RandomBits(m2, 3);
		m1 = params.p - m1;
		m2 = params.p - m2;

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
	ZZX dfft = ZZX::zero();
	for (i = 0; i < cfft1.size(); ++i) {
		Cipher cmul = scheme.mul(cfft1[i], cfft2[i]);
		Cipher ms = scheme.modSwitch(cmul, cmul.level + 1);
		ZZ d = scheme.decrypt(ms);
		SetCoeff(dfft, i, d);
	}
	timeutils.stop("mul and decrypt fft");


	ZZX zpoly1 = ZZX::zero();
	ZZX zpoly2 = ZZX::zero();
	ZZX zpoly = ZZX::zero();

	for (i = 0; i < N; ++i) {
		SetCoeff(zpoly1, i, poly1[i]);
		SetCoeff(zpoly2, i, poly2[i]);
	}
	zpoly1.normalize();
	zpoly2.normalize();

	CPolyRingUtils::mulPoly(zpoly, zpoly1, zpoly2, N);

	cout << "zpoly1" << zpoly1 << endl;
	cout << "zpoly2" << zpoly2 << endl;

	cout << "zpoly" << zpoly << endl;
	cout << "dfft" << dfft << endl;

	cout << "!!! END TEST FFT !!!" << endl; // prints !!!Hello World!!!
}



int main() {
	testFFT();
	return 0;
}
