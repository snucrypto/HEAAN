#include "TestScheme.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>

#include <NTL/BasicThreadPool.h>

#include "CZZ.h"
#include "Cipher.h"
#include "Params.h"
#include "PubKey.h"
#include "Scheme.h"
#include "SchemeAlgo.h"
#include "SchemeAux.h"
#include "SecKey.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "StringUtils.h"
#include "TimeUtils.h"

using namespace NTL;

//-----------------------------------------

void TestScheme::testEncodeBatch(long logN, long logl, long logp, long L, long logSlots) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	//-----------------------------------------
	timeutils.start("Encrypt batch");
	Cipher cipher = scheme.encrypt(mvec, slots);
	timeutils.stop("Encrypt batch");
	//-----------------------------------------
	timeutils.start("Decrypt batch");
	CZZ* dvec = scheme.decrypt(secretKey, cipher);
	timeutils.stop("Decrypt batch");
	//-----------------------------------------
	StringUtils::showcompare(mvec, dvec, slots, "val");
	//-----------------------------------------
	cout << "!!! END TEST ENCODE BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testConjugateBatch(long logN, long logl, long logp, long L, long logSlots) {
	cout << "!!! START TEST CONJUGATE !!!" << endl;
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	CZZ* mvecconj = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		mvecconj[i] = mvec[i].conjugate();
	}

	Cipher cipher = scheme.encrypt(mvec, slots);
	timeutils.start("Conjugate");
	Cipher cconj = scheme.conjugate(cipher);
	timeutils.stop("Conjugate");

	CZZ* dvecconj = scheme.decrypt(secretKey, cconj);

	StringUtils::showcompare(mvecconj, dvecconj, slots, "conj");

	cout << "!!! END TEST CONJUGATE !!!" << endl;
}

void TestScheme::testLeftRotate2Batch(long logN, long logl, long logp, long L, long rotlogSlots, long logSlots) {
	cout << "!!! START TEST LEFT ROTATE 2 !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	long rotSlots = (1 << rotlogSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Left Rotate 2");
	scheme.leftRotate2AndEqual(cipher, rotlogSlots);
	timeutils.stop("Left Rotate 2");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cipher);
	EvaluatorUtils::leftRotate(mvec, slots, rotSlots);
	StringUtils::showcompare(mvec, dvec, slots, "val");
	//-----------------------------------------
	cout << "!!! END TEST LEFT ROTATE 2 !!!" << endl;
}

void TestScheme::testLeftRotateBatch(long logN, long logl, long logp, long L, long rotSlots, long logSlots) {
	cout << "!!! START TEST LEFT ROTATE !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Rotate");
	scheme.rotateAndEqual(cipher, rotSlots);
	timeutils.stop("Rotate");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cipher);
	EvaluatorUtils::leftRotate(mvec, slots, rotSlots);
	StringUtils::showcompare(mvec, dvec, slots, "val");
	//-----------------------------------------
	cout << "!!! END TEST LEFT ROTATE !!!" << endl;
}

void TestScheme::testSlotsSum(long logN, long logl, long logp, long L, long logSlots) {
	cout << "!!! START TEST SLOTS SUM !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, logp);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("slotsum");
	algo.partialSlotsSumAndEqual(cipher, slots);
	timeutils.stop("slotsum");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cipher);
	CZZ msum = CZZ();
	for (long i = 0; i < slots; ++i) {
		msum += mvec[i];
	}
	StringUtils::showcompare(msum, dvec, slots, "slotsum");
	//-----------------------------------------
	cout << "!!! END TEST SLOTS SUM !!!" << endl;
}


//-----------------------------------------

void TestScheme::testPowerOf2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots) {
	cout << "!!! START TEST POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mpow = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR angle = random_RR();
		RR mr = cos(angle * 2 * Pi);
		RR mi = sin(angle * 2 * Pi);
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		mpow[i] = EvaluatorUtils::evaluatePow2(mr, mi, logDegree, logp);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Power of 2 batch");
	Cipher cpow = algo.powerOf2(cipher, logDegree);
	timeutils.stop("Power of 2 batch");
	//-----------------------------------------
	CZZ* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::showcompare(mpow, dpow, slots, "pow");
	//-----------------------------------------
	cout << "!!! END TEST POWER OF 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST POWER BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mpow = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR angle = random_RR();
		RR mr = cos(angle * 2 * Pi);
		RR mi = sin(angle * 2 * Pi);
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		mpow[i] = EvaluatorUtils::evaluatePow(mr, mi, degree, logp);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Power batch");
	Cipher cpow = algo.power(cipher, degree);
	timeutils.stop("Power batch");
	//-----------------------------------------
	CZZ* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::showcompare(mpow, dpow, slots, "pow");
	//-----------------------------------------
	cout << "!!! END TEST POWER BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testProd2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots) {
	cout << "!!! START TEST PROD 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(2);
	long slots = 1 << logSlots;
	long degree = 1 << logDegree;
	Cipher* cvec = new Cipher[degree];
	CZZ** mvec = new CZZ*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = new CZZ[slots];
	}
	CZZ* pvec = new CZZ[slots];
	for (long i = 0; i < degree; ++i) {
		for (long j = 0; j < slots; ++j) {
			mvec[i][j] = EvaluatorUtils::evaluateRandomCircleVal(logp);
		}
	}
	for (long j = 0; j < slots; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
			pvec[j] >>= logp;
		}
	}
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], slots);
	}
	//-----------------------------------------
	timeutils.start("Prod 2 batch");
	Cipher cprod = algo.prod2(cvec, logDegree);
	timeutils.stop("Prod 2 batch");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cprod);
	StringUtils::showcompare(pvec, dvec, slots, "prod");
	//-----------------------------------------
	cout << "!!! END TEST PROD 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots) {
	cout << "!!! START TEST INVERSE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* minv = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR angle = random_RR() / 20;
		RR mr = cos(angle * 2 * Pi);
		RR mi = sin(angle * 2 * Pi);
		mvec[i] = EvaluatorUtils::evaluateVal(1 - mr, -mi, logp);
		minv[i] = EvaluatorUtils::evaluateInverse(mr, mi, logp);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Inverse batch");
	Cipher cinv = algo.inverse(cipher, invSteps);
	timeutils.stop("Inverse batch");
	//-----------------------------------------
	CZZ* dinv = scheme.decrypt(secretKey, cinv);
	StringUtils::showcompare(minv, dinv, slots, "inv");
	//-----------------------------------------
	cout << "!!! END TEST INVERSE BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testLogarithmBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST LOGARITHM BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mlog = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		double mr = (double)arc4random() / RAND_MAX / 20;
		double mi = (double)arc4random() / RAND_MAX / 20;
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		mlog[i] = EvaluatorUtils::evaluateLogarithm(1 + mr, mi, logp);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(LOGARITHM + " batch");
	Cipher clog = algo.function(cipher, LOGARITHM, degree);
	timeutils.stop(LOGARITHM + " batch");
	//-----------------------------------------
	CZZ* dlog = scheme.decrypt(secretKey, clog);
	StringUtils::showcompare(mlog, dlog, slots, LOGARITHM);
	//-----------------------------------------
	cout << "!!! END TEST LOGARITHM BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testExponentBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mexp = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		mexp[i] = EvaluatorUtils::evaluateExponent(mr, mi, logp);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(EXPONENT + " batch");
	Cipher cexp = algo.function(cipher, EXPONENT, degree);
	timeutils.stop(EXPONENT + " batch");
	//-----------------------------------------
	CZZ* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);
	//-----------------------------------------
	cout << "!!! END TEST EXPONENT BATCH !!!" << endl;
}

void TestScheme::testExponentBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mexp = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		mexp[i] = EvaluatorUtils::evaluateExponent(mr, mi, logp);
	}
	EvaluatorUtils::leftShift(mexp, slots, logp);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(EXPONENT + " lazy");
	Cipher cexp = algo.functionLazy(cipher, EXPONENT, degree);
	timeutils.stop(EXPONENT + " lazy");
	//-----------------------------------------
	CZZ* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);
	//-----------------------------------------
	cout << "!!! END TEST EXPONENT LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoidBatch(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* msig = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		msig[i] = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(SIGMOID + " batch");
	Cipher csig = algo.function(cipher, SIGMOID, degree);
	timeutils.stop(SIGMOID + " batch");
	//-----------------------------------------
	CZZ* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::showcompare(msig, dsig, slots, SIGMOID);
	//-----------------------------------------
	cout << "!!! END TEST SIGMOID BATCH !!!" << endl;
}

void TestScheme::testSigmoidBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* msig = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, logp);
		msig[i] = EvaluatorUtils::evaluateSigmoid(mr, mi, logp);
	}
	EvaluatorUtils::leftShift(msig, slots, logp);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(SIGMOID + " lazy");
	Cipher csig = algo.functionLazy(cipher, SIGMOID, degree);
	timeutils.stop(SIGMOID + " lazy");
	//-----------------------------------------
	CZZ* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::showcompare(msig, dsig, slots, SIGMOID);
	//-----------------------------------------
	cout << "!!! END TEST SIGMOID LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testFFTBatch(long logN, long logl, long logp, long L, long logfftdim, long logSlots) {
	cout << "!!! START TEST FFT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	CZZ** mvec1 = new CZZ*[slots];
	CZZ** mvec2 = new CZZ*[slots];
	CZZ** mvecp = new CZZ*[slots];
	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::evaluateRandomVals(fftdim, logp);
		mvec2[i] = EvaluatorUtils::evaluateRandomVals(fftdim, logp);
		mvecp[i] = NumUtils::fftFull(mvec1[i], mvec2[i], fftdim, scheme.aux.ksiPows, logp);
	}
	Cipher* cvec1 = new Cipher[fftdim];
	Cipher* cvec2 = new Cipher[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		CZZ* mvals1 = new CZZ[slots];
		CZZ* mvals2	= new CZZ[slots];
		for (long i = 0; i < slots; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, slots);
		cvec2[j] = scheme.encrypt(mvals2, slots);
	}
	//-----------------------------------------
	timeutils.start("cfft 1 batch");
	Cipher* cfft1 = algo.fft(cvec1, fftdim);
	timeutils.stop("cfft 1 batch");
	//-----------------------------------------
	timeutils.start("cfft 2 batch");
	Cipher* cfft2 = algo.fft(cvec2, fftdim);
	timeutils.stop("cfft 2 batch");
	//-----------------------------------------
	timeutils.start("cfft mult batch");
	algo.multModSwitchAndEqualVec(cfft1, cfft2, fftdim);
	timeutils.stop("cfft mult batch");
	//-----------------------------------------
	timeutils.start("cfft inv batch");
	Cipher* cvecp = algo.fftInv(cfft1, fftdim);
	timeutils.stop("cfft inv batch");
	//-----------------------------------------
	CZZ** dvecp = new CZZ*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvecp[j] = scheme.decrypt(secretKey, cvecp[j]);
	}
	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvecp[i][j], dvecp[j][i], "fft");
		}
	}
	//-----------------------------------------
	cout << "!!! END TEST FFT BATCH !!!" << endl;
}

void TestScheme::testFFTLazy(long logN, long logl, long logp, long L, long logfftdim) {
	cout << "!!! START TEST FFT LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	CZZ* mvec1 = EvaluatorUtils::evaluateRandomVals(fftdim, logp);
	CZZ* mvec2 = EvaluatorUtils::evaluateRandomVals(fftdim, logp);
	CZZ* mvecp = NumUtils::fftFullLazy(mvec1, mvec2, fftdim, scheme.aux.ksiPows, logp);
	Cipher* cvec1 = algo.encryptSingleArray(mvec1, fftdim);
	Cipher* cvec2 = algo.encryptSingleArray(mvec2, fftdim);
	//-----------------------------------------
	timeutils.start("cfft 1 lazy");
	Cipher* cfft1 = algo.fft(cvec1, fftdim);
	timeutils.stop("cfft 1 lazy");
	//-----------------------------------------
	timeutils.start("cfft 2 lazy");
	Cipher* cfft2 = algo.fft(cvec2, fftdim);
	timeutils.stop("cfft 2 lazy");
	//-----------------------------------------
	timeutils.start("cfft mult lazy");
	algo.multModSwitchAndEqualVec(cfft1, cfft2, fftdim);
	timeutils.stop("cfft mult lazy");
	//-----------------------------------------
	timeutils.start("cfft inv lazy");
	Cipher* cvecp = algo.fftInvLazy(cfft1, fftdim);
	timeutils.stop("cfft inv lazy");
	//-----------------------------------------
	CZZ* dvecp = algo.decryptSingleArray(secretKey, cvecp, fftdim);
	StringUtils::showcompare(mvecp, dvecp, fftdim, "fft");
	//-----------------------------------------
	cout << "!!! END TEST FFT LAZY !!!" << endl;
}
