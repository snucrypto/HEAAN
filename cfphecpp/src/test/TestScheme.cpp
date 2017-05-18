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
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);

	vector<CZZ> mvec;
	EvaluatorUtils::evaluateRandomVals(mvec, slots, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	vector<CZZ> dvec = scheme.decryptFull(cipher);

	StringUtils::showcompare(mvec, dvec, "val");

	cout << "!!! STOP TEST ENCODE BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerOf2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots) {
	cout << "!!! START TEST POWER OF 2 BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, mpow;
	EvaluatorUtils::evaluateRandomCircleValsAndPows2(mvec, mpow, slots, logDegree, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start("Power of 2");
	Cipher cpow = algo.powerOf2(cipher, logDegree);
	timeutils.stop("Power of 2");

	vector<CZZ> dpow = scheme.decryptFull(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testPowerOf2Extended(long logN, long logl, long logp, long L, long logDegree) {
	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	double angle = (double)arc4random() / RAND_MAX;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);

	CZZ mval = EvaluatorUtils::evaluateVal(mr, mi, logp);
	vector<CZZ> mpow = EvaluatorUtils::evaluatePow2vec(mr, mi, logDegree, logp);

	Cipher cipher = scheme.encryptFull(mval);

	vector<Cipher> cpow;
	timeutils.start("Power of 2");
	algo.powerOf2Extended(cpow, cipher, logDegree);
	timeutils.stop("Power of 2");

	vector<CZZ> dpow = scheme.decryptFullSingleVec(cpow);

	StringUtils::showcompare(mpow, dpow, "pow2");

	cout << "!!! END TEST POWER OF 2 EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! END TEST POWER BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, mpow;
	EvaluatorUtils::evaluateRandomCircleValsAndPows(mvec, mpow, slots, degree, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start("Power");
	Cipher cpow = algo.power(cipher, degree);
	timeutils.stop("Power");

	vector<CZZ> dpow = scheme.decryptFull(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER BATCH !!!" << endl;

}

void TestScheme::testPowerExtended(long logN, long logl, long logp, long L, long degree) {
	cout << "!!! END TEST POWER EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------

	double angle = (double)arc4random() / RAND_MAX;
	double mr = cos(angle * 2 * Pi);
	double mi = sin(angle * 2 * Pi);

	CZZ mval = EvaluatorUtils::evaluateVal(mr, mi, logp);
	vector<CZZ> mpow = EvaluatorUtils::evaluatePowvec(mr, mi, degree, logp);

	Cipher cipher = scheme.encryptFull(mval);

	vector<Cipher> cpow;
	timeutils.start("Power");
	algo.powerExtended(cpow, cipher, degree);
	timeutils.stop("Power");

	vector<CZZ> dpow = scheme.decryptFullSingleVec(cpow);

	StringUtils::showcompare(mpow, dpow, "pow");

	cout << "!!! END TEST POWER EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testProd2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots) {
	cout << "!!! START TEST PROD 2 BATCH !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
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
		Cipher cipher = scheme.encryptFull(mvec[i]);
		cvec.push_back(cipher);
	}

	timeutils.start("Prod 2");
	Cipher cprod = algo.prod2(cvec, logDegree);
	timeutils.stop("Prod 2");

	vector<CZZ> dvec = scheme.decryptFull(cprod);

	StringUtils::showcompare(pvec, dvec, "prod");

	cout << "!!! END TEST PROD 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots) {
	cout << "!!! START TEST INVERSE !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, minv;

	EvaluatorUtils::evaluateRandomCircleBarValsAndInverses(mvec, minv, slots, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start("Inverse");
	Cipher cinv = algo.inverse(cipher, invSteps);
	timeutils.stop("Inverse");

	vector<CZZ> dinv = scheme.decryptFull(cinv);

	StringUtils::showcompare(minv, dinv, "inv");

	cout << "!!! END TEST INVERSE !!!" << endl;
}

void TestScheme::testInverseExtended(long logN, long logl, long logp, long L, long invSteps) {
	cout << "!!! START TEST INVERSE EXTENDED !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
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

	Cipher cipher = scheme.encryptFull(mbar);

	vector<Cipher> cinv;
	timeutils.start("Inverse extended");
	algo.inverseExtended(cinv, cipher, invSteps);
	timeutils.stop("Inverse extended");

	vector<CZZ> dinv = scheme.decryptFullSingleVec(cinv);

	StringUtils::showcompare(minv, dinv, "inv");

	cout << "!!! END TEST INVERSE EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testExponentBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, mexp;
	EvaluatorUtils::evaluateRandomValsAndExponents(mvec, mexp, slots, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start(EXPONENT);
	Cipher cexp = algo.function(cipher, EXPONENT, degree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dexp = scheme.decryptFull(cexp);

	StringUtils::showcompare(mexp, dexp, EXPONENT);

	cout << "!!! END TEST EXPONENT !!!" << endl;
}

void TestScheme::testLazyExponentBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT LAZY !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, mexp;
	EvaluatorUtils::evaluateRandomValsAndExponents(mvec, mexp, slots, logp);
	EvaluatorUtils::leftShift(mexp, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start(EXPONENT);
	Cipher cexp = algo.functionLazy(cipher, EXPONENT, degree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dexp = scheme.decryptFull(cexp);

	StringUtils::showcompare(mexp, dexp, EXPONENT);

	cout << "!!! END TEST EXPONENT LAZY !!!" << endl;
}

void TestScheme::testExponentExtended(long logN, long logl, long logp, long L, long degree) {
	cout << "!!! START TEST EXPONENT EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);

	//----------------------------

	double mr = (double)arc4random() / RAND_MAX;
	double mi = (double)arc4random() / RAND_MAX;

	CZZ mval = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ mexp = EvaluatorUtils::evaluateExponent(mr, mi, logp);

	Cipher cipher = scheme.encryptFull(mval);

	vector<Cipher> cexp;

	timeutils.start(EXPONENT);
	algo.functionExtended(cexp, cipher, EXPONENT, degree);
	timeutils.stop(EXPONENT);

	vector<CZZ> dexp = scheme.decryptFullSingleVec(cexp);

	StringUtils::showcompare(mexp, dexp, EXPONENT);

	cout << "!!! END TEST EXPONENT EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoidBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, msig;
	EvaluatorUtils::evaluateRandomValsAndSigmoids(mvec, msig, slots, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start(SIGMOID);
	Cipher csig = algo.function(cipher, SIGMOID, degree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dsig = scheme.decryptFull(csig);

	StringUtils::showcompare(msig, dsig, SIGMOID);

	cout << "!!! END TEST SIGMOID !!!" << endl;
}

void TestScheme::testLazySigmoidBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID LAZY !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------
	long slots = 1 << logSlots;

	vector<CZZ> mvec, msig;
	EvaluatorUtils::evaluateRandomValsAndSigmoids(mvec, msig, slots, logp);
	EvaluatorUtils::leftShift(msig, logp);

	Cipher cipher = scheme.encryptFull(mvec);

	timeutils.start(SIGMOID);
	Cipher csig = algo.functionLazy(cipher, SIGMOID, degree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dsig = scheme.decryptFull(csig);

	StringUtils::showcompare(msig, dsig, SIGMOID);

	cout << "!!! END TEST SIGMOID LAZY !!!" << endl;
}

void TestScheme::testSigmoidExtended(long logN, long logl, long logp, long L, long degree) {
	cout << "!!! START TEST SIGMOID EXTENDED !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	double mr = (double)arc4random() / RAND_MAX;
	double mi = (double)arc4random() / RAND_MAX;

	CZZ mval = EvaluatorUtils::evaluateVal(mr, mi, logp);
	CZZ msig = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);

	Cipher c = scheme.encryptFull(mval);

	vector<Cipher> csig;

	timeutils.start(SIGMOID);
	algo.functionExtended(csig, c, SIGMOID, degree);
	timeutils.stop(SIGMOID);

	vector<CZZ> dsig = scheme.decryptFullSingleVec(csig);

	StringUtils::showcompare(msig, dsig, SIGMOID);

	cout << "!!! END TEST SIGMOID EXTENDED !!!" << endl;
}

//-----------------------------------------

void TestScheme::testFFT(long logN, long logl, long logp, long L, long logfftdim) {
	cout << "!!! START TEST FFT !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------
	long fftdim = 1 << logfftdim;

	vector<CZZ> mvec1, mvec2;
	EvaluatorUtils::evaluateRandomVals(mvec1, fftdim, logp);
	EvaluatorUtils::evaluateRandomVals(mvec2, fftdim, logp);

	vector<CZZ> mvecp = NumUtils::fftFull(mvec1, mvec2, params.ksiPows);

	vector<Cipher> cvec1 = scheme.encryptFullSingleVec(mvec1);
	vector<Cipher> cvec2 = scheme.encryptFullSingleVec(mvec2);

	timeutils.start("cfft 1");
	vector<Cipher> cfft1 = algo.fft(cvec1);
	timeutils.stop("cfft 1");

	timeutils.start("cfft 2");
	vector<Cipher> cfft2 = algo.fft(cvec2);
	timeutils.stop("cfft 2");

	timeutils.start("cfft mult");
	for (long i = 0; i < fftdim; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("cfft mult");

	timeutils.start("cfft inv");
	vector<Cipher> cvecp = algo.fftInv(cfft1);
	timeutils.stop("cfft inv");

	vector<CZZ> dvecp = scheme.decryptFullSingleVec(cvecp);

	StringUtils::showcompare(mvecp, dvecp, "fft");

	cout << "!!! END TEST FFT !!!" << endl;
}

void TestScheme::testFFTBatch(long logN, long logl, long logp, long L, long logfftdim, long logSlots) {
	cout << "!!! START TEST FFT BATCH !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;

	vector<vector<CZZ>> mvec1, mvec2, mvecp, dvecp;
	for (long i = 0; i < slots; ++i) {
		vector<CZZ> tmp1, tmp2;
		EvaluatorUtils::evaluateRandomVals(tmp1, fftdim, logp);
		EvaluatorUtils::evaluateRandomVals(tmp2, fftdim, logp);
		mvec1.push_back(tmp1);
		mvec2.push_back(tmp2);
	}

	for (long i = 0; i < slots; ++i) {
		vector<CZZ> tmp = NumUtils::fftFull(mvec1[i], mvec2[i], params.ksiPows);
		mvecp.push_back(tmp);
	}

	vector<Cipher> cvec1, cvec2;
	for (long j = 0; j < fftdim; ++j) {
		vector<CZZ> mvals1, mvals2;
		for (long i = 0; i < slots; ++i) {
			mvals1.push_back(mvec1[i][j]);
			mvals2.push_back(mvec2[i][j]);
		}
		Cipher cipher1 = scheme.encryptFull(mvals1);
		Cipher cipher2 = scheme.encryptFull(mvals2);

		cvec1.push_back(cipher1);
		cvec2.push_back(cipher2);
	}

	timeutils.start("cfft 1");
	vector<Cipher> cfft1 = algo.fft(cvec1);
	timeutils.stop("cfft 1");

	timeutils.start("cfft 2");
	vector<Cipher> cfft2 = algo.fft(cvec2);
	timeutils.stop("cfft 2");

	timeutils.start("cfft mult");
	for (long i = 0; i < fftdim; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("cfft mult");

	timeutils.start("cfft inv");
	vector<Cipher> cvecp = algo.fftInv(cfft1);
	timeutils.stop("cfft inv");

	for (long j = 0; j < fftdim; ++j) {
		vector<CZZ> tmp = scheme.decryptFull(cvecp[j]);
		dvecp.push_back(tmp);
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvecp[i][j], dvecp[j][i], "fft");
		}
	}

	cout << "!!! END TEST FFT BATCH !!!" << endl;
}

void TestScheme::testLazyFFT(long logN, long logl, long logp, long L, long logfftdim) {
	cout << "!!! START TEST FFT LAZY !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------
	long fftdim = 1 << logfftdim;

	vector<CZZ> mvec1, mvec2;
	EvaluatorUtils::evaluateRandomVals(mvec1, fftdim, logp);
	EvaluatorUtils::evaluateRandomVals(mvec2, fftdim, logp);

	vector<CZZ> mvecp = NumUtils::fftFullLazy(mvec1, mvec2, params.ksiPows);

	vector<Cipher> cvec1 = scheme.encryptFullSingleVec(mvec1);
	vector<Cipher> cvec2 = scheme.encryptFullSingleVec(mvec2);

	timeutils.start("cfft 1");
	vector<Cipher> cfft1 = algo.fft(cvec1);
	timeutils.stop("cfft 1");

	timeutils.start("cfft 2");
	vector<Cipher> cfft2 = algo.fft(cvec2);
	timeutils.stop("cfft 2");

	timeutils.start("cfft mult");
	for (long i = 0; i < fftdim; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("cfft mult");

	timeutils.start("cfft inv");
	vector<Cipher> cvecp = algo.fftInvLazy(cfft1);
	timeutils.stop("cfft inv");

	vector<CZZ> dvecp = scheme.decryptFullSingleVec(cvecp);

	StringUtils::showcompare(mvecp, dvecp, "fft");

	cout << "!!! END TEST FFT LAZY !!!" << endl;
}
