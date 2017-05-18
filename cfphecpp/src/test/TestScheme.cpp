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

void TestScheme::testEncodeBatch(long logN, long logl, long logp, long L, long logSlots) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long mvecsize = (1 << logSlots);
	vector<CZZ> mvec, dvec;
	EvaluatorUtils::evaluateRandomVals(mvec, mvecsize, logp);

	Cipher cipher = scheme.fullEncrypt(mvec);
	dvec = scheme.fullDecrypt(cipher);

	StringUtils::showcompare(mvec, dvec, "val");

	cout << "!!! STOP TEST ENCODE BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerOf2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots) {
	cout << "!!! START TEST POWER OF 2 BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	vector<CZZ> mvec, mpow;

	long slots = 1 << logSlots;
	EvaluatorUtils::evaluateRandomCircleValsAndPows2(mvec, mpow, slots, logDegree, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start("Power of 2");
	Cipher cpow = algo.powerOf2(c, logDegree);
	timeutils.stop("Power of 2");

	CZZ dpow = scheme.fullSingleDecrypt(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testPowerOf2Extended(long logN, long logl, long logp, long L, long logDegree) {
	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	double angle = (double)arc4random() / RAND_MAX;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	vector<CZZ> mpow = EvaluatorUtils::evaluatePow2vec(mr, mi, logDegree, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cpow;
	timeutils.start("Power of 2");
	algo.powerOf2Extended(cpow, c, logDegree);
	timeutils.stop("Power of 2");

	vector<CZZ> dpow = scheme.fullSingleDecryptVec(cpow);

	StringUtils::showcompare(mpow, dpow, "pow2");
	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! END TEST POWER BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long slots = 1 << logSlots;

	vector<CZZ> mvec, mpow;
	EvaluatorUtils::evaluateRandomCircleValsAndPows(mvec, mpow, slots, degree, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start("Power");
	Cipher cpow = algo.power(c, degree);
	timeutils.stop("Power");

	vector<CZZ> dpow = scheme.fullDecrypt(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER BATCH !!!" << endl;

}

void TestScheme::testPowerExtended(long logN, long logl, long logp, long L, long degree) {
	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	double angle = (double)arc4random() / RAND_MAX;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	vector<CZZ> mpow = EvaluatorUtils::evaluatePowvec(mr, mi, degree, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cpow;
	timeutils.start("Power");
	algo.powerExtended(cpow, c, degree);
	timeutils.stop("Power");

	vector<CZZ> dpow = scheme.fullSingleDecryptVec(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

}

//-----------------------------------------

void TestScheme::testProd2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots) {
	cout << "!!! START TEST PROD 2 BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long slots = 1 << logSlots;
	long degree = 1 << logDegree;

	vector<Cipher> cvec;
	vector<vector<CZZ>> mvec;
	vector<CZZ> pvec;

	EvaluatorUtils::evaluateRandomCircleValsAndProduct(mvec, pvec, slots, degree, logp);

	for (long i = 0; i < degree; ++i) {
		Cipher c = scheme.fullEncrypt(mvec[i]);
		cvec.push_back(c);
	}
	timeutils.start("Prod 2");
	Cipher cprod = algo.prod2(cvec, logDegree);
	timeutils.stop("Prod 2");

	vector<CZZ> dvec = scheme.fullDecrypt(cprod);

	StringUtils::showcompare(pvec, dvec, "prod");

	cout << "!!! END TEST PROD 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots) {
	cout << "!!! START TEST INVERSE !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long slots = 1 << logSlots;

	vector<CZZ> mvec, minv;

	EvaluatorUtils::evaluateRandomCircleBarValsAndInverses(mvec, minv, slots, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start("Inverse");
	Cipher cinv = algo.inverse(c, invSteps);
	timeutils.stop("Inverse");

	vector<CZZ> dinv = scheme.fullDecrypt(cinv);

	StringUtils::showcompare(minv, dinv, "inv");

	cout << "!!! END TEST INVERSE !!!" << endl;
}

void TestScheme::testInverseExtended(long logN, long logl, long logp, long L, long invSteps) {
	cout << "!!! START TEST INVERSE EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	double angle = (double)arc4random() / RAND_MAX / 50;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);

	CZZ mbar = EvaluatorUtils::evaluateVal(1 - mr, -mi, logp);
	CZZ minv = EvaluatorUtils::evaluateInverse(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(mbar);

	vector<Cipher> cinv;
	timeutils.start("Inverse extended");
	algo.inverseExtended(cinv, c, invSteps);
	timeutils.stop("Inverse extended");

	vector<CZZ> dinv = scheme.fullSingleDecryptVec(cinv);

	StringUtils::showcompare(minv, dinv, "inv");

	cout << "!!! END TEST INVERSE EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testExponentBatch(long logN, long logl, long logp, long L, long expDegree, long logSlots) {
	cout << "!!! START TEST EXPONENT !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long slots = 1 << logSlots;

	vector<CZZ> mvec, fvec;

	EvaluatorUtils::evaluateRandomValsAndExponents(mvec, fvec, slots, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start(EXPONENT);
	Cipher cexp = algo.function(c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dvec = scheme.fullDecrypt(cexp);

	StringUtils::showcompare(fvec, dvec, EXPONENT);

	cout << "!!! END TEST EXPONENT !!!" << endl;
}

void TestScheme::testLazyExponentBatch(long logN, long logl, long logp, long L, long expDegree, long logSlots) {
	cout << "!!! START TEST EXPONENT SIMPLE !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long slots = 1 << logSlots;

	vector<CZZ> mvec, fvec;
	EvaluatorUtils::evaluateRandomValsAndExponents(mvec, fvec, slots, logp);
	EvaluatorUtils::leftShift(fvec, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start(EXPONENT);
	Cipher cexp = algo.functionLazy(c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dvec = scheme.fullDecrypt(cexp);

	StringUtils::showcompare(fvec, dvec, EXPONENT);

	cout << "!!! END TEST EXPONENT SIMPLE !!!" << endl;
}

void TestScheme::testExponentExtended(long logN, long logl, long logp, long L, long expDegree) {
	cout << "!!! START TEST EXPONENT EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);

	//----------------------------

	double mr = (double)arc4random() / RAND_MAX;
	double mi = (double)arc4random() / RAND_MAX;

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ mexp = EvaluatorUtils::evaluateExponent(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cexp;
	timeutils.start(EXPONENT);
	algo.functionExtended(cexp, c, EXPONENT, expDegree);
	timeutils.stop(EXPONENT);
	vector<CZZ> dexp = scheme.fullSingleDecryptVec(cexp);

	StringUtils::showcompare(mexp, dexp, EXPONENT);

	cout << "!!! END TEST EXPONENT EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoidBatch(long logN, long logl, long logp, long L, long sigmoidDegree, long logSlots) {
	cout << "!!! START TEST SIGMOID !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> mvec, fvec;

	long slots = 1 << logSlots;

	EvaluatorUtils::evaluateRandomValsAndSigmoids(mvec, fvec, slots, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start(SIGMOID);
	Cipher csigmoid = algo.function(c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dvec = scheme.fullDecrypt(csigmoid);

	StringUtils::showcompare(fvec, dvec, SIGMOID);

	cout << "!!! END TEST SIGMOID !!!" << endl;
}

void TestScheme::testLazySigmoidBatch(long logN, long logl, long logp, long L, long sigmoidDegree, long logSlots) {
	cout << "!!! START TEST SIGMOID SIMPLE !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> mvec, fvec;

	long slots = 1 << logSlots;
	EvaluatorUtils::evaluateRandomValsAndSigmoids(mvec, fvec, slots, logp);
	EvaluatorUtils::leftShift(fvec, logp);

	Cipher c = scheme.fullEncrypt(mvec);

	timeutils.start(SIGMOID);
	Cipher csigmoid = algo.functionLazy(c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dvec = scheme.fullDecrypt(csigmoid);

	StringUtils::showcompare(fvec, dvec, SIGMOID);

	cout << "!!! END TEST SIGMOID SIMPLE !!!" << endl;
}

void TestScheme::testSigmoidExtended(long logN, long logl, long logp, long L, long sigmoidDegree) {
	cout << "!!! START TEST SIGMOID EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	double mr = (double)arc4random() / RAND_MAX;
	double mi = (double)arc4random() / RAND_MAX;

	CZZ m = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ msigmoid = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> csigmoid;

	timeutils.start(SIGMOID);
	algo.functionExtended(csigmoid, c, SIGMOID, sigmoidDegree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dsigmoid = scheme.fullSingleDecryptVec(csigmoid);

	StringUtils::showcompare(msigmoid, dsigmoid, SIGMOID);

	cout << "!!! END TEST SIGMOID EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testFFT(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg) {
	cout << "!!! START TEST FFT !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long FFTdim = 1 << logFFTdim;

	vector<CZZ> mp1, mp2, mfft1, mfft2, mfftx;
	vector<Cipher> cfft1, cfft2;

	EvaluatorUtils::evaluateRandomVals(mp1, FFTdeg, logp);
	EvaluatorUtils::evaluateRandomVals(mp2, FFTdeg, logp);

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

	vector<Cipher> cp1 = scheme.fullSingleEncryptVec(mp1);
	vector<Cipher> cp2 = scheme.fullSingleEncryptVec(mp2);

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

	vector<CZZ> dpx = scheme.fullSingleDecryptVec(cpx);

//	StringUtils::showcompare(mpx, dpx, "fft");

	cout << "!!! END TEST FFT !!!" << endl;
}

void TestScheme::testLazyFFT(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg) {
	cout << "!!! START TEST FFT SIMPLE !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long FFTdim = 1 << logFFTdim;

	vector<CZZ> mp1, mp2, mfft1, mfft2, mfftx;
	vector<Cipher> cfft1, cfft2;

	EvaluatorUtils::evaluateRandomVals(mp1, FFTdeg, logp);
	EvaluatorUtils::evaluateRandomVals(mp2, FFTdeg, logp);

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

	vector<CZZ> mpx = NumUtils::fftInvLazy(mfftx, params.ksiPows);

	vector<Cipher> cp1 = scheme.fullSingleEncryptVec(mp1);
	vector<Cipher> cp2 = scheme.fullSingleEncryptVec(mp2);

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
	vector<Cipher> cpx = algo.fftInvLazy(cfft1);
	timeutils.stop("cfftx inv");

	vector<CZZ> dpx = scheme.fullSingleDecryptVec(cpx);

	StringUtils::showcompare(mpx, dpx, "fft");

	cout << "!!! END TEST FFT SIMPLE !!!" << endl;
}
