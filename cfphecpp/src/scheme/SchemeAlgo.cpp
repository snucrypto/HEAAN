#include "SchemeAlgo.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "../eval/Ksi.h"
#include "Params.h"

void SchemeAlgo::powerOf2(vector<Cipher>& c2k, Cipher& c, long& deg) {

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

void SchemeAlgo::prodOfSame(vector<Cipher>& c2k, Cipher& c, long& deg) {

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

void SchemeAlgo::inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, long& r) {
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


vector<Cipher> SchemeAlgo::fft(vector<Cipher>& ciphers, vector<Ksi>& ksis) {
	long csize = ciphers.size();
	if(csize == 1) {
		return ciphers;
	}

	vector<Cipher> res;

	long logcsize = log2(csize);

	vector<Cipher> sub1, sub2;

	for (long i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fft(sub1, ksis);
	vector<Cipher> y2 = fft(sub2, ksis);

	vector<Cipher> tmp;

	for (long i = 0; i < csize/2; ++i) {
		Cipher mul1 = scheme.multByConstant(y1[i], scheme.params.p);
		scheme.modSwitchAndEqual(mul1, mul1.level + 1);

		CZZ x = ksis[logcsize].pows[i];
		Cipher mul2 = scheme.multByConstant(y2[i], x);
		scheme.modSwitchAndEqual(mul2, mul2.level + 1);

		Cipher sum = scheme.add(mul1, mul2);
		Cipher diff = scheme.sub(mul1, mul2);
		res.push_back(sum);
		tmp.push_back(diff);
	}
	for (long i = 0; i < csize/2; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}

vector<Cipher> SchemeAlgo::fftInv(vector<Cipher>& ciphers, vector<Ksi>& ksis) {
	long csize = ciphers.size();
	if(csize == 1) {
		return ciphers;
	}

	vector<Cipher> res;

	long logcsize = log2(csize);

	vector<Cipher> sub1, sub2;

	for (long i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fft(sub1, ksis);
	vector<Cipher> y2 = fft(sub2, ksis);

	vector<Cipher> tmp;

	{
		Cipher mul1 = scheme.multByConstant(y1[0], scheme.params.p);
		scheme.modSwitchAndEqual(mul1, mul1.level + 1);

		CZZ x = ksis[logcsize].pows[0];
		Cipher mul2 = scheme.multByConstant(y2[0], x);
		scheme.modSwitchAndEqual(mul2, mul2.level + 1);

		Cipher sum = scheme.add(mul1, mul2);
		Cipher diff = scheme.sub(mul1, mul2);
		res.push_back(sum);
		tmp.push_back(diff);
	}

	for (long i = 1; i < csize/2; ++i) {
		Cipher mul1 = scheme.multByConstant(y1[i], scheme.params.p);
		scheme.modSwitchAndEqual(mul1, mul1.level + 1);

		CZZ x = ksis[logcsize].pows[csize - i];
		Cipher mul2 = scheme.multByConstant(y2[i], x);
		scheme.modSwitchAndEqual(mul2, mul2.level + 1);

		Cipher sum = scheme.add(mul1, mul2);
		Cipher diff = scheme.sub(mul1, mul2);
		res.push_back(sum);
		tmp.push_back(diff);
	}

	for (long i = 0; i < csize/2; ++i) {
		res.push_back(tmp[i]);
	}
	return res;
}

vector<Cipher> SchemeAlgo::fftNew(vector<Cipher>& ciphers, vector<Ksi>& ksis, ZZ& factor, long bnd) {
	long csize = ciphers.size();
	long logcsize = log2(csize);

	vector<Cipher> res;

	if(csize == bnd) {

		for (long i = 0; i < csize; ++i) {
			CZZ f = ksis[logcsize].pows[0];
			Cipher c = scheme.multByConstant(ciphers[0], f);

			for (long j = 1; j < csize; ++j) {
				long ij = (i * j) % csize;
				CZZ f = ksis[logcsize].pows[ij];
				Cipher cx = scheme.multByConstant(ciphers[j], f);
				scheme.addAndEqual(c, cx);
			}
			res.push_back(c);
		}
		return res;
	}

	vector<Cipher> sub1, sub2;

	for (long i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fftNew(sub1, ksis, factor, bnd);
	vector<Cipher> y2 = fftNew(sub2, ksis, factor, bnd);

	vector<Cipher> tmp;

	for (long i = 0; i < csize/2; ++i) {
		Cipher mul1 = scheme.multByConstant(y1[i], factor);

		CZZ x = ksis[logcsize].pows[i];
		Cipher mul2 = scheme.multByConstant(y2[i], x);

		Cipher sum = scheme.add(mul1, mul2);
		Cipher diff = scheme.sub(mul1, mul2);
		res.push_back(sum);
		tmp.push_back(diff);
	}

	for (long i = 0; i < csize/2; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}

vector<Cipher> SchemeAlgo::fftInvNew(vector<Cipher>& ciphers, vector<Ksi>& ksis, ZZ& factor, long bnd) {
	long csize = ciphers.size();
	long logcsize = log2(csize);

	vector<Cipher> res;

	if(csize == bnd) {

		for (long i = 0; i < csize; ++i) {
			CZZ f = ksis[logcsize].pows[0];
			Cipher c = scheme.multByConstant(ciphers[0], f);

			for (long j = 1; j < csize; ++j) {
				long ij = (i * j) % csize;
				CZZ f = ksis[logcsize].pows[ij];
				Cipher cx = scheme.multByConstant(ciphers[j], f);
				scheme.addAndEqual(c, cx);
			}
			res.push_back(c);
		}
		return res;
	}

	vector<Cipher> sub1, sub2;

	for (long i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fftNew(sub1, ksis, factor, bnd);
	vector<Cipher> y2 = fftNew(sub2, ksis, factor, bnd);

	vector<Cipher> tmp;

	{
		Cipher mul1 = scheme.multByConstant(y1[0], factor);

		CZZ x = ksis[logcsize].pows[0];
		Cipher mul2 = scheme.multByConstant(y2[0], x);

		Cipher sum = scheme.add(mul1, mul2);
		Cipher diff = scheme.sub(mul1, mul2);
		res.push_back(sum);
		tmp.push_back(diff);
	}

	for (long i = 1; i < csize/2; ++i) {
		Cipher mul1 = scheme.multByConstant(y1[i], factor);

		CZZ x = ksis[logcsize].pows[csize - i];
		Cipher mul2 = scheme.multByConstant(y2[i], x);

		Cipher sum = scheme.add(mul1, mul2);
		Cipher diff = scheme.sub(mul1, mul2);
		res.push_back(sum);
		tmp.push_back(diff);
	}

	for (long i = 0; i < csize/2; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}

