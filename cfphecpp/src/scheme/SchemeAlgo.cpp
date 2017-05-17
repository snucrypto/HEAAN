#include "SchemeAlgo.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "Params.h"

void SchemeAlgo::powerOf2Extended(vector<Cipher>& res, Cipher& c, const long& logDegree) {
	res.reserve(logDegree + 1);
	Cipher c2 = c;
	res.push_back(c2);
	for (long i = 1; i < logDegree + 1; ++i) {
		scheme.squareAndEqual(c2);
		scheme.modSwitchAndEqual(c2);
		res.push_back(c2);
	}
}

void SchemeAlgo::powerExtended(vector<Cipher>& res, Cipher& c, const long& degree) {
	long logDegree = log2(degree);
	vector<Cipher> po2;
	powerOf2Extended(po2, c, logDegree);
	for (long i = 0; i < logDegree; ++i) {
		long powi = (1 << i);
		res.push_back(po2[i]);
		for (int j = 0; j < powi-1; ++j) {
			Cipher tmp = scheme.modEmbed(res[j], po2[i].level);
			scheme.multModSwitchAndEqual(tmp, po2[i]);
			res.push_back(tmp);
		}
	}
	res.push_back(po2[logDegree]);
	long degree2 = (1 << logDegree);
	for (int i = 0; i < (degree - degree2); ++i) {
		Cipher tmp = scheme.modEmbed(res[i], po2[logDegree].level);
		scheme.multModSwitchAndEqual(tmp, po2[logDegree]);
		res.push_back(tmp);
	}
}

void SchemeAlgo::prod2Extended(vector<vector<Cipher>>& res, vector<Cipher>& cs, const long& logDegree) {
	res.reserve(logDegree + 1);
	res.push_back(cs);
	long size, idx;
	for (long i = 1; i < logDegree + 1; ++i) {
		vector<Cipher> c2k;
		idx = 0;
		size = res[i-1].size();
		while(idx < size) {
			Cipher c2 = scheme.mult(res[i - 1][idx], res[i - 1][idx + 1]);
			scheme.modSwitchAndEqual(c2);
			c2k.push_back(c2);
			idx += 2;
		}
		res.push_back(c2k);
	}
}

void SchemeAlgo::inverseExtended(vector<Cipher>& cres, vector<Cipher>& vres, Cipher& c, const long& steps) {
	cres.reserve(steps-1);
	vres.reserve(steps-1);
	cres.push_back(c);

	Cipher tmp = scheme.addConst(c, scheme.params.p);
	scheme.modEmbedAndEqual(tmp);
	vres.push_back(tmp);

	for (long i = 1; i < steps-1; ++i) {
		tmp = scheme.square(cres[i - 1]);
		scheme.modSwitchAndEqual(tmp);
		cres.push_back(tmp);
		scheme.addConstAndEqual(tmp, scheme.params.p);
		scheme.multAndEqual(tmp, vres[i-1]);
		scheme.modSwitchAndEqual(tmp, i + 2);
		vres.push_back(tmp);
	}
}

void SchemeAlgo::exponentExtended(vector<Cipher>& res, Cipher& c, const long& degree) {
	vector<Cipher> pows;
	powerExtended(pows, c, degree);

	Cipher exp = scheme.multByConst(pows[0], scheme.params.taylorPows.expPows[1]);
	ZZ p2 = scheme.params.p * scheme.params.taylorPows.expPows[0];
	scheme.addConstAndEqual(exp, p2);
	res.push_back(exp);

	for (int i = 1; i < degree; ++i) {
		exp = scheme.multByConst(pows[i], scheme.params.taylorPows.expPows[i + 1]);
		Cipher tmp = scheme.modEmbed(res[i-1], exp.level);
		scheme.addAndEqual(exp, tmp);
		res.push_back(exp);
	}
	for (int i = 0; i < res.size(); ++i) {
		scheme.modSwitchAndEqual(res[i]);
	}
}

void SchemeAlgo::sigmoidExtended(vector<Cipher>& res, Cipher& c, const long& degree) {
	vector<Cipher> pows;
	powerExtended(pows, c, degree);

	Cipher sigmoid = scheme.multByConst(pows[0], scheme.params.taylorPows.sigmoidPows[1]);
	ZZ p2 = scheme.params.p * scheme.params.taylorPows.sigmoidPows[0];
	scheme.addConstAndEqual(sigmoid, p2);
	res.push_back(sigmoid);

	long idx = 0;
	for (int i = 1; i < degree; ++i) {
		if(abs(scheme.params.taylorPows.sigmoidCoeffs[i + 1]) > 1e-17) {
			sigmoid = scheme.multByConst(pows[i], scheme.params.taylorPows.sigmoidPows[i + 1]);
			Cipher tmp = scheme.modEmbed(res[idx++], sigmoid.level);
			scheme.addAndEqual(sigmoid, tmp);
			res.push_back(sigmoid);
		}
	}
	for (int i = 0; i < res.size(); ++i) {
		scheme.modSwitchAndEqual(res[i]);
	}
}

Cipher SchemeAlgo::powerOf2(Cipher& c, const long& logDegree) {
	Cipher res = c;
	for (long i = 1; i < logDegree + 1; ++i) {
		scheme.squareAndEqual(res);
		scheme.modSwitchAndEqual(res);
	}
	return res;
}

Cipher SchemeAlgo::prod2(vector<Cipher>& cs, const long& logDegree) {
	vector<Cipher> res = cs;
	vector<Cipher> tmp2;
	for (long i = logDegree; i > 0; --i) {
		long powi = (1 << i);
		for (long j = 0; j < powi; j = j + 2) {
			Cipher c2 = scheme.mult(res[j], res[j + 1]);
			scheme.modSwitchAndEqual(c2);
			tmp2.push_back(c2);
		}
		res = tmp2;
	}
	return res[0];
}

Cipher SchemeAlgo::inverse(Cipher& c, const long& steps) {
	Cipher tmp = scheme.addConst(c, scheme.params.p);
	scheme.modEmbedAndEqual(tmp);
	Cipher res = c;
	for (long i = 1; i < steps-1; ++i) {
		scheme.squareAndEqual(res);
		scheme.modSwitchAndEqual(res);
		scheme.addConstAndEqual(res, scheme.params.p);
		scheme.multAndEqual(res, tmp);
		scheme.modSwitchAndEqual(res, i + 2);
		tmp = res;
	}
	return res;
}

Cipher SchemeAlgo::exponent(Cipher& c, const long& degree) {
	vector<Cipher> pows;
	powerExtended(pows, c, degree);

	Cipher exp = scheme.multByConst(pows[0], scheme.params.taylorPows.expPows[1]);
	ZZ p2 = scheme.params.p * scheme.params.taylorPows.expPows[0];
	scheme.addConstAndEqual(exp, p2);

	for (int i = 1; i < degree; ++i) {
		Cipher tmp = scheme.multByConst(pows[i], scheme.params.taylorPows.expPows[i + 1]);
		scheme.modEmbedAndEqual(exp, tmp.level);
		scheme.addAndEqual(exp, tmp);
	}
	scheme.modSwitchAndEqual(exp);
	return exp;
}

Cipher SchemeAlgo::sigmoid(Cipher& c, const long& degree) {
	vector<Cipher> pows;
	powerExtended(pows, c, degree);

	Cipher sigmoid = scheme.multByConst(pows[0], scheme.params.taylorPows.sigmoidPows[1]);
	ZZ p2 = scheme.params.p * scheme.params.taylorPows.sigmoidPows[0];
	scheme.addConstAndEqual(sigmoid, p2);

	long idx = 0;
	for (int i = 1; i < degree; ++i) {
		if(abs(scheme.params.taylorPows.sigmoidCoeffs[i + 1]) > 1e-17) {
			Cipher tmp = scheme.multByConst(pows[i], scheme.params.taylorPows.sigmoidPows[i + 1]);
			scheme.modEmbedAndEqual(sigmoid, tmp.level);
			scheme.addAndEqual(sigmoid, tmp);
		}
	}
	return sigmoid;
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

	long shift = isForward ? (scheme.params.M / csize) : (scheme.params.M - scheme.params.M / csize);

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

vector<Cipher> SchemeAlgo::bitReverse(vector<Cipher>& ciphers) {
	vector<Cipher> res;
	vector<Cipher> tmp;
	long N = ciphers.size();
	long logN = log2(N);
	res.push_back(ciphers[0]);
	for (long i = 0; i < logN; ++i) {
		long powi = (1 << i);
		long powni = (1 << (logN - i));
		long pownih = (1 << (logN - i - 1));
		for (long j = 0; j < powi; ++j) {
			tmp.push_back(ciphers[j * powni + pownih]);
		}
		tmp = bitReverse(tmp);
		for (long j = 0; j < tmp.size(); ++j) {
			res.push_back(tmp[j]);
		}
		tmp.clear();
	}
	return res;
}

vector<Cipher> SchemeAlgo::fftButterfly(vector<Cipher>& ciphers) {
	vector<Cipher> fft = ciphers;
	long N = ciphers.size();
	long logN = log2(N);
	for (long i = 0; i < logN; ++i) {
		long powi = (1 << i);
		long logNi = logN - i;
		long pownih = (1 << (logNi - 1));
		long powni = (1 << logNi);
		for (long j = 0; j < powi; ++j) {
			for (long k = 0; k < pownih; ++k) {
				long s = j * powni + k;
				long t = s + pownih;
				Cipher as = scheme.add(fft[s], fft[t]);
				Cipher at = scheme.sub(fft[s], fft[t]);
				scheme.multByMonomialAndEqual(at, powi * k);
				fft[s] = as;
				fft[t] = at;
			}
		}
	}
	return fft;
//	return bitReverse(fft);
}

vector<Cipher> SchemeAlgo::fftButterflyInv(vector<Cipher>& ciphers) {
	vector<Cipher> fftInv = ciphers;
	long N = ciphers.size();
	long logN = log2(N);
	for (long i = 0; i < logN; ++i) {
		long powi = (1 << i);
		long logNi = logN - i;
		long pownih = (1 << (logNi - 1));
		long powni = (1 << logNi);
		for (long j = 0; j < powi; ++j) {
			for (long k = 0; k < pownih; ++k) {
				long s = j * powni + k;
				long t = s + pownih;
				Cipher as = scheme.add(fftInv[s], fftInv[t]);
				Cipher at = scheme.sub(fftInv[s], fftInv[t]);
				scheme.multByMonomialAndEqual(at, scheme.params.M - powi * k);
				fftInv[s] = as;
				fftInv[t] = at;
			}
		}
	}
	long bits = scheme.params.logp - logN;
	for (long i = 0; i < N; ++i) {
		scheme.leftShiftAndEqual(fftInv[i], bits);
		scheme.modSwitchAndEqual(fftInv[i]);
	}
	return bitReverse(fftInv);
}

