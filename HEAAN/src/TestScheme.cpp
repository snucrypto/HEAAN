/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "TestScheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/ZZ.h>

#include "Common.h"
#include "Ciphertext.h"
#include "EvaluatorUtils.h"
#include "Ring.h"
#include "Scheme.h"
#include "SchemeAlgo.h"
#include "SecretKey.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include "SerializationUtils.h"

using namespace std;
using namespace NTL;


//----------------------------------------------------------------------------------
//   STANDARD TESTS
//----------------------------------------------------------------------------------


void TestScheme::testEncrypt(long logN, long logQ, long logp, long logn) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long n = (1 << logn);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);

	timeutils.start("Encrypt");
	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);
	timeutils.stop("Encrypt");

	timeutils.start("Decrypt");
	complex<double>* dvec = scheme.decrypt(secretKey, cipher);
	timeutils.stop("Decrypt");

	StringUtils::compare(mvec, dvec, n, "val");

	cout << "!!! END TEST ENCODE BATCH !!!" << endl;
}

void TestScheme::testEncryptSingle(long logN, long logQ, long logp) {
	cout << "!!! START TEST ENCODE SINGLE !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
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

	StringUtils::compare(mval, dval, "val");

	cout << "!!! END TEST ENCODE SINGLE !!!" << endl;
}

void TestScheme::testBasic(long logN, long logQ, long logp, long logn) {
	cout << "!!! START TEST BASIC !!!" << endl;

	srand(time(NULL));
	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);

	long n = (1 << logn);
	complex<double>* mvec1 = EvaluatorUtils::randomComplexArray(n);
	complex<double>* mvec2 = EvaluatorUtils::randomComplexArray(n);
	complex<double>* madd = new complex<double>[n];
	complex<double>* mmult = new complex<double>[n];
	complex<double>* mcmult = new complex<double>[n];
	complex<double>* cvec = EvaluatorUtils::randomComplexArray(n);

	for(long i = 0; i < n; i++) {
		madd[i] = mvec1[i] + mvec2[i];
		mmult[i] = mvec1[i] * mvec2[i];
		mcmult[i] = mvec1[i] * cvec[0].real();
	}

	Ciphertext cipher1 = scheme.encrypt(mvec1, n, logp, logQ);
	Ciphertext cipher2 = scheme.encrypt(mvec2, n, logp, logQ);

	timeutils.start("Addition");
	Ciphertext cadd = scheme.add(cipher1, cipher2);
	timeutils.stop("Addition");

	timeutils.start("Multiplication & Rescaling");
	Ciphertext cmult = scheme.mult(cipher1, cipher2);
	scheme.reScaleByAndEqual(cmult, logp);
	timeutils.stop("Multiplication & Rescaling");

	timeutils.start("Constant Multiplication & Rescaling");
	Ciphertext ccmult = scheme.multByConst(cipher1, cvec[0], logp);
	scheme.reScaleByAndEqual(ccmult, logp);
	timeutils.stop("Constant Multiplication & Rescaling");

	complex<double>* dadd = scheme.decrypt(secretKey, cadd);
	complex<double>* dmult = scheme.decrypt(secretKey, cmult);
	complex<double>* dcmult = scheme.decrypt(secretKey, ccmult);

	StringUtils::compare(madd, dadd, n, "add");
	StringUtils::compare(mmult, dmult, n, "mult");
	StringUtils::compare(mcmult, dcmult, n, "cmult");

	cout << "!!! END TEST BASIC !!!" << endl;

	cipher1.kill(); cipher2.kill();
	cadd.kill(); cmult.kill(); ccmult.kill();

	delete[] dadd; delete[] dmult; delete[] dcmult;
}

void TestScheme::testimult(long logN, long logQ, long logp, long logn) {
	cout << "!!! START TEST i MULTIPLICATION !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);

	long n = (1 << logn);

	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	complex<double>* imvec = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		imvec[i].real(-mvec[i].imag());
		imvec[i].imag(mvec[i].real());
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Multiplication by i");
	Ciphertext icipher = scheme.imult(cipher);
	timeutils.stop("Multiplication by i");

	complex<double>* idvec = scheme.decrypt(secretKey, icipher);
	StringUtils::compare(imvec, idvec, n, "imult");

	cout << "!!! END TEST i MULTIPLICATION !!!" << endl;
}


//----------------------------------------------------------------------------------
//   ROTATE & CONJUGATE
//----------------------------------------------------------------------------------


void TestScheme::testRotateFast(long logN, long logQ, long logp, long logn, long logr) {
	cout << "!!! START TEST ROTATE FAST !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring, true);

	long n = (1 << logn);
	long r = (1 << logr);
	scheme.addLeftRotKey(secretKey, r);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Left Rotate Fast");
	scheme.leftRotateFastAndEqual(cipher, r);
	timeutils.stop("Left Rotate Fast");

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);
	EvaluatorUtils::leftRotateAndEqual(mvec, n, r);
	StringUtils::compare(mvec, dvec, n, "rot");

	cout << "!!! END TEST ROTATE BY POWER OF 2 BATCH !!!" << endl;
}

void TestScheme::testRotate(long logN, long logQ, long logp, long logn, long r) {
	cout << "!!! START TEST ROTATE !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	scheme.addLeftRotKeys(secretKey);

	long n = (1 << logn);

	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Left rotate batch");
	scheme.leftRotateAndEqual(cipher, r);
	timeutils.stop("Left rotate batch");

	complex<double>* dvec = scheme.decrypt(secretKey, cipher);
	EvaluatorUtils::leftRotateAndEqual(mvec, n, r);
	StringUtils::compare(mvec, dvec, n, "rot");

	cout << "!!! END TEST ROTATE !!!" << endl;
}

void TestScheme::testConjugate(long logN, long logQ, long logp, long logn) {
	cout << "!!! START TEST CONJUGATE !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	scheme.addConjKey(secretKey);

	long n = (1 << logn);

	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	complex<double>* mvecconj = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		mvecconj[i] = conj(mvec[i]);
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Conjugate");
	Ciphertext cconj = scheme.conjugate(cipher);
	timeutils.stop("Conjugate");

	complex<double>* dvecconj = scheme.decrypt(secretKey, cconj);
	StringUtils::compare(mvecconj, dvecconj, n, "conj");

	cout << "!!! END TEST CONJUGATE !!!" << endl;
}


//----------------------------------------------------------------------------------
//   POWER & PRODUCT TESTS
//----------------------------------------------------------------------------------


void TestScheme::testPowerOf2(long logN, long logQ, long logp, long logn, long logdeg) {
	cout << "!!! START TEST POWER OF 2 !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	long degree = 1 << logdeg;
	complex<double>* mvec = new complex<double>[n];
	complex<double>* mpow = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		mvec[i] = EvaluatorUtils::randomCircle();
		mpow[i] = pow(mvec[i], degree);
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Power of 2");
	Ciphertext cpow = algo.powerOf2(cipher, logp, logdeg);
	timeutils.stop("Power of 2");

	complex<double>* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::compare(mpow, dpow, n, "pow2");

	cout << "!!! END TEST POWER OF 2 !!!" << endl;
}

//-----------------------------------------

void TestScheme::testPower(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST POWER !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>* mvec = EvaluatorUtils::randomCircleArray(n);
	complex<double>* mpow = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		mpow[i] = pow(mvec[i], degree);
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Power");
	Ciphertext cpow = algo.power(cipher, logp, degree);
	timeutils.stop("Power");

	complex<double>* dpow = scheme.decrypt(secretKey, cpow);
	StringUtils::compare(mpow, dpow, n, "pow");

	cout << "!!! END TEST POWER !!!" << endl;
}

//-----------------------------------------

void TestScheme::testProdOfPo2(long logN, long logQ, long logp, long logn, long logdeg) {
	cout << "!!! START TEST PROD OF POWER OF 2 !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	long degree = 1 << logdeg;

	complex<double>** mvec = new complex<double>*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = EvaluatorUtils::randomCircleArray(n);
	}
	complex<double>* pvec = new complex<double>[n];
	for (long j = 0; j < n; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
		}
	}
	Ciphertext* cvec = new Ciphertext[degree];
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], n, logp, logQ);
	}

	timeutils.start("Product of power of 2");
	Ciphertext cprod = algo.prodOfPo2(cvec, logp, logdeg);
	timeutils.stop("Product of power of 2");

	complex<double>* dvec = scheme.decrypt(secretKey, cprod);

	StringUtils::compare(pvec, dvec, n, "prod");

	cout << "!!! END TEST PROD OF POWER OF 2 !!!" << endl;
}

void TestScheme::testProd(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST PROD !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>** mvec = new complex<double>*[degree];
	for (long i = 0; i < degree; ++i) {
		mvec[i] = EvaluatorUtils::randomCircleArray(n);
	}
	complex<double>* pvec = new complex<double>[n];
	for (long j = 0; j < n; ++j) {
		pvec[j] = mvec[0][j];
		for (long i = 1; i < degree; ++i) {
			pvec[j] *= mvec[i][j];
		}
	}
	Ciphertext* cvec = new Ciphertext[degree];
	for (long i = 0; i < degree; ++i) {
		cvec[i] = scheme.encrypt(mvec[i], n, logp, logQ);
	}

	timeutils.start("Product");
	Ciphertext cprod = algo.prod(cvec, logp, degree);
	timeutils.stop("Product");

	complex<double>* dvec = scheme.decrypt(secretKey, cprod);
	StringUtils::compare(pvec, dvec, n, "prod");

	cout << "!!! END TEST PROD !!!" << endl;
}


//----------------------------------------------------------------------------------
//   FUNCTION TESTS
//----------------------------------------------------------------------------------


void TestScheme::testInverse(long logN, long logQ, long logp, long logn, long steps) {
	cout << "!!! START TEST INVERSE !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>* mvec = EvaluatorUtils::randomCircleArray(n, 0.1);
	complex<double>* minv = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		minv[i] = 1. / mvec[i];
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start("Inverse");
	Ciphertext cinv = algo.inverse(cipher, logp, steps);
	timeutils.stop("Inverse");

	complex<double>* dinv = scheme.decrypt(secretKey, cinv);
	StringUtils::compare(minv, dinv, n, "inv");

	cout << "!!! END TEST INVERSE !!!" << endl;
}

void TestScheme::testLogarithm(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST LOGARITHM !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n, 0.1);
	complex<double>* mlog = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		mlog[i] = log(mvec[i] + 1.);
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start(LOGARITHM);
	Ciphertext clog = algo.function(cipher, LOGARITHM, logp, degree);
	timeutils.stop(LOGARITHM);

	complex<double>* dlog = scheme.decrypt(secretKey, clog);
	StringUtils::compare(mlog, dlog, n, LOGARITHM);

	cout << "!!! END TEST LOGARITHM !!!" << endl;
}

void TestScheme::testExponent(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST EXPONENT !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	complex<double>* mexp = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		mexp[i] = exp(mvec[i]);
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start(EXPONENT);
	Ciphertext cexp = algo.function(cipher, EXPONENT, logp, degree);
	timeutils.stop(EXPONENT);

	complex<double>* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::compare(mexp, dexp, n, EXPONENT);

	cout << "!!! END TEST EXPONENT !!!" << endl;
}

void TestScheme::testExponentLazy(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST EXPONENT LAZY !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	complex<double>* mexp = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		mexp[i] = exp(mvec[i]);
	}
	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start(EXPONENT + " lazy");
	Ciphertext cexp = algo.functionLazy(cipher, EXPONENT, logp, degree);
	timeutils.stop(EXPONENT + " lazy");

	complex<double>* dexp = scheme.decrypt(secretKey, cexp);
	StringUtils::compare(mexp, dexp, n, EXPONENT);

	cout << "!!! END TEST EXPONENT LAZY !!!" << endl;
}

//-----------------------------------------

void TestScheme::testSigmoid(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST SIGMOID !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;

	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	complex<double>* msig = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		msig[i] = exp(mvec[i]) / (1. + exp(mvec[i]));
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start(SIGMOID);
	Ciphertext csig = algo.function(cipher, SIGMOID, logp, degree);
	timeutils.stop(SIGMOID);

	complex<double>* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::compare(msig, dsig, n, SIGMOID);

	cout << "!!! END TEST SIGMOID !!!" << endl;
}

void TestScheme::testSigmoidLazy(long logN, long logQ, long logp, long logn, long degree) {
	cout << "!!! START TEST SIGMOID LAZY !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long n = 1 << logn;
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(n);
	complex<double>* msig = new complex<double>[n];
	for (long i = 0; i < n; ++i) {
		msig[i] = exp(mvec[i]) / (1. + exp(mvec[i]));
	}

	Ciphertext cipher = scheme.encrypt(mvec, n, logp, logQ);

	timeutils.start(SIGMOID + " lazy");
	Ciphertext csig = algo.functionLazy(cipher, SIGMOID, logp, degree);
	timeutils.stop(SIGMOID + " lazy");

	complex<double>* dsig = scheme.decrypt(secretKey, csig);
	StringUtils::compare(msig, dsig, n, SIGMOID);

	cout << "!!! END TEST SIGMOID LAZY !!!" << endl;
}


//----------------------------------------------------------------------------------
//   DFT
//----------------------------------------------------------------------------------


void TestScheme::testDFTBatch(long logN, long logQ, long logp, long logn, long logdim) {
	cout << "!!! START TEST DFT BATCH !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long dim = 1 << logdim;
	long n = 1 << logn;
	complex<double>** mvec1 = new complex<double>*[n];
	complex<double>** mvec2 = new complex<double>*[n];
	for (long i = 0; i < n; ++i) {
		mvec1[i] = EvaluatorUtils::randomComplexArray(dim);
		mvec2[i] = EvaluatorUtils::randomComplexArray(dim);
	}

	Ciphertext* cvec1 = new Ciphertext[dim];
	Ciphertext* cvec2 = new Ciphertext[dim];
	for (long j = 0; j < dim; ++j) {
		complex<double>* mvals1 = new complex<double>[n];
		complex<double>* mvals2	= new complex<double>[n];
		for (long i = 0; i < n; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, n, logp, logQ);
		cvec2[j] = scheme.encrypt(mvals2, n, logp, logQ);
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < n; ++i) {
		ring.DFT(mvec1[i], dim);
		ring.DFT(mvec2[i], dim);
		for (long j = 0; j < dim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
		}
		ring.IDFT(mvec1[i], dim);
	}

	timeutils.start("DFT 1");
	algo.DFT(cvec1, dim);
	timeutils.stop("DFT 1");

	timeutils.start("DFT 2");
	algo.DFT(cvec2, dim);
	timeutils.stop("DFT 2");

	timeutils.start("Hadamard Mult");
	algo.multModSwitchAndEqualVec(cvec1, cvec2, logp, dim);
	timeutils.stop("Hadamard Mult");

	delete[] cvec2;

	timeutils.start("IDFT");
	algo.IDFT(cvec1, dim);
	timeutils.stop("IDFT");

	complex<double>** dvec1 = new complex<double>*[dim];
	for (long j = 0; j < dim; ++j) {
		dvec1[j] = scheme.decrypt(secretKey, cvec1[j]);
	}
	for (long i = 0; i < n; ++i) {
		for (long j = 0; j < dim; ++j) {
			StringUtils::compare(mvec1[i][j], dvec1[j][i], "fft");
		}
	}

	cout << "!!! END TEST DFT BATCH !!!" << endl;
}

void TestScheme::testDFTLazyBatch(long logN, long logQ, long logp, long logn, long logdim) {
	cout << "!!! START TEST DFT BATCH LAZY !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long dim = 1 << logdim;
	long n = 1 << logn;
	complex<double>** mvec1 = new complex<double>*[n];
	complex<double>** mvec2 = new complex<double>*[n];
	for (long i = 0; i < n; ++i) {
		mvec1[i] = EvaluatorUtils::randomComplexArray(dim);
		mvec2[i] = EvaluatorUtils::randomComplexArray(dim);
	}

	Ciphertext* cvec1 = new Ciphertext[dim];
	Ciphertext* cvec2 = new Ciphertext[dim];
	for (long j = 0; j < dim; ++j) {
		complex<double>* mvals1 = new complex<double>[n];
		complex<double>* mvals2	= new complex<double>[n];
		for (long i = 0; i < n; ++i) {
			mvals1[i] = mvec1[i][j];
			mvals2[i] = mvec2[i][j];
		}
		cvec1[j] = scheme.encrypt(mvals1, n, logp, logQ);
		cvec2[j] = scheme.encrypt(mvals2, n, logp, logQ);
		delete[] mvals1;
		delete[] mvals2;
	}

	for (long i = 0; i < n; ++i) {
		ring.DFT(mvec1[i], dim);
		ring.DFT(mvec2[i], dim);
		for (long j = 0; j < dim; ++j) {
			mvec1[i][j] *= mvec2[i][j];
		}
		ring.IDFTLazy(mvec1[i], dim);
	}

	timeutils.start("DFT 1");
	algo.DFT(cvec1, dim);
	timeutils.stop("DFT 1");

	timeutils.start("DFT 2");
	algo.DFT(cvec2, dim);
	timeutils.stop("DFT 2");

	timeutils.start("Hadamard Mult");
	algo.multModSwitchAndEqualVec(cvec1, cvec2, logp, dim);
	timeutils.stop("Hadamard Mult");

	delete[] cvec2;

	timeutils.start("IDFT Lazy");
	algo.IDFTLazy(cvec1, dim);
	timeutils.stop("IDFT Lazy");

	complex<double>** dvec1 = new complex<double>*[dim];
	for (long j = 0; j < dim; ++j) {
		dvec1[j] = scheme.decrypt(secretKey, cvec1[j]);
	}
	for (long i = 0; i < n; ++i) {
		for (long j = 0; j < dim; ++j) {
			StringUtils::compare(mvec1[i][j], dvec1[j][i], "fft");
		}
	}

	cout << "!!! END TEST DFT BATCH LAZY !!!" << endl;
}

void TestScheme::testDFTLazyBatchMultipleHadamard(long logN, long logQ, long logp, long logn, long logdim, long logh) {
	cout << "!!! START TEST DFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	SchemeAlgo algo(scheme);

	long dim = 1 << logdim;
	long hdim = 1 << logh;
	long n = 1 << logn;
	complex<double>*** mvecs = new complex<double>**[hdim];
	Ciphertext** cvecs = new Ciphertext*[hdim];
	for (long h = 0; h < hdim; ++h) {
		mvecs[h] = new complex<double>*[n];
		for (long i = 0; i < n; ++i) {
			mvecs[h][i] = EvaluatorUtils::randomComplexArray(dim);
		}

		cvecs[h] = new Ciphertext[dim];
		for (long j = 0; j < dim; ++j) {
			complex<double>* mvals = new complex<double>[n];
			for (long i = 0; i < n; ++i) {
				mvals[i] = mvecs[h][i][j];
			}
			cvecs[h][j] = scheme.encrypt(mvals, n, logp, logQ);
			delete[] mvals;
		}
		for (long i = 0; i < n; ++i) {
			ring.DFT(mvecs[h][i], dim);
		}
	}

	for (long i = 0; i < n; ++i) {
		for (long j = 0; j < dim; ++j) {
			for (long s = logh - 1; s >= 0; --s) {
				long spow = 1 << s;
				for (long h = 0; h < spow; ++h) {
					mvecs[h][i][j] *= mvecs[h+spow][i][j];
				}
			}
		}
	}

	for (long i = 0; i < n; ++i) {
		ring.IDFTLazy(mvecs[0][i], dim);
	}

	for (long h = 1; h < hdim; ++h) {
		for (long i = 0; i < n; ++i) {
			delete[] mvecs[h][i];
		}
		delete[] mvecs[h];
	}

	timeutils.start("DFT");
	for (long h = 0; h < hdim; ++h) {
		algo.DFT(cvecs[h], dim);
	}
	timeutils.stop("DFT");

	timeutils.start("Hadamard Mult");
	for (long s = logh - 1; s >= 0; --s) {
		long spow = 1 << s;
		for (long h = 0; h < spow; ++h) {
			algo.multModSwitchAndEqualVec(cvecs[h], cvecs[h+spow], logp, dim);
			delete[] cvecs[h+spow];
		}
	}
	timeutils.stop("Hadamard Mult");

	timeutils.start("IDFT Lazy");
	algo.IDFTLazy(cvecs[0], dim);
	timeutils.stop("IDFT Lazy");

	complex<double>** dvec = new complex<double>*[dim];
	for (long j = 0; j < dim; ++j) {
		dvec[j] = scheme.decrypt(secretKey, cvecs[0][j]);
	}
	for (long i = 0; i < n; ++i) {
		for (long j = 0; j < dim; ++j) {
			StringUtils::compare(mvecs[0][i][j], dvec[j][i], "fft");
		}
	}

	cout << "!!! END TEST DFT BATCH LAZY MULTIPLE HADAMARD !!!" << endl;
}

void TestScheme::testWriteAndRead(long logN, long logQ, long logp, long logSlots) {
	cout << "!!! START TEST WRITE AND READ !!!" << endl;

	cout << "!!! END TEST WRITE AND READ !!!" << endl;
}


void TestScheme::testBootstrap(long logN, long logp, long logq, long logQ, long logSlots, long logT) {
	cout << "!!! START TEST BOOTSTRAP !!!" << endl;

	srand(time(NULL));
	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);

	timeutils.start("Key generating");
	scheme.addBootKey(secretKey, logSlots, logq + 4);
	timeutils.stop("Key generated");

	long slots = (1 << logSlots);
	complex<double>* mvec = EvaluatorUtils::randomComplexArray(slots);

	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logq);

	cout << "cipher logq before: " << cipher.logq << endl;

	scheme.modDownToAndEqual(cipher, logq);
	scheme.normalizeAndEqual(cipher);
	cipher.logq = logQ;
	cipher.logp = logq + 4;

	timeutils.start("SubSum");
	for (long i = logSlots; i < ring.logNh; ++i) {
		Ciphertext rot = scheme.leftRotateFast(cipher, (1 << i));
		scheme.addAndEqual(cipher, rot);
	}
	scheme.divByPo2AndEqual(cipher, ring.logNh);
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

	StringUtils::compare(mvec, dvec, slots, "boot");

	cout << "!!! END TEST BOOTSRTAP !!!" << endl;
}

void TestScheme::testBootstrapSingleReal(long logN, long logp, long logq, long logQ, long logT) {
	cout << "!!! START TEST BOOTSTRAP SINGLE REAL !!!" << endl;

	srand(time(NULL));
//	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring(logN, logQ);
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);

	timeutils.start("Key generating");
	scheme.addBootKey(secretKey, 0, logq + 4);
	timeutils.stop("Key generated");

	double mval = EvaluatorUtils::randomReal();

	Ciphertext cipher = scheme.encryptSingle(mval, logp, logq);

	cout << "cipher logq before: " << cipher.logq << endl;
	scheme.modDownToAndEqual(cipher, logq);
	scheme.normalizeAndEqual(cipher);
	cipher.logq = logQ;

	timeutils.start("SubSum");
	for (long i = 0; i < ring.logNh; ++i) {
		Ciphertext rot = scheme.leftRotateFast(cipher, 1 << i);
		scheme.addAndEqual(cipher, rot);
	}
	Ciphertext cconj = scheme.conjugate(cipher);
	scheme.addAndEqual(cipher, cconj);
	scheme.divByPo2AndEqual(cipher, ring.logN);
	timeutils.stop("SubSum");

	timeutils.start("EvalExp");
	scheme.evalExpAndEqual(cipher, logT);
	timeutils.stop("EvalExp");

	cout << "cipher logq after: " << cipher.logq << endl;

	cipher.logp = logp;
	complex<double> dval = scheme.decryptSingle(secretKey, cipher);

	StringUtils::compare(mval, dval.real(), "boot");

	cout << "!!! END TEST BOOTSRTAP SINGLE REAL !!!" << endl;
}

void TestScheme::test() {
}

