#include "SchemeAlgo.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "Params.h"

void SchemeAlgo::powerOf2(vector<Cipher>& c2k, Cipher& c, const long& deg) {
	c2k.push_back(c);
	for (long i = 1; i < deg + 1; ++i) {
		Cipher c2 = scheme.square(c2k[i - 1]);
		Cipher cs = scheme.modSwitch(c2, i + 1);
		c2k.push_back(cs);
	}
}

void SchemeAlgo::prod2(vector<vector<Cipher>>& cs2k, vector<Cipher>& cs, const long& deg) {
	cs2k.push_back(cs);
	long size, idx;
	for (long i = 1; i < deg + 1; ++i) {
		vector<Cipher> c2k;
		idx = 0;
		size = cs2k[i-1].size();
		while(idx < size) {
			Cipher c2 = scheme.mult(cs2k[i - 1][idx], cs2k[i - 1][idx + 1]);
			Cipher cs = scheme.modSwitch(c2, i + 1);
			c2k.push_back(cs);
			idx += 2;
		}
		cs2k.push_back(c2k);
	}
}

void SchemeAlgo::inverse(vector<Cipher>& c2k, vector<Cipher>& v2k, Cipher& c, const long& r) {
	c2k.push_back(c);

	Cipher cp = scheme.addConstant(c, scheme.params.p);
	Cipher v = scheme.modEmbed(cp, cp.level + 1);
	v2k.push_back(v);

	for (long i = 1; i < r-1; ++i) {
		Cipher c2 = scheme.square(c2k[i - 1]);
		Cipher cs = scheme.modSwitch(c2, i + 1);
		c2k.push_back(cs);
		Cipher c2p = scheme.addConstant(cs, scheme.params.p);
		Cipher v2 = scheme.mult(v2k[i-1], c2p);
		Cipher vs = scheme.modSwitch(v2, i + 2);
		v2k.push_back(vs);
	}
}


vector<Cipher> SchemeAlgo::fftRaw(vector<Cipher>& ciphers, CKsi& cksi, const bool& isForward) {
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

	vector<Cipher> y1 = fftRaw(sub1, cksi, isForward);
	vector<Cipher> y2 = fftRaw(sub2, cksi, isForward);
	if(isForward) {
		for (i = 0; i < csizeh; ++i) {
			scheme.multByConstantAndEqual(y2[i], cksi.pows[logcsize][i]);
			scheme.modSwitchAndEqual(y2[i]);
			scheme.modEmbedAndEqual(y1[i]);
		}
	} else {
		scheme.multByConstantAndEqual(y2[0], cksi.pows[logcsize][0]);
		scheme.modSwitchAndEqual(y2[0]);
		scheme.modEmbedAndEqual(y1[0]);
		for (i = 1; i < csizeh; ++i) {
			scheme.multByConstantAndEqual(y2[i], cksi.pows[logcsize][csize - i]);
			scheme.modSwitchAndEqual(y2[i]);
			scheme.modEmbedAndEqual(y1[i]);
		}
	}

	for (i = 0; i < csizeh; ++i) {
		Cipher sum = scheme.add(y1[i], y2[i]);
		Cipher diff = scheme.sub(y1[i], y2[i]);
		res.push_back(sum);
		tmp.push_back(diff);
	}

	for (i = 0; i < csizeh; ++i) {
		res.push_back(tmp[i]);
	}

	return res;
}

vector<Cipher> SchemeAlgo::fft(vector<Cipher>& ciphers, CKsi& cksi) {
	return fftRaw(ciphers, cksi, true);
}

vector<Cipher> SchemeAlgo::fftInv(vector<Cipher>& ciphers, CKsi& cksi) {
	vector<Cipher> fftInv = fftRaw(ciphers, cksi, false);
	long N = fftInv.size();
	long logN = log2(N);
	long bits = scheme.params.logp - logN;
	for (long i = 0; i < N; ++i) {
		scheme.leftShiftAndEqual(fftInv[i], bits);
		scheme.modSwitchAndEqual(fftInv[i]);
	}
	return fftInv;
}
