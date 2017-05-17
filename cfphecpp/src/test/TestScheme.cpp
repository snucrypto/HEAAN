#include "TestScheme.h"

#include <NTL/ZZ.h>
#include <cstdlib>
#include <string>
#include <vector>

#include "../czz/CZZ.h"
#include "../scheme/Cipher.h"
#include "../scheme/Message.h"
#include "../scheme/Params.h"
#include "../scheme/PubKey.h"
#include "../scheme/Scheme.h"
#include "../scheme/SchemeAlgo.h"
#include "../scheme/SecKey.h"
#include "../utils/EvaluatorUtils.h"
#include "../utils/KsiPows.h"
#include "../utils/NumUtils.h"
#include "../utils/StringUtils.h"
#include "../utils/TaylorPows.h"
#include "../utils/TimeUtils.h"

//-----------------------------------------

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

//-----------------------------------------

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

	timeutils.start("add");
	Cipher cadd = scheme.add(c1, c2);
	timeutils.stop("add");

	timeutils.start("add const");
	Cipher caddc = scheme.addConst(c1, cnst);
	timeutils.stop("add const");

	timeutils.start("mult");
	Cipher cmult = scheme.mult(c1, c2);
	timeutils.stop("mult");

	timeutils.start("mult and equal");
	scheme.multAndEqual(cmulte, c2);
	timeutils.stop("mult and equal");

	timeutils.start("mult and mod switch");
	Cipher cmultms = scheme.multAndModSwitch(c1, c2);
	timeutils.stop("mult and mod switch");

	timeutils.start("mult cnst");
	Cipher cmultc = scheme.multByConst(c1, cnst);
	timeutils.stop("mult cnst");

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

	cout << "!!! START TEST OPERATIONS !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerOf2(long logN, long logp, long L, long logPowDegree) {
	cout << "!!! START TEST POWER OF 2 !!!" << endl;

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

	double mr = (double)rand() / RAND_MAX;
	double mi = sqrt(1 - mr * mr);

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ mpow = EvaluatorUtils::evaluatePow2(mr, mi, logPowDegree, logp);

	Cipher c = scheme.fullEncrypt(m);

	timeutils.start("Power of 2");
	Cipher cpow = algo.powerOf2(c, logPowDegree);
	timeutils.stop("Power of 2");

	CZZ dpow = scheme.fullSimpleDecrypt(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

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

	double mr = (double)rand() / RAND_MAX;
	double mi = sqrt(1 - mr * mr);

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	vector<CZZ> mpow = EvaluatorUtils::evaluatePow2vec(mr, mi, logPowDegree, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cpow;
	timeutils.start("Power of 2");
	algo.powerOf2Extended(cpow, c, logPowDegree);
	timeutils.stop("Power of 2");

	vector<CZZ> dpow = scheme.fullSimpleDecryptVec(cpow);

	StringUtils::showcompare(mpow, dpow, "pow2");

	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;

}

//-----------------------------------------

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

	double mr = (double)rand() / RAND_MAX;
	double mi = sqrt(1 - mr * mr);

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);

	vector<CZZ> mpow = EvaluatorUtils::evaluatePowvec(mr, mi, powDegree, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cpow;
	timeutils.start("Power");
	algo.powerExtended(cpow, c, powDegree);
	timeutils.stop("Power");

	vector<CZZ> dpow = scheme.fullSimpleDecryptVec(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

}

//-----------------------------------------

void TestScheme::testProd2(long logN, long logp, long L, long logProdDegree) {
	cout << "!!! START TEST PROD 2 !!!" << endl;

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

	vector<vector<CZZ>> ms2k;
	vector<vector<CZZ>> ds2k;
	vector<vector<Cipher>> cs2k;

	vector<CZZ> ms = EvaluatorUtils::evaluateRandomVals(degree, logp);
	vector<Cipher> cs = scheme.fullSimpleEncryptVec(ms);

	CZZ mprod = ms[0];
	for (long i = 1; i < degree; ++i) {
		mprod *= ms[i];
		mprod <<= logp;
	}

	timeutils.start("Prod 2");
	Cipher cprod = algo.prod2(cs, logProdDegree);
	timeutils.stop("Prod 2");

	CZZ dprod = scheme.fullSimpleDecrypt(cprod);

	StringUtils::showcompare(mprod, dprod, "prod");

	cout << "!!! END TEST PROD 2 !!!" << endl;
}
//-----------------------------------------

void TestScheme::testProd2Extended(long logN, long logp, long L, long logProdDegree) {
	cout << "!!! START TEST PROD 2 EXTENDED !!!" << endl;

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

	vector<vector<CZZ>> ms2k;
	vector<vector<CZZ>> ds2k;
	vector<vector<Cipher>> cs2k;

	vector<CZZ> ms = EvaluatorUtils::evaluateRandomVals(degree, logp);
	vector<Cipher> cs = scheme.fullSimpleEncryptVec(ms);

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

	timeutils.start("Prod 2");
	algo.prod2Extended(cs2k, cs, logProdDegree);
	timeutils.stop("Prod 2");

	for (long i = 0; i < logProdDegree + 1; ++i) {
		vector<CZZ> d2k = scheme.fullSimpleDecryptVec(cs2k[i]);
		ds2k.push_back(d2k);
	}

	for (long i = 0; i < logProdDegree + 1; ++i) {
		StringUtils::showcompare(ms2k[i], ds2k[i], "pow");
	}
	cout << "!!! END TEST PROD 2 EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverse(long logN, long logp, long L, long invSteps) {
	cout << "!!! START TEST INVERSE !!!" << endl;

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
	vector<Cipher> cpow, cinv;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;
	halfp = params.p / 2;

	Cipher c = scheme.fullEncrypt(mbar);

	timeutils.start("Inverse");
	algo.inverseExtended(cpow, cinv, c, invSteps);
	timeutils.stop("Inverse");

	vector<CZZ> dinv = scheme.fullSimpleDecryptVec(cinv);

	StringUtils::showcompare(minv, dinv, "inv");

	cout << "!!! END TEST INVERSE !!!" << endl;
}

void TestScheme::testInverseExtended(long logN, long logp, long L, long invSteps) {
	cout << "!!! START TEST INVERSE EXTENDED !!!" << endl;

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
	vector<Cipher> cpow, cinv;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;
	halfp = params.p / 2;

	Cipher c = scheme.fullEncrypt(mbar);

	timeutils.start("Inverse extended");
	algo.inverseExtended(cpow, cinv, c, invSteps);
	timeutils.stop("Inverse extended");

	vector<CZZ> dinv = scheme.fullSimpleDecryptVec(cinv);

	StringUtils::showcompare(minv, dinv, "inv");

	cout << "!!! END TEST INVERSE EXTENDED !!!" << endl;
}

//-----------------------------------------

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

	long slots = 1 << logSlots;

	vector<CZZ> mvec, mexpvec;
	for (long i = 0; i < slots; ++i) {
		double mr = (double)rand() / RAND_MAX;
		double mi = (double)rand() / RAND_MAX;
		CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
		CZZ mexp = EvaluatorUtils::evaluateExponent(mr, mi, logp);
		mvec.push_back(m);
		mexpvec.push_back(mexp);
	}

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start(EXPONENT);
	Cipher cexp = algo.function(c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dexpvec = scheme.fullDecrypt(cexp);

	StringUtils::showcompare(mexpvec, dexpvec, "exp");

	cout << "!!! END TEST EXPONENT !!!" << endl;
}

void TestScheme::testExponentExtended(long logN, long logp, long L, long expDegree) {
	cout << "!!! START TEST EXPONENT EXTENDED !!!" << endl;

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

	double mr = (double)rand() / RAND_MAX;
	double mi = (double)rand() / RAND_MAX;

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ mexp = EvaluatorUtils::evaluateExponent(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cexp;

	timeutils.start(EXPONENT);
	algo.functionExtended(cexp, c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dexp = scheme.fullSimpleDecryptVec(cexp);

	StringUtils::showcompare(mexp, dexp, "exp");

	cout << "!!! END TEST EXPONENT EXTENDED !!!" << endl;
}

//-----------------------------------------

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

	timeutils.start(SIGMOID);
	Cipher csigmoid = algo.function(c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dsigmoidvec = scheme.fullDecrypt(csigmoid);

	StringUtils::showcompare(msigmoidvec, dsigmoidvec, "sigmoid");

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

	double mr = (double)rand() / RAND_MAX;
	double mi = (double)rand() / RAND_MAX;

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ msigmoid = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> csigmoid;

	timeutils.start(SIGMOID);
	algo.functionExtended(csigmoid, c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dsigmoid = scheme.fullSimpleDecryptVec(csigmoid);

	StringUtils::showcompare(msigmoid, dsigmoid, "sigmoid");

	cout << "!!! END TEST SIGMOID EXTENDED !!!" << endl;
}

//-----------------------------------------

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

	vector<CZZ> mfft1, mfft2, mfftx;
	vector<Cipher> cfft1, cfft2;

	vector<CZZ> mp1 = EvaluatorUtils::evaluateRandomVals(FFTdeg, logp);
	vector<CZZ> mp2 = EvaluatorUtils::evaluateRandomVals(FFTdeg, logp);

	CZZ zero;
	for (long i = FFTdeg; i < FFTdim / 2; ++i) {
		mp1.push_back(zero);
		mp2.push_back(zero);
	}

	mp1 = scheme.doubleConjugate(mp1);
	mp2 = scheme.doubleConjugate(mp2);

	mfft1 = NumUtils::fft(mp1, params.ksiPows);
	mfft2 = NumUtils::fft(mp2, params.ksiPows);

	for (long i = 0; i < FFTdim; ++i) {
		CZZ tmp = (mfft1[i] * mfft2[i]) >> params.logp;
		mfftx.push_back(tmp);
	}

	vector<CZZ> mpx = NumUtils::fftInv(mfftx, params.ksiPows);

	vector<Cipher> cp1 = scheme.fullSimpleEncryptVec(mp1);
	vector<Cipher> cp2 = scheme.fullSimpleEncryptVec(mp2);

	timeutils.start("cfft 1");
	cfft1 = algo.fft(cp1);
	timeutils.stop("cfft 1");

	timeutils.start("cfft 2");
	cfft2 = algo.fft(cp2);
	timeutils.stop("cfft 2");

	timeutils.start("mul fft");
	for (long i = 0; i < FFTdim; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("mul fft");

	timeutils.start("cfftx inv");
	vector<Cipher> cpx = algo.fftInv(cfft1);
	timeutils.stop("cfftx inv");

	vector<CZZ> dpx = scheme.fullSimpleDecryptVec(cpx);

	StringUtils::showcompare(mpx, dpx, "fft");

	cout << "!!! END TEST FFT !!!" << endl;
}
