#include "TestScheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "Cipher.h"
#include "CZZ.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Params.h"
#include "PubKey.h"
#include "Scheme.h"
#include "SchemeAlgo.h"
#include "SchemeAux.h"
#include "SecKey.h"
#include "StringUtils.h"
#include "TimeUtils.h"

using namespace NTL;

//-----------------------------------------

void TestScheme::testEncodeBatch(long logN, long logq, long precisionBits, long logSlots) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, precisionBits);
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

void TestScheme::testConjugateBatch(long logN, long logq, long precisionBits, long logSlots) {
	cout << "!!! START TEST CONJUGATE BATCH !!!" << endl;
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, precisionBits);
	CZZ* mvecconj = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		mvecconj[i] = mvec[i].conjugate();
	}

	Cipher cipher = scheme.encrypt(mvec, slots);
	timeutils.start("Conjugate batch");
	Cipher cconj = scheme.conjugate(cipher);
	timeutils.stop("Conjugate batch");

	CZZ* dvecconj = scheme.decrypt(secretKey, cconj);

	StringUtils::showcompare(mvecconj, dvecconj, slots, "conj");

	cout << "!!! END TEST CONJUGATE BATCH !!!" << endl;
}

void TestScheme::testimultBatch(long logN, long logq, long precisionBits, long logSlots) {
	cout << "!!! START TEST i MULTIPLICATION BATCH !!!" << endl;
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, precisionBits);
	CZZ* imvec = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		imvec[i].r = -mvec[i].i;
		imvec[i].i = mvec[i].r;
	}

	Cipher cipher = scheme.encrypt(mvec, slots);
	timeutils.start("Multiplication by i batch");
	Cipher icipher = scheme.imult(cipher, precisionBits);
	timeutils.stop("Multiplication by i batch");

	CZZ* idvec = scheme.decrypt(secretKey, icipher);

	StringUtils::showcompare(imvec, idvec, slots, "imult");

	cout << "!!! END TEST i MULTIPLICATION BATCH !!!" << endl;
}

void TestScheme::testRotateByPo2Batch(long logN, long logq, long precisionBits, long rotlogSlots, long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BY POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	long rotSlots = (1 << rotlogSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, precisionBits);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	if(isLeft) {
		timeutils.start("Left Rotate by power of 2 batch");
		scheme.leftRotateByPo2AndEqual(cipher, rotlogSlots);
		timeutils.stop("Left Rotate by power of 2 batch");
	} else {
		timeutils.start("Right Rotate by power of 2 batch");
		scheme.rightRotateByPo2AndEqual(cipher, rotlogSlots);
		timeutils.stop("Right Rotate by power of 2 batch");
	}
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cipher);
	if(isLeft) {
		EvaluatorUtils::leftRotateAndEqual(mvec, slots, rotSlots);
	} else {
		EvaluatorUtils::rightRotateAndEqual(mvec, slots, rotSlots);
	}
	StringUtils::showcompare(mvec, dvec, slots, "val");
	//-----------------------------------------
	cout << "!!! END TEST ROTATE BY POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testRotateBatch(long logN, long logq, long precisionBits, long rotSlots, long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, precisionBits);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	if(isLeft) {
		timeutils.start("Left rotate batch");
		scheme.leftRotateAndEqual(cipher, rotSlots);
		timeutils.stop("Left rotate batch");
	} else {
		timeutils.start("Right rotate batch");
		scheme.rightRotateAndEqual(cipher, rotSlots);
		timeutils.stop("Right rotate batch");
	}
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cipher);
	if(isLeft) {
		EvaluatorUtils::leftRotateAndEqual(mvec, slots, rotSlots);
	} else {
		EvaluatorUtils::rightRotateAndEqual(mvec, slots, rotSlots);
	}
	StringUtils::showcompare(mvec, dvec, slots, "val");
	//-----------------------------------------
	cout << "!!! END TEST ROTATE BATCH !!!" << endl;
}

void TestScheme::testSlotsSum(long logN, long logq, long precisionBits, long logSlots) {
	cout << "!!! START TEST SLOTS SUM !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evaluateRandomVals(slots, precisionBits);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("slots sum");
	algo.partialSlotsSumAndEqual(cipher, slots);
	timeutils.stop("slots sum");
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

void TestScheme::testPowerOf2Batch(long logN, long logq, long precisionBits, long logDegree, long logSlots) {
	cout << "!!! START TEST POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mpow = new CZZ[slots];

	CZZ** mpows = new CZZ*[logDegree + 1];
	for (long i = 0; i < logDegree + 1; ++i) {
		mpows[i] = new CZZ[slots];
	}

	for (long i = 0; i < slots; ++i) {
		RR angle = random_RR();
		RR mr = cos(angle * 2 * Pi);
		RR mi = sin(angle * 2 * Pi);
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		mpow[i] = EvaluatorUtils::evaluatePow2(mr, mi, logDegree, precisionBits);
		for (int j = 0; j < logDegree + 1; ++j) {
			mpows[j][i] = EvaluatorUtils::evaluatePow2(mr, mi, j, precisionBits);
		}
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Power of 2 batch");
	Cipher cpow = algo.powerOf2(cipher, precisionBits, logDegree);
	timeutils.stop("Power of 2 batch");

	//-----------------------------------------
	CZZ* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::showcompare(mpow, dpow, slots, "pow");
	//-----------------------------------------
	cout << "!!! END TEST POWER OF 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerBatch(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST POWER BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
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
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		mpow[i] = EvaluatorUtils::evaluatePow(mr, mi, degree, precisionBits);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Power batch");
	Cipher cpow = algo.power(cipher, precisionBits, degree);
	timeutils.stop("Power batch");
	//-----------------------------------------
	CZZ* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::showcompare(mpow, dpow, slots, "pow");
	//-----------------------------------------
	cout << "!!! END TEST POWER BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testProdOfPo2Batch(long logN, long logq, long precisionBits, long logDegree, long logSlots) {
	cout << "!!! START TEST PROD OF POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(4);
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
			mvec[i][j] = EvaluatorUtils::evaluateRandomCircleVal(precisionBits);
		}
	}
	for (long j = 0; j < slots; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
			pvec[j] >>= precisionBits;
		}
	}
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], slots);
	}
	//-----------------------------------------
	timeutils.start("Product of power of 2 batch");
	Cipher cprod = algo.prodOfPo2(cvec, precisionBits, logDegree);
	timeutils.stop("Product of power of 2 batch");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cprod);
	StringUtils::showcompare(pvec, dvec, slots, "prod");
	//-----------------------------------------
	cout << "!!! END TEST PROD OF POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testProdBatch(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST PROD BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(4);
	long slots = 1 << logSlots;
	Cipher* cvec = new Cipher[degree];
	CZZ** mvec = new CZZ*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = new CZZ[slots];
	}
	CZZ* pvec = new CZZ[slots];
	for (long i = 0; i < degree; ++i) {
		for (long j = 0; j < slots; ++j) {
			mvec[i][j] = EvaluatorUtils::evaluateRandomCircleVal(precisionBits);
		}
	}
	for (long j = 0; j < slots; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
			pvec[j] >>= precisionBits;
		}
	}
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], slots);
	}
	//-----------------------------------------
	timeutils.start("Product batch");
	Cipher cprod = algo.prod(cvec, precisionBits, degree);
	timeutils.stop("Product batch");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cprod);
	StringUtils::showcompare(pvec, dvec, slots, "prod");
	//-----------------------------------------
	cout << "!!! END TEST PROD BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverseBatch(long logN, long logq, long precisionBits, long invSteps, long logSlots) {
	cout << "!!! START TEST INVERSE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
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
		mvec[i] = EvaluatorUtils::evaluateVal(1 - mr, -mi, precisionBits);
		minv[i] = EvaluatorUtils::evaluateInverse(mr, mi, precisionBits);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start("Inverse batch");
	Cipher cinv = algo.inverse(cipher, precisionBits, invSteps);
	timeutils.stop("Inverse batch");
	//-----------------------------------------
	CZZ* dinv = scheme.decrypt(secretKey, cinv);
	StringUtils::showcompare(minv, dinv, slots, "inv");
	//-----------------------------------------
	cout << "!!! END TEST INVERSE BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testLogarithmBatch(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST LOGARITHM BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mlog = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		double mr = (double)rand() / RAND_MAX / 20;
		double mi = (double)rand() / RAND_MAX / 20;
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		mlog[i] = EvaluatorUtils::evaluateLogarithm(1 + mr, mi, precisionBits);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(LOGARITHM + " batch");
	Cipher clog = algo.function(cipher, LOGARITHM, precisionBits, degree);
	timeutils.stop(LOGARITHM + " batch");
	//-----------------------------------------
	CZZ* dlog = scheme.decrypt(secretKey, clog);
	StringUtils::showcompare(mlog, dlog, slots, LOGARITHM);
	//-----------------------------------------
	cout << "!!! END TEST LOGARITHM BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testExponentBatch(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mexp = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		mexp[i] = EvaluatorUtils::evaluateExponent(mr, mi, precisionBits);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(EXPONENT + " batch");
	Cipher cexp = algo.function(cipher, EXPONENT, precisionBits, degree);
	timeutils.stop(EXPONENT + " batch");
	//-----------------------------------------
	CZZ* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);
	//-----------------------------------------
	cout << "!!! END TEST EXPONENT BATCH !!!" << endl;
}

void TestScheme::testExponentBatchLazy(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mexp = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		mexp[i] = EvaluatorUtils::evaluateExponent(mr, mi, precisionBits);
	}
	EvaluatorUtils::leftShiftAndEqual(mexp, slots, precisionBits);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(EXPONENT + " lazy");
	Cipher cexp = algo.functionLazy(cipher, EXPONENT, precisionBits, degree);
	timeutils.stop(EXPONENT + " lazy");
	//-----------------------------------------
	CZZ* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);
	//-----------------------------------------
	cout << "!!! END TEST EXPONENT LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoidBatch(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* msig = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		msig[i] = EvaluatorUtils::evaluateSigmoid(mr, mi, precisionBits);
	}
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(SIGMOID + " batch");
	Cipher csig = algo.function(cipher, SIGMOID, precisionBits, degree);
	timeutils.stop(SIGMOID + " batch");
	//-----------------------------------------
	CZZ* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::showcompare(msig, dsig, slots, SIGMOID);
	//-----------------------------------------
	cout << "!!! END TEST SIGMOID BATCH !!!" << endl;
}

void TestScheme::testSigmoidBatchLazy(long logN, long logq, long precisionBits, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* msig = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evaluateVal(mr, mi, precisionBits);
		msig[i] = EvaluatorUtils::evaluateSigmoid(mr, mi, precisionBits);
	}
	EvaluatorUtils::leftShiftAndEqual(msig, slots, precisionBits);
	Cipher cipher = scheme.encrypt(mvec, slots);
	//-----------------------------------------
	timeutils.start(SIGMOID + " lazy");
	Cipher csig = algo.functionLazy(cipher, SIGMOID, precisionBits, degree);
	timeutils.stop(SIGMOID + " lazy");
	//-----------------------------------------
	CZZ* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::showcompare(msig, dsig, slots, SIGMOID);
	//-----------------------------------------
	cout << "!!! END TEST SIGMOID LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testFFTBatch(long logN, long logq, long precisionBits, long logSlots, long logfftdim) {
	cout << "!!! START TEST FFT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	CZZ** mvec1 = new CZZ*[slots];
	CZZ** mvec2 = new CZZ*[slots];

	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::evaluateRandomVals(fftdim, precisionBits);
		mvec2[i] = EvaluatorUtils::evaluateRandomVals(fftdim, precisionBits);
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
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < slots; ++i) {
		NumUtils::fft(mvec1[i], fftdim, schemeaux);
		NumUtils::fft(mvec2[i], fftdim, schemeaux);
		for (long j = 0; j < fftdim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
			mvec1[i][j] >>= precisionBits;
		}
		NumUtils::fftInv(mvec1[i], fftdim, schemeaux);
	}
	//-----------------------------------------
	timeutils.start("ciphers fft 1 batch");
	algo.fft(cvec1, fftdim);
	timeutils.stop("ciphers fft 1 batch");
	//-----------------------------------------
	timeutils.start("ciphers fft 2 batch");
	algo.fft(cvec2, fftdim);
	timeutils.stop("ciphers fft 2 batch");
	//-----------------------------------------
	timeutils.start("ciphers hadamard mult batch");
	algo.multModSwitchAndEqualVec(cvec1, cvec2, precisionBits, fftdim);
	timeutils.stop("ciphers hadamard mult batch");
	//-----------------------------------------
	delete[] cvec2;
	//-----------------------------------------
	timeutils.start("ciphers fft inverse batch");
	algo.fftInv(cvec1, fftdim);
	timeutils.stop("ciphers fft inverse batch");
	//-----------------------------------------
	CZZ** dvec1 = new CZZ*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvec1[j] = scheme.decrypt(secretKey, cvec1[j]);
	}
	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvec1[i][j], dvec1[j][i], "fft");
		}
	}
	//-----------------------------------------
	cout << "!!! END TEST FFT BATCH !!!" << endl;
}

void TestScheme::testFFTBatchLazy(long logN, long logq, long precisionBits, long logSlots, long logfftdim) {
	cout << "!!! START TEST FFT BATCH LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	CZZ** mvec1 = new CZZ*[slots];
	CZZ** mvec2 = new CZZ*[slots];

	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::evaluateRandomVals(fftdim, precisionBits);
		mvec2[i] = EvaluatorUtils::evaluateRandomVals(fftdim, precisionBits);
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
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < slots; ++i) {
		NumUtils::fft(mvec1[i], fftdim, schemeaux);
		NumUtils::fft(mvec2[i], fftdim, schemeaux);
		for (long j = 0; j < fftdim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
			mvec1[i][j] >>= precisionBits;
		}
		NumUtils::fftInvLazy(mvec1[i], fftdim, schemeaux);
	}
	//-----------------------------------------
	timeutils.start("ciphers fft 1");
	algo.fft(cvec1, fftdim);
	timeutils.stop("ciphers fft 1");
	//-----------------------------------------
	timeutils.start("ciphers fft 2");
	algo.fft(cvec2, fftdim);
	timeutils.stop("ciphers fft 2");
	//-----------------------------------------
	timeutils.start("ciphers hadamard mult");
	algo.multModSwitchAndEqualVec(cvec1, cvec2, precisionBits, fftdim);
	timeutils.stop("ciphers hadamard mult");
	//-----------------------------------------
	delete[] cvec2;
	//-----------------------------------------
	timeutils.start("ciphers fft inverse lazy");
	algo.fftInvLazy(cvec1, fftdim);
	timeutils.stop("ciphers fft inverse lazy");
	//-----------------------------------------
	CZZ** dvec1 = new CZZ*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvec1[j] = scheme.decrypt(secretKey, cvec1[j]);
	}
	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvec1[i][j], dvec1[j][i], "fft");
		}
	}
	//-----------------------------------------
	cout << "!!! END TEST FFT BATCH LAZY !!!" << endl;
}

void TestScheme::testFFTBatchLazyMultipleHadamard(long logN, long logq, long precisionBits, long logSlots, long logfftdim, long logHdim) {
	cout << "!!! START TEST FFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logN);
	Scheme scheme(params, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long hdim = 1 << logHdim;
	long slots = 1 << logSlots;
	CZZ*** mvecs = new CZZ**[hdim];
	Cipher** cvecs = new Cipher*[hdim];
	for (long h = 0; h < hdim; ++h) {
		mvecs[h] = new CZZ*[slots];

		for (long i = 0; i < slots; ++i) {
			mvecs[h][i] = EvaluatorUtils::evaluateRandomVals(fftdim, precisionBits);
		}

		cvecs[h] = new Cipher[fftdim];

		for (long j = 0; j < fftdim; ++j) {
			CZZ* mvals = new CZZ[slots];
			for (long i = 0; i < slots; ++i) {
				mvals[i] = mvecs[h][i][j];
			}
			cvecs[h][j] = scheme.encrypt(mvals, slots);
			delete[] mvals;
		}
		for (long i = 0; i < slots; ++i) {
			NumUtils::fft(mvecs[h][i], fftdim, schemeaux);
		}
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			for (long s = logHdim - 1; s >= 0; --s) {
				long spow = 1 << s;
				for (long h = 0; h < spow; ++h) {
					mvecs[h][i][j] *= mvecs[h+spow][i][j];
					mvecs[h][i][j] >>= precisionBits;
				}
			}
		}
	}

	for (long i = 0; i < slots; ++i) {
		NumUtils::fftInvLazy(mvecs[0][i], fftdim, schemeaux);
	}

	for (long h = 1; h < hdim; ++h) {
		for (long i = 0; i < slots; ++i) {
			delete[] mvecs[h][i];
		}
		delete[] mvecs[h];
	}

	for (long h = 0; h < hdim; ++h) {
		timeutils.start("ciphers fft");
		algo.fft(cvecs[h], fftdim);
		timeutils.stop("ciphers fft");
	}
	for (long s = logHdim - 1; s >= 0; --s) {
		long spow = 1 << s;
		for (long h = 0; h < spow; ++h) {
			timeutils.start("ciphers hadamard mult");
			algo.multModSwitchAndEqualVec(cvecs[h], cvecs[h+spow], precisionBits, fftdim);
			timeutils.stop("ciphers hadamard mult");
			delete[] cvecs[h+spow];
		}
	}

	timeutils.start("ciphers fft inverse lazy");
	algo.fftInvLazy(cvecs[0], fftdim);
	timeutils.stop("ciphers fft inverse lazy");

	//-----------------------------------------
	CZZ** dvec = new CZZ*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvec[j] = scheme.decrypt(secretKey, cvecs[0][j]);
	}
	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvecs[0][i][j], dvec[j][i], "fft");
		}
	}
	//-----------------------------------------
	cout << "!!! END TEST FFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;
}
