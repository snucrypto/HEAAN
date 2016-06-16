#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>

#include "utils/TimeUtils.h"
#include "utils/PolyRingUtils.h"
#include "polyscheme/PolyCipher.h"
#include "polyscheme/PolyParams.h"
#include "polyscheme/PolyPubKey.h"
#include "polyscheme/PolyScheme.h"
#include "polyscheme/PolySecKey.h"

using namespace std;



void test1() {
	TimeUtils timeutils;

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;

	cout << "------------------" << endl;

	timeutils.start("GenParams");
	PolyParams params(lambda, false);
	timeutils.stop("GenParams");

	cout << "------------------" << endl;

	timeutils.start("GenSecKey");
	PolySecKey secretKey(params);
	timeutils.stop("GenSecKey");

	cout << "------------------" << endl;

	timeutils.start("GenPubKey");
	PolyPubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");

	cout << "------------------" << endl;

	timeutils.start("GenScheme");
	PolyScheme scheme(params, secretKey, publicKey);
	timeutils.stop("GenScheme");

	cout << "------------------" << endl;

//	cout << "sk: " << secretKey.s << endl;
//	cout << params.toString() << endl;

	cout << "------------------" << endl;

	ZZ m;
	RandomBits(m, 3);
	m = params.p - m;

	vector<ZZ> m2k;
	vector<ZZ> m2ks;
	vector<ZZ> m2ke;

	long i;
	long deg = 5;

	m2k.push_back(m);
	m2ks.push_back(m);
	m2ke.push_back(m);

	for (i = 1; i < deg; ++i) {
		m2k.push_back(m2ks[i-1] * m2ks[i-1]);
		m2ks.push_back(m2k[i] / params.p);
		m2ke.push_back(m2k[i]);
	}

	vector<PolyCipher> c2k;
	vector<PolyCipher> c2ks;
	vector<PolyCipher> c2ke;

	timeutils.start("Encrypt c");
	PolyCipher c = scheme.encrypt(m);
	timeutils.stop("Encrypt c");

	c2k.push_back(c);
	c2ks.push_back(c);
	c2ke.push_back(c);

	for (i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mul ");
		PolyCipher c2 = scheme.mul(c2ks[i - 1], c2ks[i - 1]);
		timeutils.stop("Mul ");
		c2k.push_back(c2);

		cout << "------------------" << endl;

		timeutils.start("MS ");
		PolyCipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2ks.push_back(cs);

		cout << "------------------" << endl;

		timeutils.start("ME ");
		PolyCipher ce = scheme.modEmbed(c2, i + 1);
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
		cout << "------------------" << endl;
		cout << "ms: " << i << " " << m2ks[i] << endl;
		cout << "ds: " << i << " " << d2ks[i] << endl;
		cout << "es: " << i << " " << e2ks[i] << endl;
		cout << "------------------" << endl;
		cout << "me: " << i << " " << m2ke[i] << endl;
		cout << "de: " << i << " " << d2ke[i] << endl;
		cout << "ee: " << i << " " << e2ke[i] << endl;
		cout << "------------------" << endl;

	}
	cout << "!!!BYE FPHE!!!" << endl; // prints !!!Hello World!!!
}

void test2() {
	TimeUtils timeutils;

	cout << "!!!HELLO FPHE!!!" << endl; // prints !!!Hello World!!!

	long lambda = 10;

	cout << "------------------" << endl;

	timeutils.start("GenParams");
	PolyParams params(lambda, false);
	timeutils.stop("GenParams");

	cout << "------------------" << endl;

	timeutils.start("GenSecKey");
	PolySecKey secretKey(params);
	timeutils.stop("GenSecKey");

	cout << "------------------" << endl;

	timeutils.start("GenPubKey");
	PolyPubKey publicKey(params, secretKey);
	timeutils.stop("GenPubKey");

	cout << "------------------" << endl;

	timeutils.start("GenScheme");
	PolyScheme scheme(params, secretKey, publicKey);
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

	vector<PolyCipher> c;
	vector<PolyCipher> c2;
	vector<PolyCipher> c4;
	vector<PolyCipher> c8;

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
		PolyCipher temp = scheme.mul(c[i], c[i + 8]);
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
		PolyCipher temp = scheme.mul(c2[i], c2[i + 4]);
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
		PolyCipher temp = scheme.mul(c4[i], c4[i + 2]);
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
	PolyCipher c16 = scheme.mul(c8[0], c8[1]);
	scheme.modSwitchAndEqual(c16, 5);
	timeutils.stop("level5");
	cout << "------------------" << endl;
	d16 = scheme.decrypt(c16);
	e16 = x16 - d16;
	cout << "------------------" << endl;
	cout << " x16:  " << x16 << endl;
	cout << " d16:  " << d16 << endl;
	cout << " e16:  " << e16 << endl;
	cout << "------------------" << endl;



	cout << "!!!BYE FPHE!!!" << endl; // prints !!!Hello World!!!
}

int main() {
//	test2();
	cout << "------------------" << endl;
	cout << "------------------" << endl;
	cout << "------------------" << endl;
	cout << "------------------" << endl;
	cout << "------------------" << endl;
	test1();
	return 0;
}
