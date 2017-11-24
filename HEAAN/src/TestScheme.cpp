#include "TestScheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>

#include "Common.h"
#include "Ciphertext.h"
#include "CZZ.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Params.h"
#include "Scheme.h"
#include "SchemeAlgo.h"
#include "SecretKey.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include "Context.h"

using namespace std;
using namespace NTL;

//-----------------------------------------

void TestScheme::testEncodeBatch(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evalRandCZZArray(slots, logp);
	//-----------------------------------------
	timeutils.start("Encrypt batch");
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
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

void TestScheme::testConjugateBatch(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST CONJUGATE BATCH !!!" << endl;
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	scheme.addConjKey(secretKey);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evalRandCZZArray(slots, logp);
	CZZ* mvecconj = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		mvecconj[i] = mvec[i].conjugate();
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	timeutils.start("Conjugate batch");
	Ciphertext cconj = scheme.conjugate(cipher);
	timeutils.stop("Conjugate batch");

	CZZ* dvecconj = scheme.decrypt(secretKey, cconj);

	StringUtils::showcompare(mvecconj, dvecconj, slots, "conj");

	cout << "!!! END TEST CONJUGATE BATCH !!!" << endl;
}

void TestScheme::testimultBatch(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST i MULTIPLICATION BATCH !!!" << endl;
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evalRandCZZArray(slots, logp);
	CZZ* imvec = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		imvec[i].r = -mvec[i].i;
		imvec[i].i = mvec[i].r;
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	timeutils.start("Multiplication by i batch");
	Ciphertext icipher = scheme.imult(cipher);
	timeutils.stop("Multiplication by i batch");

	CZZ* idvec = scheme.decrypt(secretKey, icipher);

	StringUtils::showcompare(imvec, idvec, slots, "imult");

	cout << "!!! END TEST i MULTIPLICATION BATCH !!!" << endl;
}

void TestScheme::testRotateByPo2Batch(long logN, long logQ, long logp, long logRotSlots, long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BY POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	scheme.addLeftRotKeys(secretKey);
	//-----------------------------------------
	long slots = (1 << logSlots);
	long rotSlots = (1 << logRotSlots);
	CZZ* mvec = EvaluatorUtils::evalRandCZZArray(slots, logp);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	if(isLeft) {
		timeutils.start("Left Rotate by power of 2 batch");
		scheme.leftRotateByPo2AndEqual(cipher, logRotSlots);
		timeutils.stop("Left Rotate by power of 2 batch");
	} else {
		timeutils.start("Right Rotate by power of 2 batch");
		scheme.rightRotateByPo2AndEqual(cipher, logRotSlots);
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

void TestScheme::testRotateBatch(long logN, long logQ, long logp, long rotSlots, long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	scheme.addLeftRotKeys(secretKey);
	scheme.addRightRotKeys(secretKey);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evalRandCZZArray(slots, logp);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
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

void TestScheme::testSlotsSum(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST SLOTS SUM !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	scheme.addLeftRotKeys(secretKey);
	//-----------------------------------------
	long slots = (1 << logSlots);
	CZZ* mvec = EvaluatorUtils::evalRandCZZArray(slots, logp);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
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

void TestScheme::testPowerOf2Batch(long logN, long logQ, long logp, long logDegree, long logSlots) {
	cout << "!!! START TEST POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
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
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		mpow[i] = EvaluatorUtils::evalCZZPow2(mr, mi, logDegree, logp);
		for (int j = 0; j < logDegree + 1; ++j) {
			mpows[j][i] = EvaluatorUtils::evalCZZPow2(mr, mi, j, logp);
		}
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start("Power of 2 batch");
	Ciphertext cpow = algo.powerOf2(cipher, logp, logDegree);
	timeutils.stop("Power of 2 batch");

	//-----------------------------------------
	CZZ* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::showcompare(mpow, dpow, slots, "pow");
	//-----------------------------------------
	cout << "!!! END TEST POWER OF 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST POWER BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mpow = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR angle = random_RR();
		RR mr = cos(angle * 2 * Pi);
		RR mi = sin(angle * 2 * Pi);
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		mpow[i] = EvaluatorUtils::evalCZZPow(mr, mi, degree, logp);
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start("Power batch");
	Ciphertext cpow = algo.power(cipher, logp, degree);
	timeutils.stop("Power batch");
	//-----------------------------------------
	CZZ* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::showcompare(mpow, dpow, slots, "pow");
	//-----------------------------------------
	cout << "!!! END TEST POWER BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testProdOfPo2Batch(long logN, long logQ, long logp, long logDegree, long logSlots) {
	cout << "!!! START TEST PROD OF POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(4);
	long slots = 1 << logSlots;
	long degree = 1 << logDegree;
	Ciphertext* cvec = new Ciphertext[degree];
	CZZ** mvec = new CZZ*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = new CZZ[slots];
	}
	CZZ* pvec = new CZZ[slots];
	for (long i = 0; i < degree; ++i) {
		for (long j = 0; j < slots; ++j) {
			mvec[i][j] = EvaluatorUtils::evalRandCZZCircle(logp);
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
		cvec[i] = scheme.encrypt(mvec[i], slots, logQ);
	}
	//-----------------------------------------
	timeutils.start("Product of power of 2 batch");
	Ciphertext cprod = algo.prodOfPo2(cvec, logp, logDegree);
	timeutils.stop("Product of power of 2 batch");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cprod);
	StringUtils::showcompare(pvec, dvec, slots, "prod");
	//-----------------------------------------
	cout << "!!! END TEST PROD OF POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testProdBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST PROD BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(4);
	long slots = 1 << logSlots;
	Ciphertext* cvec = new Ciphertext[degree];
	CZZ** mvec = new CZZ*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = new CZZ[slots];
	}
	CZZ* pvec = new CZZ[slots];
	for (long i = 0; i < degree; ++i) {
		for (long j = 0; j < slots; ++j) {
			mvec[i][j] = EvaluatorUtils::evalRandCZZCircle(logp);
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
		cvec[i] = scheme.encrypt(mvec[i], slots, logQ);
	}
	//-----------------------------------------
	timeutils.start("Product batch");
	Ciphertext cprod = algo.prod(cvec, logp, degree);
	timeutils.stop("Product batch");
	//-----------------------------------------
	CZZ* dvec = scheme.decrypt(secretKey, cprod);
	StringUtils::showcompare(pvec, dvec, slots, "prod");
	//-----------------------------------------
	cout << "!!! END TEST PROD BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testInverseBatch(long logN, long logQ, long logp, long invSteps, long logSlots) {
	cout << "!!! START TEST INVERSE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* minv = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR angle = random_RR() / 20;
		RR mr = cos(angle * 2 * Pi);
		RR mi = sin(angle * 2 * Pi);
		mvec[i] = EvaluatorUtils::evalCZZ(1 - mr, -mi, logp);
		minv[i] = EvaluatorUtils::evalCZZInv(mr, mi, logp);
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start("Inverse batch");
	Ciphertext cinv = algo.inverse(cipher, logp, invSteps);
	timeutils.stop("Inverse batch");
	//-----------------------------------------
	CZZ* dinv = scheme.decrypt(secretKey, cinv);
	StringUtils::showcompare(minv, dinv, slots, "inv");
	//-----------------------------------------
	cout << "!!! END TEST INVERSE BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testLogarithmBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST LOGARITHM BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mlog = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		double mr = (double)rand() / RAND_MAX / 20;
		double mi = (double)rand() / RAND_MAX / 20;
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		mlog[i] = EvaluatorUtils::evalCZZLog(1 + mr, mi, logp);
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start(LOGARITHM + " batch");
	Ciphertext clog = algo.function(cipher, LOGARITHM, logp, degree);
	timeutils.stop(LOGARITHM + " batch");
	//-----------------------------------------
	CZZ* dlog = scheme.decrypt(secretKey, clog);
	StringUtils::showcompare(mlog, dlog, slots, LOGARITHM);
	//-----------------------------------------
	cout << "!!! END TEST LOGARITHM BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testExponentBatch(long logN, long logq, long logp, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logq);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mexp = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		mexp[i] = EvaluatorUtils::evalCZZExp(mr, mi, logp);
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logq);
	//-----------------------------------------
	timeutils.start(EXPONENT + " batch");
	Ciphertext cexp = algo.function(cipher, EXPONENT, logp, degree);
	timeutils.stop(EXPONENT + " batch");
	//-----------------------------------------
	CZZ* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);
	//-----------------------------------------
	cout << "!!! END TEST EXPONENT BATCH !!!" << endl;
}

void TestScheme::testExponentBatchLazy(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* mexp = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		mexp[i] = EvaluatorUtils::evalCZZExp(mr, mi, logp);
	}
	EvaluatorUtils::leftShiftAndEqual(mexp, slots, logp);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start(EXPONENT + " lazy");
	Ciphertext cexp = algo.functionLazy(cipher, EXPONENT, logp, degree);
	timeutils.stop(EXPONENT + " lazy");
	//-----------------------------------------
	CZZ* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);
	//-----------------------------------------
	cout << "!!! END TEST EXPONENT LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoidBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* msig = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		msig[i] = EvaluatorUtils::evalCZZSigmoid(mr, mi, logp);
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start(SIGMOID + " batch");
	Ciphertext csig = algo.function(cipher, SIGMOID, logp, degree);
	timeutils.stop(SIGMOID + " batch");
	//-----------------------------------------
	CZZ* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::showcompare(msig, dsig, slots, SIGMOID);
	//-----------------------------------------
	cout << "!!! END TEST SIGMOID BATCH !!!" << endl;
}

void TestScheme::testSigmoidBatchLazy(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	long slots = 1 << logSlots;
	CZZ* mvec = new CZZ[slots];
	CZZ* msig = new CZZ[slots];
	for (long i = 0; i < slots; ++i) {
		RR mr = random_RR();
		RR mi = random_RR();
		mvec[i] = EvaluatorUtils::evalCZZ(mr, mi, logp);
		msig[i] = EvaluatorUtils::evalCZZSigmoid(mr, mi, logp);
	}
	EvaluatorUtils::leftShiftAndEqual(msig, slots, logp);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logQ);
	//-----------------------------------------
	timeutils.start(SIGMOID + " lazy");
	Ciphertext csig = algo.functionLazy(cipher, SIGMOID, logp, degree);
	timeutils.stop(SIGMOID + " lazy");
	//-----------------------------------------
	CZZ* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::showcompare(msig, dsig, slots, SIGMOID);
	//-----------------------------------------
	cout << "!!! END TEST SIGMOID LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testFFTBatch(long logN, long logQ, long logp, long logSlots, long logfftdim) {
	cout << "!!! START TEST FFT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	CZZ** mvec1 = new CZZ*[slots];
	CZZ** mvec2 = new CZZ*[slots];

	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::evalRandCZZArray(fftdim, logp);
		mvec2[i] = EvaluatorUtils::evalRandCZZArray(fftdim, logp);
	}

	Ciphertext* cvec1 = new Ciphertext[fftdim];
	Ciphertext* cvec2 = new Ciphertext[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		CZZ* mvals1 = new CZZ[slots];
		CZZ* mvals2	= new CZZ[slots];
		for (long i = 0; i < slots; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, slots, logQ);
		cvec2[j] = scheme.encrypt(mvals2, slots, logQ);
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < slots; ++i) {
		NumUtils::fft(mvec1[i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
		NumUtils::fft(mvec2[i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
		for (long j = 0; j < fftdim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
			mvec1[i][j] >>= logp;
		}
		NumUtils::fftInv(mvec1[i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
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
	algo.multModSwitchAndEqualVec(cvec1, cvec2, logp, fftdim);
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

void TestScheme::testFFTBatchLazy(long logN, long logQ, long logp, long logSlots, long logfftdim) {
	cout << "!!! START TEST FFT BATCH LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	CZZ** mvec1 = new CZZ*[slots];
	CZZ** mvec2 = new CZZ*[slots];

	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::evalRandCZZArray(fftdim, logp);
		mvec2[i] = EvaluatorUtils::evalRandCZZArray(fftdim, logp);
	}

	Ciphertext* cvec1 = new Ciphertext[fftdim];
	Ciphertext* cvec2 = new Ciphertext[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		CZZ* mvals1 = new CZZ[slots];
		CZZ* mvals2	= new CZZ[slots];
		for (long i = 0; i < slots; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, slots, logQ);
		cvec2[j] = scheme.encrypt(mvals2, slots, logQ);
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < slots; ++i) {
		NumUtils::fft(mvec1[i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
		NumUtils::fft(mvec2[i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
		for (long j = 0; j < fftdim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
			mvec1[i][j] >>= logp;
		}
		NumUtils::fftInvLazy(mvec1[i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
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
	algo.multModSwitchAndEqualVec(cvec1, cvec2, logp, fftdim);
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

void TestScheme::testFFTBatchLazyMultipleHadamard(long logN, long logQ, long logp, long logSlots, long logfftdim, long logHdim) {
	cout << "!!! START TEST FFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logQ);
	Context context(params);
	SecretKey secretKey(params);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long hdim = 1 << logHdim;
	long slots = 1 << logSlots;
	CZZ*** mvecs = new CZZ**[hdim];
	Ciphertext** cvecs = new Ciphertext*[hdim];
	for (long h = 0; h < hdim; ++h) {
		mvecs[h] = new CZZ*[slots];

		for (long i = 0; i < slots; ++i) {
			mvecs[h][i] = EvaluatorUtils::evalRandCZZArray(fftdim, logp);
		}

		cvecs[h] = new Ciphertext[fftdim];

		for (long j = 0; j < fftdim; ++j) {
			CZZ* mvals = new CZZ[slots];
			for (long i = 0; i < slots; ++i) {
				mvals[i] = mvecs[h][i][j];
			}
			cvecs[h][j] = scheme.encrypt(mvals, slots, logQ);
			delete[] mvals;
		}
		for (long i = 0; i < slots; ++i) {
			NumUtils::fft(mvecs[h][i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
		}
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			for (long s = logHdim - 1; s >= 0; --s) {
				long spow = 1 << s;
				for (long h = 0; h < spow; ++h) {
					mvecs[h][i][j] *= mvecs[h+spow][i][j];
					mvecs[h][i][j] >>= logp;
				}
			}
		}
	}

	for (long i = 0; i < slots; ++i) {
		NumUtils::fftInvLazy(mvecs[0][i], fftdim, context.ksiPowsr, context.ksiPowsi, context.M);
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
			algo.multModSwitchAndEqualVec(cvecs[h], cvecs[h+spow], logp, fftdim);
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

