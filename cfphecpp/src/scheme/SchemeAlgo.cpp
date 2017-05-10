#include "SchemeAlgo.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "Params.h"

void SchemeAlgo::powerOf2(vector<Cipher>& c2k, Cipher& c, const long& deg) {
	c2k.reserve(deg + 1);
	c2k.push_back(c);
	for (long i = 1; i < deg + 1; ++i) {
		Cipher c2 = scheme.square(c2k[i - 1]);
		scheme.modSwitchAndEqual(c2);
		c2k.push_back(c2);
	}
}

void SchemeAlgo::prod2(vector<vector<Cipher>>& cs2k, vector<Cipher>& cs, const long& deg) {
	cs2k.reserve(deg + 1);
	cs2k.push_back(cs);
	long size, idx;
	for (long i = 1; i < deg + 1; ++i) {
		vector<Cipher> c2k;
		idx = 0;
		size = cs2k[i-1].size();
		while(idx < size) {
			Cipher c2 = scheme.mult(cs2k[i - 1][idx], cs2k[i - 1][idx + 1]);
			scheme.modSwitchAndEqual(c2);
			c2k.push_back(c2);
			idx += 2;
		}
		cs2k.push_back(c2k);
	}
}

void SchemeAlgo::inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, const long& r) {
	c2k.reserve(r-1);
	v2k.reserve(r-1);
	c2k.push_back(c);

	Cipher tmp = scheme.addConst(c, scheme.params.p);
	scheme.modEmbedAndEqual(tmp);
	v2k.push_back(tmp);

	for (long i = 1; i < r-1; ++i) {
		tmp = scheme.square(c2k[i - 1]);
		scheme.modSwitchAndEqual(tmp);
		c2k.push_back(tmp);
		scheme.addConstAndEqual(tmp, scheme.params.p);
		scheme.multAndEqual(tmp, v2k[i-1]);
		scheme.modSwitchAndEqual(tmp, i + 2);
		v2k.push_back(tmp);
	}
}

void SchemeAlgo::exp(Cipher& res, Cipher& c, const long& deg) {

}

void SchemeAlgo::sigmoid(Cipher& res, Cipher& c, const long& deg) {

}

vector<Cipher> SchemeAlgo::fftRaw(vector<Cipher>& ciphers, const bool& isForward) {
	long csize = ciphers.size();

	if(csize == 1) {
		return ciphers;
	}

	long csizeh = csize >> 1;

	vector<Cipher> res, sub1, sub2;

	res.reserve(csize);
	sub1.reserve(csizeh);
	sub2.reserve(csizeh);

	for (long i = 0; i < csize; i = i+2) {
		sub1.push_back(ciphers[i]);
		sub2.push_back(ciphers[i+1]);
	}

	vector<Cipher> y1 = fftRaw(sub1, isForward);
	vector<Cipher> y2 = fftRaw(sub2, isForward);

	long shift = isForward ? (scheme.params.d / csize) : (scheme.params.d - scheme.params.d / csize);

	for (long i = 0; i < csizeh; ++i) {
		scheme.multByMonomialAndEqual(y2[i], shift * i);
		Cipher sum = y1[i];
		scheme.addAndEqual(sum, y2[i]);
		scheme.subAndEqual(y1[i], y2[i]);
		res.push_back(sum);
	}

	for (long i = 0; i < csizeh; ++i) {
		res.push_back(y1[i]);
	}
	return res;
}

vector<Cipher> SchemeAlgo::fft(vector<Cipher>& ciphers) {
	return fftRaw(ciphers, true);
}

vector<Cipher> SchemeAlgo::fftInv(vector<Cipher>& ciphers) {
	vector<Cipher> fftInv = fftRaw(ciphers, false);
	long N = fftInv.size();
	long logN = log2(N);
	long bits = scheme.params.logp - logN;
	for (long i = 0; i < N; ++i) {
		scheme.leftShiftAndEqual(fftInv[i], bits);
		scheme.modSwitchAndEqual(fftInv[i]);
	}
	return fftInv;
}
