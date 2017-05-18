#include "SchemeAlgo.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "Params.h"

Cipher SchemeAlgo::powerOf2(Cipher& c, const long& logDegree) {
	Cipher res = c;
	for (long i = 1; i < logDegree + 1; ++i) {
		scheme.squareAndEqual(res);
		scheme.modSwitchAndEqual(res);
	}
	return res;
}

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

//-----------------------------------------

void SchemeAlgo::powerExtended(vector<Cipher>& res, Cipher& c, const long& degree) {
	res.reserve(degree);
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

//-----------------------------------------

Cipher SchemeAlgo::prod2(vector<Cipher>& ciphers, const long& logDegree) {
	vector<Cipher> res = ciphers;
	for (long i = logDegree; i > 0; --i) {
		vector<Cipher> tmp;
		long powi = (1 << i);
		tmp.reserve(powi / 2);
		for (long j = 0; j < powi; j = j + 2) {
			Cipher cmult = scheme.mult(res[j], res[j + 1]);
			scheme.modSwitchAndEqual(cmult);
			tmp.push_back(cmult);
		}
		res = tmp;
	}
	return res[0];
}

void SchemeAlgo::prod2Extended(vector<vector<Cipher>>& res, vector<Cipher>& ciphers, const long& logDegree) {
	res.reserve(logDegree + 1);
	res.push_back(ciphers);
	for (long i = 0; i < logDegree; ++i) {
		vector<Cipher> ctmp;
		long size = res[i].size();
		ctmp.reserve(size / 2);
		for (long j = 0; j < size; j = j + 2) {
			Cipher c2 = scheme.mult(res[i][j], res[i][j + 1]);
			scheme.modSwitchAndEqual(c2);
			ctmp.push_back(c2);
		}
		res.push_back(ctmp);
	}
}

//-----------------------------------------

Cipher SchemeAlgo::inverse(Cipher& c, const long& steps) {
	Cipher cpow = c;
	Cipher tmp = scheme.addConst(c, scheme.params.p);
	scheme.modEmbedAndEqual(tmp);
	Cipher res = tmp;

	for (long i = 0; i < steps - 1; ++i) {
		scheme.squareAndEqual(cpow);
		scheme.modSwitchAndEqual(cpow);
		tmp = cpow;
		scheme.addConstAndEqual(tmp, scheme.params.p);
		scheme.multAndEqual(tmp, res);
		scheme.modSwitchAndEqual(tmp, i + 3);
		res = tmp;
	}
	return res;
}

void SchemeAlgo::inverseExtended(vector<Cipher>& res, Cipher& c, const long& steps) {
	res.reserve(steps);
	Cipher cpow = c;
	Cipher tmp = scheme.addConst(c, scheme.params.p);
	scheme.modEmbedAndEqual(tmp);
	res.push_back(tmp);

	for (long i = 0; i < steps - 1; ++i) {
		scheme.squareAndEqual(cpow);
		scheme.modSwitchAndEqual(cpow);
		tmp = cpow;
		scheme.addConstAndEqual(tmp, scheme.params.p);
		scheme.multAndEqual(tmp, res[i]);
		scheme.modSwitchAndEqual(tmp, i + 3);
		res.push_back(tmp);
	}
}

//-----------------------------------------

Cipher SchemeAlgo::function(Cipher& c, string& funcName, const long& degree) {
	vector<Cipher> cpows;
	powerExtended(cpows, c, degree);

	vector<ZZ> pows = scheme.params.taylorPows.powsMap.at(funcName);
	vector<double> coeffs = scheme.params.taylorPows.coeffsMap.at(funcName);

	Cipher res = scheme.multByConst(cpows[0], pows[1]);
	ZZ p2 = scheme.params.p * pows[0];
	scheme.addConstAndEqual(res, p2);


	for (int i = 1; i < degree; ++i) {
		if(abs(coeffs[i + 1]) > 1e-17) {
			Cipher tmp = scheme.multByConst(cpows[i], pows[i + 1]);
			scheme.modEmbedAndEqual(res, tmp.level);
			scheme.addAndEqual(res, tmp);
		}
	}
	scheme.modSwitchAndEqual(res);
	return res;
}

Cipher SchemeAlgo::functionSimple(Cipher& c, string& funcName, const long& degree) {
	vector<Cipher> cpows;
	powerExtended(cpows, c, degree);

	vector<ZZ> pows = scheme.params.taylorPows.powsMap.at(funcName);
	vector<double> coeffs = scheme.params.taylorPows.coeffsMap.at(funcName);

	Cipher res = scheme.multByConst(cpows[0], pows[1]);
	ZZ p2 = scheme.params.p * pows[0];
	scheme.addConstAndEqual(res, p2);

	for (int i = 1; i < degree; ++i) {
		if(abs(coeffs[i + 1]) > 1e-27) {
			Cipher tmp = scheme.multByConst(cpows[i], pows[i + 1]);
			scheme.modEmbedAndEqual(res, tmp.level);
			scheme.addAndEqual(res, tmp);
		}
	}
	return res;
}

void SchemeAlgo::functionExtended(vector<Cipher>& res, Cipher& c, string& funcName, const long& degree) {
	vector<Cipher> cpows;
	powerExtended(cpows, c, degree);

	vector<ZZ> pows = scheme.params.taylorPows.powsMap.at(funcName);
	vector<double> coeffs = scheme.params.taylorPows.coeffsMap.at(funcName);

	Cipher cn = scheme.multByConst(cpows[0], pows[1]);
	ZZ p2 = scheme.params.p * pows[0];
	scheme.addConstAndEqual(cn, p2);
	res.push_back(cn);

	long idx = 0;
	for (long i = 1; i < degree; ++i) {
		if(abs(coeffs[i + 1]) > 1e-17) {
			cn = scheme.multByConst(cpows[i], pows[i + 1]);
			Cipher tmp = scheme.modEmbed(res[idx++], cn.level);
			scheme.addAndEqual(cn, tmp);
			res.push_back(cn);
		}
	}
	for (long i = 0; i < res.size(); ++i) {
		scheme.modSwitchAndEqual(res[i]);
	}
}

//-----------------------------------------

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

vector<Cipher> SchemeAlgo::fftInvSimple(vector<Cipher>& ciphers) {
	return fftRaw(ciphers, false);
}
