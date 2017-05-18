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
	cout << "!!! START TEST ENCODE !!!" << endl;

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

	cout << "!!! STOP TEST ENCODE !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerOf2(long logN, long logl, long logp, long L, long logPowDegree) {
	cout << "!!! START TEST POWER OF 2 !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

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

void TestScheme::testPowerOf2Extended(long logN, long logl, long logp, long L, long logPowDegree) {
	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

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

void TestScheme::testPowerExtended(long logN, long logl, long logp, long L, long powDegree) {
	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

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

void TestScheme::testProd2(long logN, long logl, long logp, long L, long logProdDegree) {
	cout << "!!! START TEST PROD 2 !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long prodDegree = 1 << logProdDegree;

	vector<CZZ> mvec;
	CZZ mprod;

	EvaluatorUtils::evaluateRandomValsAndProduct(mvec, mprod, prodDegree, logp);

	vector<Cipher> cvec = scheme.fullSimpleEncryptVec(mvec);

	timeutils.start("Prod 2");
	Cipher cprod = algo.prod2(cvec, logProdDegree);
	timeutils.stop("Prod 2");

	CZZ dprod = scheme.fullSimpleDecrypt(cprod);

	StringUtils::showcompare(mprod, dprod, "prod");

	cout << "!!! END TEST PROD 2 !!!" << endl;
}
//-----------------------------------------

void TestScheme::testProd2Extended(long logN, long logl, long logp, long L, long logProdDegree) {
	cout << "!!! START TEST PROD 2 EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	long degree = 1 << logProdDegree;

	vector<vector<CZZ>> ms2k;
	vector<vector<CZZ>> ds2k;
	vector<vector<Cipher>> cs2k;

	vector<CZZ> ms;
	EvaluatorUtils::evaluateRandomVals(ms, degree, logp);
	vector<Cipher> cs = scheme.fullSimpleEncryptVec(ms);

	ms2k.push_back(ms);
	for (long i = 0; i < logProdDegree; ++i) {
		vector<CZZ> m2k;
		long size = ms2k[i].size();
		for (long j = 0; j < size; j = j + 2) {
			CZZ m2 = (ms2k[i][j] * ms2k[i][j+1]) >> params.logp;
			m2k.push_back(m2);
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
//		StringUtils::showcompare(ms2k[i], ds2k[i], "pow");
	}
	cout << "!!! END TEST PROD 2 EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverse(long logN, long logl, long logp, long L, long invSteps) {
	cout << "!!! START TEST INVERSE !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L, 3, 0.5, 64);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	double mi = (double)rand() / RAND_MAX;
	double mr = sqrt(1 -mi * mi);

	CZZ m = EvaluatorUtils::evaluateVal(1 - mr, mi, logp);
	CZZ minv = EvaluatorUtils::evaluateInverse(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	timeutils.start("Inverse");
	Cipher cinv = algo.inverse(c, invSteps);
	timeutils.stop("Inverse");

	CZZ dinv = scheme.fullSimpleDecrypt(cinv);

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

	double mi = (double)rand() / RAND_MAX;
	double mr = (double)rand() / RAND_MAX;

	CZZ m = EvaluatorUtils::evaluateVal(1 - mr, mi, logp);
	CZZ minv = EvaluatorUtils::evaluateInverse(mr, mi, logp);

	Cipher c = scheme.fullEncrypt(m);

	vector<Cipher> cinv;
	timeutils.start("Inverse extended");
	algo.inverseExtended(cinv, c, invSteps);
	timeutils.stop("Inverse extended");

	vector<CZZ> dinv = scheme.fullSimpleDecryptVec(cinv);

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

void TestScheme::testExponentSimpleBatch(long logN, long logl, long logp, long L, long expDegree, long logSlots) {
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
	Cipher cexp = algo.functionSimple(c, EXPONENT, expDegree);
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

void TestScheme::testSigmoidSimpleBatch(long logN, long logl, long logp, long L, long sigmoidDegree, long logSlots) {
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
	Cipher csigmoid = algo.functionSimple(c, SIGMOID, sigmoidDegree);
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

//	StringUtils::showcompare(mpx, dpx, "fft");

	cout << "!!! END TEST FFT !!!" << endl;
}

void TestScheme::testFFTsimple(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg) {
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

	vector<CZZ> mpx = NumUtils::fftInvSimple(mfftx, params.ksiPows);

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
	vector<Cipher> cpx = algo.fftInvSimple(cfft1);
	timeutils.stop("cfftx inv");

	vector<CZZ> dpx = scheme.fullSimpleDecryptVec(cpx);

	StringUtils::showcompare(mpx, dpx, "fft");

	cout << "!!! END TEST FFT SIMPLE !!!" << endl;
}
