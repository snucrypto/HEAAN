#include "TestScheme.h"

#include <NTL/ZZ.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "../czz/CZZ.h"
#include "../scheme/Cipher.h"
#include "../scheme/Message.h"
#include "../scheme/Params.h"
#include "../scheme/PubKey.h"
#include "../scheme/Scheme.h"
#include "../scheme/SchemeAlgo.h"
#include "../scheme/SecKey.h"
#include "../utils/KsiPows.h"
#include "../utils/NumUtils.h"
#include "../utils/TimeUtils.h"

void TestScheme::testEncode(long logN, long logp, long L, long logSlots) {
	cout << "!!! START TEST ENCODE !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 1;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long mvecsize = (1 << logSlots);

	vector<CZZ> mvec = EvaluatorUtils::evaluateRandomVals(mvecsize, logp);

	Cipher cipher = scheme.fullEncrypt(mvec);
	vector<CZZ> dvec = scheme.fullDecrypt(cipher);

	StringUtils::showcompare(mvec, dvec, "val");

	cout << "!!! STOP TEST ENCODE !!!" << endl;
}

void TestScheme::testOperations(long logN, long logp, long L, long logSlots) {
	cout << "!!! START TEST OPERATIONS !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long slots = (1 << (logSlots-1));

	vector<CZZ> m1vec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	vector<CZZ> m2vec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	ZZ cnst = ZZ(324024);

	vector<CZZ> addvec, addcvec, multvec, multevec, multmsvec, multcvec;


	for (long i = 0; i < slots; ++i) {
		addvec.push_back(m1vec[i] + m2vec[i]);
		addcvec.push_back(m1vec[i] + cnst);
		multvec.push_back(m1vec[i] * m2vec[i]);
		multevec.push_back(m1vec[i] * m2vec[i]);
		multmsvec.push_back(m1vec[i] * m2vec[i] / params.p);
		multcvec.push_back(m1vec[i] * cnst);
	}

	Cipher c1 = scheme.fullEncrypt(m1vec);
	Cipher c2 = scheme.fullEncrypt(m2vec);
	Cipher cmulte = scheme.fullEncrypt(m1vec);

	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher cadd = scheme.add(c1, c2);
	timeutils.stop("add");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher caddc = scheme.addConst(c1, cnst);
	timeutils.stop("add");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult");
	Cipher cmult = scheme.mult(c1, c2);
	timeutils.stop("mult");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and equal");
	scheme.multAndEqual(cmulte, c2);
	timeutils.stop("mult and equal");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and mod switch");
	Cipher cmultms = scheme.multAndModSwitch(c1, c2);
	timeutils.stop("mult and mod switch");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult cnst");
	Cipher cmultc = scheme.multByConst(c1, cnst);
	timeutils.stop("mult cnst");
	cout << "------------------" << endl;

	vector<CZZ> d1vec = scheme.fullDecrypt(c1);
	vector<CZZ> d2vec = scheme.fullDecrypt(c2);
	vector<CZZ> daddvec = scheme.fullDecrypt(cadd);
	vector<CZZ> daddcvec = scheme.fullDecrypt(caddc);
	vector<CZZ> dmultvec = scheme.fullDecrypt(cmult);
	vector<CZZ> dmultevec = scheme.fullDecrypt(cmulte);
	vector<CZZ> dmultmsvec = scheme.fullDecrypt(cmultms);
	vector<CZZ> dmultcvec = scheme.fullDecrypt(cmultc);

	StringUtils::showcompare(m1vec, d1vec, "1vals");
	StringUtils::showcompare(m2vec, d2vec, "2vals");
	StringUtils::showcompare(addvec, daddvec, "add");
	StringUtils::showcompare(multvec, dmultvec, "1vals");
	StringUtils::showcompare(multcvec, dmultcvec, "1vals");
}
void TestScheme::testPowerOf2(long logN, long logp, long L, long logPowDegree) {
	cout << "!!! START TEST POWER OF 2!!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	CZZ m, m2, d2;

	m = params.ksiPows.pows[logN][1];

	m2 = m;
	for (long i = 0; i < logPowDegree; ++i) {
		m2 = (m2 * m2) >> params.logp;
	}

	vector<CZZ> mconj = scheme.doubleConjugate(m);
	Message msg = scheme.encode(mconj);
	Cipher c = scheme.encrypt(msg);

	cout << "------------------" << endl;
	timeutils.start("Power of 2");
	Cipher c2 = algo.powerOf2(c, logPowDegree);
	timeutils.stop("Power of 2");
	cout << "------------------" << endl;

	Message dmsg = scheme.decrypt(c2);
	vector<CZZ> ddconj = scheme.decode(dmsg);
	vector<CZZ> dvec = scheme.deConjugate(ddconj);
	d2 = dvec[0];

	cout << "m2: " << m2.toString() << endl;
	cout << "d2: " << d2.toString() << endl;
	cout << "e2: " << (m2 - d2).toString() << endl;

	cout << "!!! END TEST POWER OF 2 !!!" << endl;
}

void TestScheme::testPowerOf2Extended(long logN, long logp, long L, long logPowDegree) {
	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> m2k;
	vector<CZZ> d2k;
	vector<Cipher> c2k;
	CZZ m, m2;

	m = params.ksiPows.pows[logN][2];

	m2k.push_back(m);
	for (long i = 0; i < logPowDegree; ++i) {
		m2 = (m2k[i] * m2k[i]) >> params.logp;
		m2k.push_back(m2);
	}
	vector<CZZ> mconj = scheme.doubleConjugate(m);
	Message msg = scheme.encode(mconj);
	Cipher c = scheme.encrypt(msg);

	cout << "------------------" << endl;
	timeutils.start("Power of 2");
	algo.powerOf2Extended(c2k, c, logPowDegree);
	timeutils.stop("Power of 2");
	cout << "------------------" << endl;

	for (long i = 0; i < c2k.size(); ++i) {
		Message dmsg = scheme.decrypt(c2k[i]);
		vector<CZZ> ddconj = scheme.decode(dmsg);
		vector<CZZ> dvec = scheme.deConjugate(ddconj);
		d2k.push_back(dvec[0]);
	}

	for (long i = 0; i < logPowDegree + 1; ++i) {
		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i].toString() << endl;
		cout << "d: " << i << " " << d2k[i].toString() << endl;
		cout << "e: " << i << " " << (m2k[i] - d2k[i]).toString() << endl;
//		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;

}

void TestScheme::testPowerExtended(long logN, long logp, long L, long powDegree) {
	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> mpow;
	vector<CZZ> dpow;
	vector<Cipher> cpow;

	CZZ m = params.ksiPows.pows[logN][2];
	mpow.push_back(m);
	for (long i = 0; i < powDegree - 1; ++i) {
		CZZ mp = mpow[i] * m >> params.logp;
		mpow.push_back(mp);
	}
	vector<CZZ> mconj = scheme.doubleConjugate(m);
	Message msg = scheme.encode(mconj);
	Cipher c = scheme.encrypt(msg);

	cout << "------------------" << endl;
	timeutils.start("Power");
	algo.powerExtended(cpow, c, powDegree);
	timeutils.stop("Power");
	cout << "------------------" << endl;

	for (long i = 0; i < cpow.size(); ++i) {
		Message dmsg = scheme.decrypt(cpow[i]);
		vector<CZZ> ddconj = scheme.decode(dmsg);
		vector<CZZ> dvec = scheme.deConjugate(ddconj);
		dpow.push_back(dvec[0]);
	}

	for (long i = 0; i < powDegree; ++i) {
		cout << "------------------" << endl;
		cout << "m: " << i << " " << mpow[i].toString() << endl;
		cout << "d: " << i << " " << dpow[i].toString() << endl;
		cout << "e: " << i << " " << (mpow[i] - dpow[i]).toString() << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

}

void TestScheme::testProd2Extended(long logN, long logp, long L, long logProdDegree) {
	cout << "!!! START TEST PROD !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long degree = 1 << logProdDegree;

	CZZ m, e;
	vector<CZZ> ms;
	vector<vector<CZZ>> ms2k;
	vector<vector<CZZ>> ds2k;
	vector<Cipher> cs;
	vector<vector<Cipher>> cs2k;

	for (long i = 0; i < degree; ++i) {
		m = params.ksiPows.pows[logN][i % 3];
		vector<CZZ> mconj = scheme.doubleConjugate(m);
		Message msg = scheme.encode(mconj);
		Cipher c = scheme.encrypt(msg);
		ms.push_back(m);
		cs.push_back(c);
	}

	ms2k.push_back(ms);
	for (long i = 1; i < logProdDegree + 1; ++i) {
		vector<CZZ> m2k;
		long idx = 0;
		long m2ksize = ms2k[i-1].size();
		while(idx < m2ksize) {
			CZZ m2 = (ms2k[i - 1][idx] * ms2k[i - 1][idx+1]) >> params.logp;
			m2k.push_back(m2);
			idx += 2;
		}
		ms2k.push_back(m2k);
	}

	cout << "------------------" << endl;
	timeutils.start("Prod 2");
	algo.prod2Extended(cs2k, cs, logProdDegree);
	timeutils.stop("Prod 2");
	cout << "------------------" << endl;

	for (long i = 0; i < logProdDegree + 1; ++i) {
		vector<CZZ> d2k;
		for (long j = 0; j < cs2k[i].size(); ++j) {
			Message dmsg = scheme.decrypt(cs2k[i][j]);
			vector<CZZ> dconj = scheme.decode(dmsg);
			vector<CZZ> dvec = scheme.deConjugate(dconj);
			d2k.push_back(dvec[0]);
		}
		ds2k.push_back(d2k);
	}

	for (long i = 0; i < logProdDegree + 1; ++i) {
		for (long j = 0; j < cs2k[i].size(); ++j) {
			e = ms2k[i][j] - ds2k[i][j];
			cout << "------------------" << endl;
			cout << "m: " << i << " " << j << " " << ms2k[i][j].toString() << endl;
			cout << "d: " << i << " " << j << " " << ds2k[i][j].toString() << endl;
			cout << "e: " << i << " " << j << " " << e.toString() << endl;
//			cout << "eBnds: " << i << " " << j << " " << cs2k[i][j].eBnd << endl;
//			cout << "mBnds: " << i << " " << j << " " << cs2k[i][j].mBnd << endl;
			cout << "------------------" << endl;
		}
	}
	cout << "!!! END TEST PROD !!!" << endl;
}

void TestScheme::testInverseExtended(long logN, long logp, long L, long invSteps) {
	cout << "!!! START TEST INV !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long error = 13;

	ZZ halfp;
	CZZ m, mbar, minv;
	vector<CZZ> d2k;
	vector<Cipher> c2k, v2k;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;
	halfp = params.p / 2;


	Cipher c = scheme.fullEncrypt(mbar);

	cout << "------------------" << endl;
	timeutils.start("Inverse");
	algo.inverseExtended(c2k, v2k, c, invSteps);
	timeutils.stop("Inverse");
	cout << "------------------" << endl;

	for (long i = 0; i < v2k.size(); ++i) {
		vector<CZZ> dvec = scheme.fullDecrypt(v2k[i]);
		d2k.push_back(dvec[0]);
	}

	for (long i = 0; i < invSteps - 1; ++i) {
		cout << "------------------" << endl;
		cout << "minv:  " << i << " " << minv.toString() << endl;
		cout << "ds:    " << i << " " << d2k[i].toString() << endl;
		cout << "es:    " << i << " " << (minv - d2k[i]).toString() << endl;
//		cout << "eBnds: " << i << " " << v2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << v2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST INV !!!" << endl;
}

void TestScheme::testExponent(long logN, long logp, long L, long expDegree, long logSlots) {
	cout << "!!! START TEST EXPONENT !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> mvec, mexpvec;
	long slots = 1 << logSlots;

	for (long i = 0; i < slots; ++i) {
		double mr = (double)rand() / RAND_MAX;
		double mi = (double)rand() / RAND_MAX;
		CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
		CZZ mexp = EvaluatorUtils::evaluateExponent(mr, mi, logp);
		mvec.push_back(m);
		mexpvec.push_back(mexp);
	}

	Cipher c = scheme.fullEncrypt(mvec);

	string expName = "Exponent";
	cout << "------------------" << endl;
	timeutils.start(EXPONENT);
	Cipher cexp = algo.function(c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);
	cout << "----------------" << endl;

	vector<CZZ> dexpvec = scheme.fullDecrypt(cexp);

	for (long i = 0; i < dexpvec.size(); ++i) {
		cout << "------------------" << endl;
		cout << "mexp:  " << i << " " << mexpvec[i].toString() << endl;
		cout << "dexp:  " << i << " " << dexpvec[i].toString() << endl;
		cout << "eexp:  " << i << " " << (mexpvec[i] - dexpvec[i]).toString() << endl;
	}

	cout << "!!! END TEST EXPONENT !!!" << endl;
}

void TestScheme::testExponentExtended(long logN, long logp, long L, long expDegree) {
	cout << "!!! START TEST EXPONENT EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	double mr = 0.284345;
	double mi = 0.932432;

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ mexp = EvaluatorUtils::evaluateExponent(mr, mi, logp);


	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cexp;
	cout << "------------------" << endl;
	timeutils.start(EXPONENT);
	algo.functionExtended(cexp, c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);
	cout << "----------------" << endl;

	vector<CZZ> dexp;
	for (long i = 0; i < cexp.size(); ++i) {
		vector<CZZ> dvec = scheme.fullDecrypt(cexp[i]);
		dexp.push_back(dvec[0]);
	}

	for (long i = 0; i < dexp.size(); ++i) {
		cout << "------------------" << endl;
		cout << "mexp:  " << i << " " << mexp.toString() << endl;
		cout << "dexp:  " << i << " " << dexp[i].toString() << endl;
		cout << "eexp:  " << i << " " << (mexp - dexp[i]).toString() << endl;
	}

	cout << "!!! END TEST EXPONENT EXTENDED !!!" << endl;
}

void TestScheme::testSigmoid(long logN, long logp, long L, long sigmoidDegree, long logSlots) {
	cout << "!!! START TEST SIGMOID !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> mvec, msigmoidvec;

	long slots = 1 << logSlots;
	for (long i = 0; i < slots; ++i) {
		double mr = (double)rand() / RAND_MAX;
		double mi = (double)rand() / RAND_MAX;
		CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
		CZZ msigmoid = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);
		mvec.push_back(m);
		msigmoidvec.push_back(msigmoid);
	}


	Cipher c = scheme.fullEncrypt(mvec);

	cout << "------------------" << endl;
	timeutils.start(SIGMOID);
	Cipher csigmoid = algo.function(c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);
	cout << "----------------" << endl;

	vector<CZZ> dsigmoidvec = scheme.fullDecrypt(csigmoid);

	for (long i = 0; i < dsigmoidvec.size(); ++i) {
		cout << "------------------" << endl;
		cout << "msigmoid:  " << i << " " << msigmoidvec[i].toString() << endl;
		cout << "dsigmoid:  " << i << " " << dsigmoidvec[i].toString() << endl;
		cout << "esigmoid:  " << i << " " << (msigmoidvec[i] - dsigmoidvec[i]).toString() << endl;
	}

	cout << "!!! END TEST SIGMOID !!!" << endl;
}

void TestScheme::testSigmoidExtended(long logN, long logp, long L, long sigmoidDegree) {
	cout << "!!! START TEST SIGMOID EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	double mr = 0.284345;
	double mi = 0.932432;

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ msigmoid = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> csigmoid;
	cout << "------------------" << endl;
	timeutils.start(SIGMOID);
	algo.functionExtended(csigmoid, c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);
	cout << "----------------" << endl;

	vector<CZZ> dsigmoid;
	for (long i = 0; i < csigmoid.size(); ++i) {
		vector<CZZ> dvec = scheme.fullDecrypt(csigmoid[i]);
		dsigmoid.push_back(dvec[0]);
	}

	for (long i = 0; i < dsigmoid.size(); ++i) {
		cout << "------------------" << endl;
		cout << "msigmoid:  " << i << " " << msigmoid.toString() << endl;
		cout << "dsigmoid:  " << i << " " << dsigmoid[i].toString() << endl;
		cout << "esigmoid:  " << i << " " << (msigmoid - dsigmoid[i]).toString() << endl;
	}

	cout << "!!! END TEST SIGMOID EXTENDED !!!" << endl;
}

void TestScheme::testFFT(long logN, long logp, long L, long logFFTdim, long FFTdeg) {
	cout << "!!! START TEST FFT !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logl = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logN, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long FFTdim = 1 << logFFTdim;

	CZZ zero, tmp;

	vector<CZZ> mp1, mp2, mpx;
	vector<CZZ> mfft1, mfft2, mfftx;
	vector<Cipher> cp1, cp2, cpx;
	vector<Cipher> cfft1, cfft2;
//	vector<Cipher> cfftbut1, cfftbut2;
	vector<CZZ> dpx;

	for (long i = 0; i < FFTdeg; ++i) {
		mp1.push_back(params.ksiPows.pows[logN][i % 3]);
		mp2.push_back(params.ksiPows.pows[logN][i % 2]);
	}

	for (long i = FFTdeg; i < FFTdim / 2; ++i) {
		mp1.push_back(zero);
		mp2.push_back(zero);
	}

	mp1 = scheme.doubleConjugate(mp1);
	mp2 = scheme.doubleConjugate(mp2);

	mfft1 = NumUtils::fft(mp1, params.ksiPows);
	mfft2 = NumUtils::fft(mp2, params.ksiPows);

	for (long i = 0; i < FFTdim; ++i) {
		tmp = (mfft1[i] * mfft2[i]) >> params.logp;
		mfftx.push_back(tmp);
	}

	mpx = NumUtils::fftInv(mfftx, params.ksiPows);

	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < FFTdim; ++i) {
		vector<CZZ> mconj1 = scheme.doubleConjugate(mp1[i]);
		vector<CZZ> mconj2 = scheme.doubleConjugate(mp2[i]);

		Message msgp1 = scheme.encode(mconj1);
		Message msgp2 = scheme.encode(mconj2);

		Cipher c1 = scheme.encrypt(msgp1);
		Cipher c2 = scheme.encrypt(msgp2);

		cp1.push_back(c1);
		cp2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 1");
	cfft1 = algo.fft(cp1);
	timeutils.stop("cfft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 2");
	cfft2 = algo.fft(cp2);
	timeutils.stop("cfft 2");
	cout << "------------------" << endl;


//	cout << "------------------" << endl;
//	timeutils.start("cfft but 1");
//	cfftbut1 = algo.fftButterfly(cp1);
//	timeutils.stop("cfft but 1");
//	cout << "------------------" << endl;
//
//	cout << "------------------" << endl;
//	timeutils.start("cfft but 2");
//	cfftbut2 = algo.fftButterfly(cp2);
//	timeutils.stop("cfft but 2");
//	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul fft");
	for (long i = 0; i < FFTdim; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("mul fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftx inv");
	cpx = algo.fftInv(cfft1);
	timeutils.stop("cfftx inv");
	cout << "------------------" << endl;


	for (long i = 0; i < FFTdim; ++i) {
		Message dx = scheme.decrypt(cpx[i]);
		vector<CZZ> dconj = scheme.decode(dx);
		vector<CZZ> dmp = scheme.deConjugate(dconj);
		dpx.push_back(dmp[0]);
	}

	for (long i = 0; i < FFTdim; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: mpx    = " << mpx[i].toString() << endl;
		cout << i << " step: dpx    = " << dpx[i].toString() << endl;
		cout << i << " step: epx    = " << (mpx[i]-dpx[i]).toString() << endl;


		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl;
}
