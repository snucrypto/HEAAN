#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>

#include "scheme/Cipher.h"
#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SecKey.h"
#include "utils/TimeUtils.h"
#include "utils/PolyRingUtils.h"

using namespace std;
using namespace NTL;

void test1() {
	cout << "!!! START TEST 1 !!!" << endl; // prints !!!Hello World!!!

	TimeUtils timeutils;

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
	m = 1;
	m <<= params.logP;
	m -= 3;


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
		m2ks.push_back(m2k[i] >> params.logP);
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

	cout << params.logQ << endl;

	cout << "!!! END TEST 1 !!!" << endl; // prints !!!Hello World!!!
}

int main() {
	test1();
	return 0;
}
