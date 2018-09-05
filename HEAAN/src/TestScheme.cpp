/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "TestScheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>

#include "Common.h"
#include "Ciphertext.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Scheme.h"
#include "SchemeAlgo.h"
#include "SecretKey.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include "Context.h"
#include "SerializationUtils.h"

using namespace std;
using namespace NTL;


//----------------------------------------------------------------------------------
//   STANDARD TESTS
//----------------------------------------------------------------------------------


void TestScheme::testEncodeBatch(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);

	timeutils.start("Encrypt batch");
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);
	timeutils.stop("Encrypt batch");

	timeutils.start("Decrypt batch");
	complex<double>* dvec = scheme.decrypt(secretKey, cipher);
	timeutils.stop("Decrypt batch");

	StringUtils::showcompare(mvec, dvec, slots, "val");

	cout << "!!! END TEST ENCODE BATCH !!!" << endl;
}

void TestScheme::testEncodeSingle(long logN, long logQ, long logp) {
	cout << "!!! START TEST ENCODE SINGLE !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	complex<double> mval = EvaluatorUtils::randomComplex();

	timeutils.start("Encrypt Single");
	Ciphertext cipher = scheme.encryptSingle(mval, logp, logQ);
	timeutils.stop("Encrypt Single");

	timeutils.start("Decrypt Single");
	complex<double> dval = scheme.decryptSingle(secretKey, cipher);
	timeutils.stop("Decrypt Single");

	StringUtils::showcompare(mval, dval, "val");

	cout << "!!! END TEST ENCODE SINGLE !!!" << endl;
}

void TestScheme::testBasic(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST BASIC !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	//-----------------------------------------
	SetNumThreads(1);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec1 = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* mvec2 = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* cvec = EvaluatorUtils::randomComplexArray(slots);

	complex<double>* mvecAdd = new complex<double>[slots];
	complex<double>* mvecMult = new complex<double>[slots];
	complex<double>* mvecCMult = new complex<double>[slots];
	for(long i = 0; i < slots; i++) {
		mvecAdd[i] = mvec1[i] + mvec2[i];
		mvecMult[i] = mvec1[i] * mvec2[i];
		mvecCMult[i] = mvec1[i] * cvec[i];
	}

	timeutils.start("Encrypt two batch");
	Ciphertext cipher1 = scheme.encrypt(mvec1, slots, logp, logQ);
	Ciphertext cipher2 = scheme.encrypt(mvec2, slots, logp, logQ);
	timeutils.stop("Encrypt two batch");

	timeutils.start("Homomorphic Addition");
	Ciphertext addCipher = scheme.add(cipher1, cipher2);
	timeutils.stop("Homomorphic Addition");

	timeutils.start("Homomorphic Multiplication");
	Ciphertext multCipher = scheme.mult(cipher1, cipher2);
	scheme.reScaleByAndEqual(multCipher, logp);
	timeutils.stop("Homomorphic Multiplication");

	timeutils.start("Homomorphic Multiplication");
	Ciphertext cmultCipher = scheme.multByConstVec(cipher1, cvec, slots, logp);
	scheme.reScaleByAndEqual(cmultCipher, logp);
	timeutils.stop("Homomorphic Multiplication");

	timeutils.start("Decrypt batch");
	complex<double>* dvecAdd = scheme.decrypt(secretKey, addCipher);
	complex<double>* dvecMult = scheme.decrypt(secretKey, multCipher);
	complex<double>* dvecCMult = scheme.decrypt(secretKey, cmultCipher);
	timeutils.stop("Decrypt batch");

	StringUtils::showcompare(mvecAdd, dvecAdd, slots, "add");
	StringUtils::showcompare(mvecMult, dvecMult, slots, "mult");
	StringUtils::showcompare(mvecCMult, dvecCMult, slots, "mult");
}

//-----------------------------------------

void TestScheme::testConjugateBatch(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST CONJUGATE BATCH !!!" << endl;
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	scheme.addConjKey(secretKey);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* mvecconj = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		mvecconj[i] = conj(mvec[i]);
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start("Conjugate batch");
	Ciphertext cconj = scheme.conjugate(cipher);
	timeutils.stop("Conjugate batch");

	complex<double>* dvecconj = scheme.decrypt(secretKey, cconj);

	StringUtils::showcompare(mvecconj, dvecconj, slots, "conj");

	cout << "!!! END TEST CONJUGATE BATCH !!!" << endl;
}

void TestScheme::testimultBatch(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST i MULTIPLICATION BATCH !!!" << endl;
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* imvec = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		imvec[i].real(-mvec[i].imag());
		imvec[i].imag(mvec[i].real());
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start("Multiplication by i batch");
	Ciphertext icipher = scheme.imult(cipher);
	timeutils.stop("Multiplication by i batch");

	complex<double>* idvec = scheme.decrypt(secretKey, icipher);

	StringUtils::showcompare(imvec, idvec, slots, "imult");

	cout << "!!! END TEST i MULTIPLICATION BATCH !!!" << endl;
}

void TestScheme::testRotateByPo2Batch(long logN, long logQ, long logp, long logRotSlots, long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BY POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	scheme.addLeftRotKeys(secretKey);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	long rotSlots = (1 << logRotSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	if(isLeft) {
		timeutils.start("Left Rotate by power of 2 batch");
		scheme.leftRotateByPo2AndEqual(cipher, logRotSlots);
		timeutils.stop("Left Rotate by power of 2 batch");
	} else {
		timeutils.start("Right Rotate by power of 2 batch");
		scheme.rightRotateByPo2AndEqual(cipher, logRotSlots);
		timeutils.stop("Right Rotate by power of 2 batch");
	}

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);

	if(isLeft) {
		EvaluatorUtils::leftRotateAndEqual(mvec, slots, rotSlots);
	} else {
		EvaluatorUtils::rightRotateAndEqual(mvec, slots, rotSlots);
	}

	StringUtils::showcompare(mvec, dvec, slots, "rot");
	//-----------------------------------------
	cout << "!!! END TEST ROTATE BY POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testRotateBatch(long logN, long logQ, long logp, long rotSlots, long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	scheme.addLeftRotKeys(secretKey);
	scheme.addRightRotKeys(secretKey);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	if(isLeft) {
		timeutils.start("Left rotate batch");
		scheme.leftRotateAndEqual(cipher, rotSlots);
		timeutils.stop("Left rotate batch");
	} else {
		timeutils.start("Right rotate batch");
		scheme.rightRotateAndEqual(cipher, rotSlots);
		timeutils.stop("Right rotate batch");
	}

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);

	if(isLeft) {
		EvaluatorUtils::leftRotateAndEqual(mvec, slots, rotSlots);
	} else {
		EvaluatorUtils::rightRotateAndEqual(mvec, slots, rotSlots);
	}

	StringUtils::showcompare(mvec, dvec, slots, "rot");

	cout << "!!! END TEST ROTATE BATCH !!!" << endl;
}

void TestScheme::testSlotsSum(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST SLOTS SUM !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	scheme.addLeftRotKeys(secretKey);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start("slots sum");
	algo.partialSlotsSumAndEqual(cipher, slots);
	timeutils.stop("slots sum");

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);

	complex<double> msum;
	for (long i = 0; i < slots; ++i) {
		msum += mvec[i];
	}

	StringUtils::showcompare(msum, dvec, slots, "slotsum");

	cout << "!!! END TEST SLOTS SUM !!!" << endl;
}


//----------------------------------------------------------------------------------
//   POWER & PRODUCT TESTS
//----------------------------------------------------------------------------------


void TestScheme::testPowerOf2Batch(long logN, long logQ, long logp, long logDegree, long logSlots) {
	cout << "!!! START TEST POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	long degree = 1 << logDegree;
	complex<double>* mvec = new complex<double>[slots];
	complex<double>* mpow = new complex<double>[slots];

	for (long i = 0; i < slots; ++i) {
		mvec[i] = EvaluatorUtils::randomCircle();
		mpow[i] = pow(mvec[i], degree);
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start("Power of 2 batch");
	Ciphertext cpow = algo.powerOf2(cipher, logp, logDegree);
	timeutils.stop("Power of 2 batch");

	complex<double>* dpow = scheme.decrypt(secretKey, cpow);

	StringUtils::showcompare(mpow, dpow, slots, "pow2");

	cout << "!!! END TEST POWER OF 2 BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPowerBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST POWER BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>* mvec = EvaluatorUtils::randomCircleArray(slots);
	complex<double>* mpow = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		mpow[i] = pow(mvec[i], degree);
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start("Power batch");
	Ciphertext cpow = algo.power(cipher, logp, degree);
	timeutils.stop("Power batch");

	complex<double>* dpow = scheme.decrypt(secretKey, cpow);

	StringUtils::showcompare(mpow, dpow, slots, "pow");

	cout << "!!! END TEST POWER BATCH !!!" << endl;
}

//-----------------------------------------

void TestScheme::testProdOfPo2Batch(long logN, long logQ, long logp, long logDegree, long logSlots) {
	cout << "!!! START TEST PROD OF POWER OF 2 BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(4);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	long degree = 1 << logDegree;

	complex<double>** mvec = new complex<double>*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = EvaluatorUtils::randomCircleArray(slots);
	}

	complex<double>* pvec = new complex<double>[slots];
	for (long j = 0; j < slots; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
		}
	}

	Ciphertext* cvec = new Ciphertext[degree];
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], slots, logp, logQ);
	}

	timeutils.start("Product of power of 2 batch");
	Ciphertext cprod = algo.prodOfPo2(cvec, logp, logDegree);
	timeutils.stop("Product of power of 2 batch");

	complex<double>* dvec = scheme.decrypt(secretKey, cprod);

	StringUtils::showcompare(pvec, dvec, slots, "prod");

	cout << "!!! END TEST PROD OF POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testProdBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST PROD BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(4);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>** mvec = new complex<double>*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = EvaluatorUtils::randomCircleArray(slots);
	}

	complex<double>* pvec = new complex<double>[slots];
	for (long j = 0; j < slots; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
		}
	}

	Ciphertext* cvec = new Ciphertext[degree];
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], slots, logp, logQ);
	}

	timeutils.start("Product batch");
	Ciphertext cprod = algo.prod(cvec, logp, degree);
	timeutils.stop("Product batch");

	complex<double>* dvec = scheme.decrypt(secretKey, cprod);

	StringUtils::showcompare(pvec, dvec, slots, "prod");

	cout << "!!! END TEST PROD BATCH !!!" << endl;
}


//----------------------------------------------------------------------------------
//   FUNCTION TESTS
//----------------------------------------------------------------------------------


void TestScheme::testInverseBatch(long logN, long logQ, long logp, long invSteps, long logSlots) {
	cout << "!!! START TEST INVERSE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>* mvec = EvaluatorUtils::randomCircleArray(slots, 0.1);
	complex<double>* minv = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		minv[i] = 1. / mvec[i];
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start("Inverse batch");
	Ciphertext cinv = algo.inverse(cipher, logp, invSteps);
	timeutils.stop("Inverse batch");

	complex<double>* dinv = scheme.decrypt(secretKey, cinv);

	StringUtils::showcompare(minv, dinv, slots, "inv");

	cout << "!!! END TEST INVERSE BATCH !!!" << endl;
}

void TestScheme::testLogarithmBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST LOGARITHM BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots, 0.1);
	complex<double>* mlog = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		mlog[i] = log(mvec[i] + 1.);
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start(LOGARITHM + " batch");
	Ciphertext clog = algo.function(cipher, LOGARITHM, logp, degree);
	timeutils.stop(LOGARITHM + " batch");

	complex<double>* dlog = scheme.decrypt(secretKey, clog);

	StringUtils::showcompare(mlog, dlog, slots, LOGARITHM);

	cout << "!!! END TEST LOGARITHM BATCH !!!" << endl;
}

void TestScheme::testExponentBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* mexp = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		mexp[i] = exp(mvec[i]);
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start(EXPONENT + " batch");
	Ciphertext cexp = algo.function(cipher, EXPONENT, logp, degree);
	timeutils.stop(EXPONENT + " batch");

	complex<double>* dexp = scheme.decrypt(secretKey, cexp);

	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);

	cout << "!!! END TEST EXPONENT BATCH !!!" << endl;
}

void TestScheme::testExponentBatchLazy(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST EXPONENT LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* mexp = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		mexp[i] = exp(mvec[i]);
	}
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start(EXPONENT + " lazy");
	Ciphertext cexp = algo.functionLazy(cipher, EXPONENT, logp, degree);
	timeutils.stop(EXPONENT + " lazy");

	complex<double>* dexp = scheme.decrypt(secretKey, cexp);

	StringUtils::showcompare(mexp, dexp, slots, EXPONENT);

	cout << "!!! END TEST EXPONENT LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoidBatch(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;

	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* msig = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		msig[i] = exp(mvec[i]) / (1. + exp(mvec[i]));
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start(SIGMOID + " batch");
	Ciphertext csig = algo.function(cipher, SIGMOID, logp, degree);
	timeutils.stop(SIGMOID + " batch");

	complex<double>* dsig = scheme.decrypt(secretKey, csig);

	StringUtils::showcompare(msig, dsig, slots, SIGMOID);

	cout << "!!! END TEST SIGMOID BATCH !!!" << endl;
}

void TestScheme::testSigmoidBatchLazy(long logN, long logQ, long logp, long degree, long logSlots) {
	cout << "!!! START TEST SIGMOID LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = 1 << logSlots;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);
	complex<double>* msig = new complex<double>[slots];
	for (long i = 0; i < slots; ++i) {
		msig[i] = exp(mvec[i]) / (1. + exp(mvec[i]));
	}

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	timeutils.start(SIGMOID + " lazy");
	Ciphertext csig = algo.functionLazy(cipher, SIGMOID, logp, degree);
	timeutils.stop(SIGMOID + " lazy");

	complex<double>* dsig = scheme.decrypt(secretKey, csig);

	StringUtils::showcompare(msig, dsig, slots, SIGMOID);

	cout << "!!! END TEST SIGMOID LAZY !!!" << endl;
}


//----------------------------------------------------------------------------------
//   FFT TESTS
//----------------------------------------------------------------------------------


void TestScheme::testFFTBatch(long logN, long logQ, long logp, long logSlots, long logfftdim) {
	cout << "!!! START TEST FFT BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	complex<double>** mvec1 = new complex<double>*[slots];
	complex<double>** mvec2 = new complex<double>*[slots];
	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::randomComplexArray(fftdim);
		mvec2[i] = EvaluatorUtils::randomComplexArray(fftdim);
	}

	Ciphertext* cvec1 = new Ciphertext[fftdim];
	Ciphertext* cvec2 = new Ciphertext[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		complex<double>* mvals1 = new complex<double>[slots];
		complex<double>* mvals2	= new complex<double>[slots];
		for (long i = 0; i < slots; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, slots, logp, logQ);
		cvec2[j] = scheme.encrypt(mvals2, slots, logp, logQ);
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < slots; ++i) {
		context.fft(mvec1[i], fftdim);
		context.fft(mvec2[i], fftdim);
		for (long j = 0; j < fftdim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
		}
		context.fftInv(mvec1[i], fftdim);
	}

	timeutils.start("ciphers fft 1 batch");
	algo.fft(cvec1, fftdim);
	timeutils.stop("ciphers fft 1 batch");

	timeutils.start("ciphers fft 2 batch");
	algo.fft(cvec2, fftdim);
	timeutils.stop("ciphers fft 2 batch");

	timeutils.start("ciphers hadamard mult batch");
	algo.multModSwitchAndEqualVec(cvec1, cvec2, logp, fftdim);
	timeutils.stop("ciphers hadamard mult batch");

	delete[] cvec2;

	timeutils.start("ciphers fft inverse batch");
	algo.fftInv(cvec1, fftdim);
	timeutils.stop("ciphers fft inverse batch");

	complex<double>** dvec1 = new complex<double>*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvec1[j] = scheme.decrypt(secretKey, cvec1[j]);
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvec1[i][j], dvec1[j][i], "fft");
		}
	}

	cout << "!!! END TEST FFT BATCH !!!" << endl;
}

void TestScheme::testFFTBatchLazy(long logN, long logQ, long logp, long logSlots, long logfftdim) {
	cout << "!!! START TEST FFT BATCH LAZY !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long slots = 1 << logSlots;
	complex<double>** mvec1 = new complex<double>*[slots];
	complex<double>** mvec2 = new complex<double>*[slots];
	for (long i = 0; i < slots; ++i) {
		mvec1[i] = EvaluatorUtils::randomComplexArray(fftdim);
		mvec2[i] = EvaluatorUtils::randomComplexArray(fftdim);
	}

	Ciphertext* cvec1 = new Ciphertext[fftdim];
	Ciphertext* cvec2 = new Ciphertext[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		complex<double>* mvals1 = new complex<double>[slots];
		complex<double>* mvals2	= new complex<double>[slots];
		for (long i = 0; i < slots; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, slots, logp, logQ);
		cvec2[j] = scheme.encrypt(mvals2, slots, logp, logQ);
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < slots; ++i) {
		context.fft(mvec1[i], fftdim);
		context.fft(mvec2[i], fftdim);
		for (long j = 0; j < fftdim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
		}
		context.fftInvLazy(mvec1[i], fftdim);
	}

	timeutils.start("ciphers fft 1");
	algo.fft(cvec1, fftdim);
	timeutils.stop("ciphers fft 1");

	timeutils.start("ciphers fft 2");
	algo.fft(cvec2, fftdim);
	timeutils.stop("ciphers fft 2");

	timeutils.start("ciphers hadamard mult");
	algo.multModSwitchAndEqualVec(cvec1, cvec2, logp, fftdim);
	timeutils.stop("ciphers hadamard mult");

	delete[] cvec2;

	timeutils.start("ciphers fft inverse lazy");
	algo.fftInvLazy(cvec1, fftdim);
	timeutils.stop("ciphers fft inverse lazy");

	complex<double>** dvec1 = new complex<double>*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvec1[j] = scheme.decrypt(secretKey, cvec1[j]);
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvec1[i][j], dvec1[j][i], "fft");
		}
	}

	cout << "!!! END TEST FFT BATCH LAZY !!!" << endl;
}

void TestScheme::testFFTBatchLazyMultipleHadamard(long logN, long logQ, long logp, long logSlots, long logfftdim, long logHdim) {
	cout << "!!! START TEST FFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	SetNumThreads(8);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long fftdim = 1 << logfftdim;
	long hdim = 1 << logHdim;
	long slots = 1 << logSlots;
	complex<double>*** mvecs = new complex<double>**[hdim];
	Ciphertext** cvecs = new Ciphertext*[hdim];
	for (long h = 0; h < hdim; ++h) {
		mvecs[h] = new complex<double>*[slots];
		for (long i = 0; i < slots; ++i) {
			mvecs[h][i] = EvaluatorUtils::randomComplexArray(fftdim);
		}

		cvecs[h] = new Ciphertext[fftdim];
		for (long j = 0; j < fftdim; ++j) {
			complex<double>* mvals = new complex<double>[slots];
			for (long i = 0; i < slots; ++i) {
				mvals[i] = mvecs[h][i][j];
			}
			cvecs[h][j] = scheme.encrypt(mvals, slots, logp, logQ);
			delete[] mvals;
		}
		for (long i = 0; i < slots; ++i) {
			context.fft(mvecs[h][i], fftdim);
		}
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			for (long s = logHdim - 1; s >= 0; --s) {
				long spow = 1 << s;
				for (long h = 0; h < spow; ++h) {
					mvecs[h][i][j] *= mvecs[h+spow][i][j];
				}
			}
		}
	}

	for (long i = 0; i < slots; ++i) {
		context.fftInvLazy(mvecs[0][i], fftdim);
	}

	for (long h = 1; h < hdim; ++h) {
		for (long i = 0; i < slots; ++i) {
			delete[] mvecs[h][i];
		}
		delete[] mvecs[h];
	}

	for (long h = 0; h < hdim; ++h) {
		timeutils.start("fft");
		algo.fft(cvecs[h], fftdim);
		timeutils.stop("fft");
	}

	for (long s = logHdim - 1; s >= 0; --s) {
		long spow = 1 << s;
		for (long h = 0; h < spow; ++h) {
			timeutils.start("hadamard mult");
			algo.multModSwitchAndEqualVec(cvecs[h], cvecs[h+spow], logp, fftdim);
			timeutils.stop("hadamard mult");
			delete[] cvecs[h+spow];
		}
	}

	timeutils.start("fft inverse lazy");
	algo.fftInvLazy(cvecs[0], fftdim);
	timeutils.stop("fft inverse lazy");

	complex<double>** dvec = new complex<double>*[fftdim];
	for (long j = 0; j < fftdim; ++j) {
		dvec[j] = scheme.decrypt(secretKey, cvecs[0][j]);
	}

	for (long i = 0; i < slots; ++i) {
		for (long j = 0; j < fftdim; ++j) {
			StringUtils::showcompare(mvecs[0][i][j], dvec[j][i], "fft");
		}
	}

	cout << "!!! END TEST FFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;
}

void TestScheme::testWriteAndRead(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST WRITE AND READ !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	string cipherPath = "testCiphertext.txt";
	timeutils.start("Write Ciphertext");
	SerializationUtils::writeCiphertext(cipher, cipherPath);
	timeutils.stop("Write Ciphertext");

	timeutils.start("Read Ciphertext");
	Ciphertext newcipher = SerializationUtils::readCiphertext(cipherPath);
	timeutils.stop("Read Ciphertext");

	if(newcipher.ax != cipher.ax || newcipher.bx != cipher.bx || newcipher.logq != cipher.logq || newcipher.slots != cipher.slots || newcipher.logp != cipher.logp) {
		cerr << "Write and Read for ciphertext does not work" << endl;
		cout << "difference ax = " << newcipher.ax - cipher.ax << endl;
		cout << "difference bx = " << newcipher.ax - cipher.bx << endl;
	} else {
		cout << "Write and Read for ciphertext works well" << endl;
	}

	string secretKeyPath = "testSecretKey.txt";
	timeutils.start("Write SecretKey");
	SerializationUtils::writeSecretKey(secretKey, secretKeyPath);
	timeutils.stop("Write SecretKey");

	timeutils.start("Read SecretKey");
	SecretKey newsecretKey = SerializationUtils::readSecretKey(secretKeyPath);
	timeutils.stop("Read SecretKey");
	if(secretKey.sx != newsecretKey.sx) {
		cout << "Write and Read for sk does not work" << endl;
	} else {
		cout << "Write and Read for sk works well" << endl;
	}

//	scheme.addLeftRotKeys(secretKey);
	scheme.addConjKey(secretKey);

	string schemeKeysPath = "testSchemeKeys.txt";
	timeutils.start("Write Scheme");
	SerializationUtils::writeSchemeKeys(scheme, schemeKeysPath);
	timeutils.stop("Write Scheme");

	string contextPath = "testContext.txt";
	timeutils.start("Write Context");
	SerializationUtils::writeContext(context, contextPath);
	timeutils.stop("Write Context");

	timeutils.start("Read Context");
	Context newcontext = SerializationUtils::readContext(contextPath);
	timeutils.stop("Read Context");

	Scheme newscheme(newcontext);

	timeutils.start("Read Scheme");
	SerializationUtils::readSchemeKeys(newscheme, schemeKeysPath);
	timeutils.stop("Read Scheme");

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);
	complex<double>* newdvec = newscheme.decrypt(newsecretKey, newcipher);

	StringUtils::showcompare(dvec, newdvec, slots, "r&w");

	cout << "!!! END TEST WRITE AND READ !!!" << endl;
}


void TestScheme::testBootstrap(long logN, long logp, long logq, long logQ, long logSlots, long logT) {
	cout << "!!! START TEST BOOTSTRAP !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	//-----------------------------------------
	timeutils.start("Key generating");
	scheme.addBootKey(secretKey, logSlots, logq + 4);
	timeutils.stop("Key generated");
	//-----------------------------------------
	SetNumThreads(1);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logq);

	cout << "cipher logq before: " << cipher.logq << endl;

	scheme.modDownToAndEqual(cipher, logq);
	scheme.normalizeAndEqual(cipher);
	cipher.logq = logQ;
	cipher.logp = logq + 4;

	timeutils.start("SubSum");
	for (long i = logSlots; i < context.logNh; ++i) {
		Ciphertext rot = scheme.leftRotateByPo2(cipher, i);
		scheme.addAndEqual(cipher, rot);
	}
	scheme.divByPo2AndEqual(cipher, context.logNh);
	timeutils.stop("SubSum");

	timeutils.start("CoeffToSlot");
	scheme.coeffToSlotAndEqual(cipher);
	timeutils.stop("CoeffToSlot");

	timeutils.start("EvalExp");
	scheme.evalExpAndEqual(cipher, logT);
	timeutils.stop("EvalExp");

	timeutils.start("SlotToCoeff");
	scheme.slotToCoeffAndEqual(cipher);
	timeutils.stop("SlotToCoeff");

	cipher.logp = logp;
	cout << "cipher logq after: " << cipher.logq << endl;

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);

	StringUtils::showcompare(mvec, dvec, slots, "boot");

	cout << "!!! END TEST BOOTSRTAP !!!" << endl;
}

void TestScheme::testBootstrapSingleReal(long logN, long logp, long logq, long logQ, long logT) {
	cout << "!!! START TEST BOOTSTRAP SINGLE REAL !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Context context(logN, logQ);
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, context);
	//-----------------------------------------
	timeutils.start("Key generating");
	scheme.addBootKey(secretKey, 0, logq + 4);
	timeutils.stop("Key generated");
	//-----------------------------------------
	SetNumThreads(1);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	double mval = EvaluatorUtils::randomReal();

	Ciphertext cipher = scheme.encryptSingle(mval, logp, logq);

	cout << "cipher logq before: " << cipher.logq << endl;
	scheme.modDownToAndEqual(cipher, logq);
	scheme.normalizeAndEqual(cipher);
	cipher.logq = logQ;

	timeutils.start("SubSum");
	for (long i = 0; i < context.logNh; ++i) {
		Ciphertext rot = scheme.leftRotateByPo2(cipher, i);
		scheme.addAndEqual(cipher, rot);
	}
	Ciphertext cconj = scheme.conjugate(cipher);
	scheme.addAndEqual(cipher, cconj);
	scheme.divByPo2AndEqual(cipher, context.logN);
	timeutils.stop("SubSum");

	timeutils.start("EvalExp");
	scheme.evalExpAndEqual(cipher, logT);
	timeutils.stop("EvalExp");

	cout << "cipher logq after: " << cipher.logq << endl;

	cipher.logp = logp;
	complex<double> dval = scheme.decryptSingle(secretKey, cipher);

	StringUtils::showcompare(mval, dval.real(), "boot");

	cout << "!!! END TEST BOOTSRTAP SINGLE REAL !!!" << endl;
}

