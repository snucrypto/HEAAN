#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>

#include "FPHEFFTCipher.h"
#include "FPHEFFTParams.h"
#include "FPHEFFTPubKey.h"
#include "FPHEFFTScheme.h"
#include "FPHEFFTSecKey.h"
#include "TimeUtils.h"
#include "ZRingUtils.h"

using namespace std;

void test1() {
	TimeUtils timeutils;

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;

	cout << "------------------" << endl;

	timeutils.start("GenParams");
	FPHEFFTParams params(lambda, false);
	timeutils.stop("GenParams");

	cout << "------------------" << endl;

	timeutils.start("GenSecKey");
	FPHEFFTSecKey secretKey(params);
	timeutils.stop("GenSecKey");

	cout << "------------------" << endl;

	timeutils.start("GenPubKey");
	FPHEFFTPubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");

	cout << "------------------" << endl;

	timeutils.start("GenScheme");
	FPHEFFTScheme scheme(params, secretKey, publicKey);
	timeutils.stop("GenScheme");

	cout << "------------------" << endl;

//	cout << "sk: " << secretKey.s << endl;
	cout << params.toString() << endl;

	cout << "------------------" << endl;

	ZZ m;
	RandomBits(m, 5);
	m = params.p - m;
	ZZ mm = m + m;
	ZZ m2 = m * m;
	ZZ ms = m2 / params.p;
	ZZ me = m2;
	ZZ ms2 = ms * ms;
	ZZ mss = ms2 / params.p;
	ZZ mse = ms2;

	cout << "------------------" << endl;

	cout << " m:  " << m << endl;
	cout << " mm: " << mm << endl;
	cout << " m2: " << m2 << endl;
	cout << " ms: " << ms << endl;
	cout << " me: " << me << endl;
	cout << " ms2: " << ms2 << endl;
	cout << " mss: " << mss << endl;
	cout << " mse: " << mse << endl;

	cout << "------------------" << endl;

	timeutils.start("Encrypt c");
	FPHEFFTCipher c = scheme.encrypt(m);
	timeutils.stop("Encrypt c");

	cout << "------------------" << endl;

	timeutils.start("Add cc");
	FPHEFFTCipher cc = scheme.add(c, c);
	timeutils.stop("Add cc");

	cout << "------------------" << endl;

	timeutils.start("Mul c2");
	FPHEFFTCipher c2 = scheme.mul(c, c);
	timeutils.stop("Mul c2");

	cout << "------------------" << endl;

	timeutils.start("MS cs");
	FPHEFFTCipher cs = scheme.modSwitch(c2, 2);
	timeutils.stop("MS cs");

	cout << "------------------" << endl;

	timeutils.start("ME ce");
	FPHEFFTCipher ce = scheme.modEmbed(c2, 2);
	timeutils.stop("ME ce");

	cout << "------------------" << endl;

	timeutils.start("Mul cs2");
	FPHEFFTCipher cs2 = scheme.mul(cs, cs);
	timeutils.stop("Mul cs2");

	cout << "------------------" << endl;



	cout << "------------------" << endl;

	timeutils.start("MS css");
	FPHEFFTCipher css = scheme.modSwitch(cs2, 3);
	timeutils.stop("MS css");

	cout << "------------------" << endl;

	timeutils.start("ME cse");
	FPHEFFTCipher cse = scheme.modEmbed(cs2, 3);
	timeutils.stop("ME cse");

	cout << "------------------" << endl;

	cout << "------------------" << endl;

	timeutils.start("Decrypt c");
	ZZ d = scheme.decrypt(c);
	timeutils.stop("Decrypt c");

	cout << "------------------" << endl;

	ZZ dd = scheme.decrypt(cc);
	ZZ d2 = scheme.decrypt(c2);
	ZZ ds = scheme.decrypt(cs);
	ZZ de = scheme.decrypt(ce);
	ZZ ds2 = scheme.decrypt(cs2);
	ZZ dss = scheme.decrypt(css);
	ZZ dse = scheme.decrypt(cse);

	cout << "------------------" << endl;

	cout << "qL: " << params.qL << endl;
	cout << "p: " << params.p << endl;
	cout << "d:  " << d << endl;
	cout << "dd: " << dd << endl;
	cout << "d2: " << d2 << endl;
	cout << "ds: " << ds << endl;
	cout << "de: " << de << endl;
	cout << "ds2: " << ds2 << endl;
	cout << "dss: " << dss << endl;
	cout << "dse: " << dse << endl;

	cout << "------------------" << endl;

	ZZ e = m - d;
	ZZ ee = mm - dd;
	ZZ e2 = m2 - d2;
	ZZ es = ms - ds;
	ZZ eE = me - de;
	ZZ es2 = ms2 - ds2;
	ZZ ess = mss - dss;
	ZZ ese = mse - dse;

	cout << "------------------" << endl;
	cout << "m:  " << m << endl;
	cout << "d:  " << d << endl;
	cout << "e:  " << e << endl;
	cout << "------------------" << endl;
	cout << "mm: " << mm << endl;
	cout << "dd: " << dd << endl;
	cout << "ee: " << ee << endl;
	cout << "------------------" << endl;
	cout << "m2: " << m2 << endl;
	cout << "d2: " << d2 << endl;
	cout << "e2: " << e2 << endl;
	cout << "------------------" << endl;
	cout << "ms: " << ms << endl;
	cout << "ds: " << ds << endl;
	cout << "es: " << es << endl;
	cout << "------------------" << endl;
	cout << "me: " << me << endl;
	cout << "de: " << de << endl;
	cout << "eE: " << eE << endl;
	cout << "------------------" << endl;
	cout << "ms2: " << ms2 << endl;
	cout << "ds2: " << ds2 << endl;
	cout << "es2: " << es2 << endl;
	cout << "------------------" << endl;
	cout << "mss: " << mss << endl;
	cout << "dss: " << dss << endl;
	cout << "ess: " << ess << endl;
	cout << "------------------" << endl;
	cout << "mse: " << mse << endl;
	cout << "dse: " << dse << endl;
	cout << "ese: " << ese << endl;
	cout << "------------------" << endl;

	cout << "!!!BYE FPHE!!!" << endl; // prints !!!Hello World!!!
}

void test2() {
	TimeUtils timeutils;

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;

	cout << "------------------" << endl;

	timeutils.start("GenParams");
	FPHEFFTParams params(lambda, false);
	timeutils.stop("GenParams");

	cout << "------------------" << endl;

	timeutils.start("GenSecKey");
	FPHEFFTSecKey secretKey(params);
	timeutils.stop("GenSecKey");

	cout << "------------------" << endl;

	timeutils.start("GenPubKey");
	FPHEFFTPubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");

	cout << "------------------" << endl;

	timeutils.start("GenScheme");
	FPHEFFTScheme scheme(params, secretKey, publicKey);
	timeutils.stop("GenScheme");

	cout << "------------------" << endl;

	int i;

	ZZ x[16];
	ZZ x2[8];
	ZZ x4[4];
	ZZ x8[2];
	ZZ x16;

	ZZ d[16];
	ZZ d2[8];
	ZZ d4[4];
	ZZ d8[2];
	ZZ d16;

	ZZ e[16];
	ZZ e2[8];
	ZZ e4[4];
	ZZ e8[2];
	ZZ e16;

	cout << "------------------" << endl;
	for (i = 0; i < 16; ++i) {
		RandomBits(x[i], 5);
		x[i] = params.p - x[i];
		cout << " x["<< i << "]:  " << x[i] << endl;
	}
	cout << "------------------" << endl;
	for (i = 0; i < 8; ++i) {
		x2[i] = (x[i] * x[i + 8]) / params.p;
		cout << " x2["<< i << "]:  " << x2[i] << endl;
	}
	cout << "------------------" << endl;
	for (i = 0; i < 4; ++i) {
		x4[i] = (x2[i] * x2[i + 4]) / params.p;
		cout << " x4["<< i << "]:  " << x4[i] << endl;
	}
	cout << "------------------" << endl;
	for (i = 0; i < 2; ++i) {
		x8[i] = x4[i] * x4[i + 2] / params.p;
		cout << " x8["<< i << "]:  " << x8[i] << endl;
	}
	cout << "------------------" << endl;
	x16 = (x8[0] * x8[1]) / params.p;
	cout << " x16:  " << x16 << endl;
	cout << "------------------" << endl;

	vector<FPHEFFTCipher> c;
	vector<FPHEFFTCipher> c2;
	vector<FPHEFFTCipher> c4;
	vector<FPHEFFTCipher> c8;

	cout << "------------------" << endl;
	timeutils.start("level1");
	for (i = 0; i < 16; ++i) {
		c.push_back(scheme.encrypt(x[i]));
	}
	timeutils.stop("level1");
	cout << "------------------" << endl;
	for (i = 0; i < 16; ++i) {
		d[i] = scheme.decrypt(c[i]);
		e[i] = x[i] - d[i];
		cout << "------------------" << endl;
		cout << " x["<< i << "]:  " << x[i] << endl;
		cout << " d["<< i << "]:  " << d[i] << endl;
		cout << " e["<< i << "]:  " << e[i] << endl;
		cout << "------------------" << endl;
	}
	cout << "------------------" << endl;
	timeutils.start("level2");
	for (i = 0; i < 8; ++i) {
		FPHEFFTCipher temp = scheme.mul(c[i], c[i + 8]);
		scheme.modSwitchAndEqual(temp, 2);
		c2.push_back(temp);
	}
	timeutils.stop("level2");
	cout << "------------------" << endl;
	for (i = 0; i < 8; ++i) {
		d2[i] = scheme.decrypt(c2[i]);
		e2[i] = x2[i] - d2[i];
		cout << "------------------" << endl;
		cout << " x2["<< i << "]:  " << x2[i] << endl;
		cout << " d2["<< i << "]:  " << d2[i] << endl;
		cout << " e2["<< i << "]:  " << e2[i] << endl;
		cout << "------------------" << endl;
	}
	cout << "------------------" << endl;
	timeutils.start("level3");
	for (i = 0; i < 4; ++i) {
		FPHEFFTCipher temp = scheme.mul(c2[i], c2[i + 4]);
		scheme.modSwitchAndEqual(temp, 3);
		c4.push_back(temp);
	}
	timeutils.stop("level3");
	cout << "------------------" << endl;
	for (i = 0; i < 4; ++i) {
		d4[i] = scheme.decrypt(c4[i]);
		e4[i] = x4[i] - d4[i];
		cout << "------------------" << endl;
		cout << " x4["<< i << "]:  " << x4[i] << endl;
		cout << " d4["<< i << "]:  " << d4[i] << endl;
		cout << " e4["<< i << "]:  " << e4[i] << endl;
		cout << "------------------" << endl;
	}
	cout << "------------------" << endl;
	timeutils.start("level4");
	for (i = 0; i < 2; ++i) {
		FPHEFFTCipher temp = scheme.mul(c4[i], c4[i + 2]);
		scheme.modSwitchAndEqual(temp, 4);
		c8.push_back(temp);
	}
	timeutils.stop("level4");
	cout << "------------------" << endl;
	for (i = 0; i < 2; ++i) {
		d8[i] = scheme.decrypt(c8[i]);
		e8[i] = x8[i] - d8[i];
		cout << "------------------" << endl;
		cout << " x8["<< i << "]:  " << x8[i] << endl;
		cout << " d8["<< i << "]:  " << d8[i] << endl;
		cout << " e8["<< i << "]:  " << e8[i] << endl;
		cout << "------------------" << endl;
	}
	cout << "------------------" << endl;
	timeutils.start("level5");
	FPHEFFTCipher c16 = scheme.mul(c8[0], c8[1]);
	scheme.modSwitchAndEqual(c16, 5);
	timeutils.stop("level5");
	cout << "------------------" << endl;
	d16 = scheme.decrypt(c16);
	e16 = x16 - d16;
	cout << "------------------" << endl;
	cout << c16.c0fft[0] << endl;
	cout << " x16:  " << x16 << endl;
	cout << " d16:  " << d16 << endl;
	cout << " e16:  " << e16 << endl;
	cout << "------------------" << endl;



	cout << "!!!BYE FPHE!!!" << endl; // prints !!!Hello World!!!
}

void test3() {
	TimeUtils timeutils;

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;

	cout << "------------------" << endl;

	timeutils.start("GenParams");
	FPHEFFTParams params(lambda, false);
	timeutils.stop("GenParams");

	cout << "------------------" << endl;

	timeutils.start("GenSecKey");
	FPHEFFTSecKey secretKey(params);
	timeutils.stop("GenSecKey");

	cout << "------------------" << endl;

	timeutils.start("GenPubKey");
	FPHEFFTPubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");

	cout << "------------------" << endl;

	timeutils.start("GenScheme");
	FPHEFFTScheme scheme(params, secretKey, publicKey);
	timeutils.stop("GenScheme");

	cout << "------------------" << endl;

	vector<ZZ> y;
	ZZ x(40900);
	FPHEFFTCipher c = scheme.encrypt(x);
	timeutils.start("convert");
	ZRingUtils::convertfft(y, c.c0fft, params.fft, params.qL, params.phim);
	timeutils.stop("convert");
}

int main() {
//	test2();
	test1();
//	test3();
	return 0;
}
