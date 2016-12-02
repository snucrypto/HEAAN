#include "SchemeAlgo.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "Params.h"

void SchemeAlgo::powerOf2(vector<Cipher>& c2k, Cipher& c, const long& deg) {

	c2k.push_back(c);

	for (long i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Square ");
		Cipher c2 = scheme.square(c2k[i - 1]);
		timeutils.stop("Square ");

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2k.push_back(cs);
		cout << "------------------" << endl;
	}
}

void SchemeAlgo::prodOfSame(vector<Cipher>& c2k, Cipher& c, const long& deg) {

	c2k.push_back(c);

	for (long i = 1; i < deg; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Mult ");
		Cipher c2 = scheme.mult(c2k[i - 1], c2k[i - 1]);
		timeutils.stop("Mult ");

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2k.push_back(cs);

		cout << "------------------" << endl;
	}
}

void SchemeAlgo::inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, const long& r) {
	c2k.push_back(c);

	Cipher cp = scheme.addConstant(c, scheme.params.p);
	Cipher v = scheme.modEmbed(cp, cp.level + 1);
	v2k.push_back(v);

	for (long i = 1; i < r-1; ++i) {
		cout << "---------" << i << "---------" << endl;

		timeutils.start("Square ");
		Cipher c2 = scheme.square(c2k[i - 1]);
		timeutils.stop("Square ");

		cout << "------------------" << endl;

		timeutils.start("MS ");
		Cipher cs = scheme.modSwitch(c2, i + 1);
		timeutils.stop("MS ");
		c2k.push_back(cs);

		cout << "------------------" << endl;

		timeutils.start("Add const ");
		Cipher c2p = scheme.addConstant(cs, scheme.params.p);
		timeutils.stop("Add const ");

		timeutils.start("Mult ");
		Cipher v2 = scheme.mult(v2k[i-1], c2p);
		timeutils.stop("Mult ");

		timeutils.start("MS ");
		Cipher vs = scheme.modSwitch(v2, i + 2);
		timeutils.stop("MS ");
		v2k.push_back(vs);

		cout << "------------------" << endl;
	}
}


vector<Cipher> SchemeAlgo::fft(vector<Cipher>& ciphers, CKsi& cksi, const bool& isForward) {
	long csize = ciphers.size();

	if(csize == 1) {
		return ciphers;
	}

	long i;
	long logcsize = log2(csize);
	long csizeh = csize >> 1;

	vector<Cipher> res, tmp, sub1, sub2;

	for (i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fft(sub1, cksi, isForward);
	vector<Cipher> y2 = fft(sub2, cksi, isForward);

	if(isForward) {
		for (i = 0; i < csizeh; ++i) {
			scheme.multByConstantAndEqual(y2[i], cksi.pows[logcsize][i]);
			scheme.modSwitchAndEqual(y2[i]);
		}
	} else {
		for (i = 0; i < csizeh; ++i) {
			scheme.multByConstantAndEqual(y2[i], cksi.pows[logcsize][csizeh - i]);
			scheme.modSwitchAndEqual(y2[i]);
		}
	}

	for (i = 0; i < csizeh; ++i) {
		Cipher sum = scheme.add(y1[i], y2[i]);
		Cipher diff = scheme.sub(y1[i], y2[i]);
		res.push_back(sum);
		tmp.push_back(diff);
	}
	for (long i = 0; i < csizeh; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}
