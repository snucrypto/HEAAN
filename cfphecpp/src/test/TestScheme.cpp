#include "TestScheme.h"

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

	CZZ m;
	vector<CZZ> mvec;
	for (long i = 0; i < mvecsize; ++i) {
		m = params.ksiPows.pows[5][i % 3];
		mvec.push_back(m);
	}

	vector<CZZ> mconj = scheme.doubleConjugate(mvec);
	Message msg = scheme.encode(mconj);
	Cipher cipher = scheme.encrypt(msg);
	Message denc = scheme.decrypt(cipher);
	vector<CZZ> dconj = scheme.decode(denc);
	vector<CZZ> dvec = scheme.deConjugate(dconj);

	for (long i = 0; i < mvecsize; ++i) {
		cout << "---------------------" << endl;
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
		cout << "---------------------" << endl;
	}

	cout << "!!! STOP TEST ENCODE !!!" << endl;
}

void TestScheme::testOperations(long logN, long logp, long L) {
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

	long slots = (1 << (logN-1));
	CZZ m1, m2, madd, mmult, mmulte, mmultms;

	m1 = params.ksiPows.pows[4][1];
	m2 = params.ksiPows.pows[4][2];

	vector<CZZ> m1vec;
	vector<CZZ> m2vec;
	vector<CZZ> maddvec;
	vector<CZZ> mmultvec;
	vector<CZZ> mmultevec;
	vector<CZZ> mmultmsvec;


	for (long i = 0; i < slots; ++i) {
		m1vec.push_back(m1);
		m2vec.push_back(m2);
		maddvec.push_back(m1vec[i] + m2vec[i]);
		mmultvec.push_back(m1vec[i] * m2vec[i]);
		mmultevec.push_back(m1vec[i] * m2vec[i]);
		mmultmsvec.push_back(m1vec[i] * m2vec[i] / params.p);
	}

	vector<CZZ> m1conj = scheme.doubleConjugate(m1vec);
	vector<CZZ> m2conj = scheme.doubleConjugate(m2vec);
	vector<CZZ> maddconj = scheme.doubleConjugate(maddvec);
	vector<CZZ> mmultconj = scheme.doubleConjugate(mmultvec);
	vector<CZZ> mmulteconj = scheme.doubleConjugate(mmultevec);
	vector<CZZ> mmultmsconj = scheme.doubleConjugate(mmultmsvec);

	Message msg1 = scheme.encode(m1conj);
	Message msg2 = scheme.encode(m2conj);
	Message msgadd = scheme.encode(maddconj);
	Message msgmult = scheme.encode(mmultconj);
	Message msgmulte = scheme.encode(mmulteconj);
	Message msgmultms = scheme.encode(mmultmsconj);

	Cipher cipher1 = scheme.encrypt(msg1);
	Cipher cipher2 = scheme.encrypt(msg2);
	Cipher cmulte = scheme.encrypt(msg1);
	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher cadd = scheme.add(cipher1, cipher2);
	timeutils.stop("add");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult");
	Cipher cmult = scheme.mult(cipher1, cipher2);
	timeutils.stop("mult");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and equal");
	scheme.multAndEqual(cmulte, cipher2);
	timeutils.stop("mult and equal");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and mod switch");
	Cipher cmultms = scheme.multAndModSwitch(cipher1, cipher2);
	timeutils.stop("mult and mod switch");
	cout << "------------------" << endl;

	Message dmsg1 = scheme.decrypt(cipher1);
	Message dmsg2 = scheme.decrypt(cipher2);
	Message dmsgadd = scheme.decrypt(cadd);
	Message dmsgmult = scheme.decrypt(cmult);
	Message dmsgmulte = scheme.decrypt(cmulte);
	Message dmsgmultms = scheme.decrypt(cmultms);

	vector<CZZ> d1conj = scheme.decode(dmsg1);
	vector<CZZ> d2conj = scheme.decode(dmsg2);
	vector<CZZ> daddconj = scheme.decode(dmsgadd);
	vector<CZZ> dmultconj = scheme.decode(dmsgmult);
	vector<CZZ> dmulteconj = scheme.decode(dmsgmulte);
	vector<CZZ> dmultmsconj = scheme.decode(dmsgmultms);

	vector<CZZ> d1vec = scheme.deConjugate(d1conj);
	vector<CZZ> d2vec = scheme.deConjugate(d2conj);
	vector<CZZ> daddvec = scheme.deConjugate(daddconj);
	vector<CZZ> dmultvec = scheme.deConjugate(dmultconj);
	vector<CZZ> dmultevec = scheme.deConjugate(dmulteconj);
	vector<CZZ> dmultmsvec = scheme.deConjugate(dmultmsconj);

	cout << "------------------" << endl;
	cout << "m1:  " << m1vec[0].toString() << endl;
	cout << "d1:  " << d1vec[0].toString() << endl;
//	cout << "msg:  " << msg1.poly << endl;
//	cout << "dmsg:  " << dmsg1.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "m2:  " << m2vec[0].toString() << endl;
	cout << "d2:  " << d2vec[0].toString() << endl;
//	cout << "msg2:  " << msg2.poly << endl;
//	cout << "dmsg2:  " << dmsg2.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "madd:  " << maddvec[0].toString() << endl;
	cout << "dadd:  " << daddvec[0].toString() << endl;
//	cout << "msgadd:  " << msgadd.poly << endl;
//	cout << "dmsgadd:  " << dmsgadd.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmult:  " << mmultvec[0].toString() << endl;
	cout << "dmult:  " << dmultvec[0].toString() << endl;
//	cout << "msgmult:  " << msgmult.poly << endl;
//	cout << "dmsgmult:  " << dmsgmult.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmulte:  " << mmultevec[0].toString() << endl;
	cout << "dmulte:  " << dmultevec[0].toString() << endl;
//	cout << "msgmulte:  " << msgmulte.poly << endl;
//	cout << "dmsgmulte:  " << dmsgmulte.poly << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmultms:  " << mmultmsvec[0].toString() << endl;
	cout << "dmultms:  " << dmultmsvec[0].toString() << endl;
//	cout << "msgmultms:  " << msgmultms.poly << endl;
//	cout << "dmsgmultms:  " << dmsgmultms.poly << endl;
	cout << "------------------" << endl;

	cout << "!!! STOP TEST OPERATIONS !!!" << endl;
}

void TestScheme::testPow(long logN, long logp, long L, long logPowDegree) {
	cout << "!!! START TEST POW !!!" << endl;

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
	CZZ e, m, m2;

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
	algo.powerOf2(c2k, c, logPowDegree);
	timeutils.stop("Power of 2");
	cout << "------------------" << endl;

	for (long i = 0; i < c2k.size(); ++i) {
		Message dmsg = scheme.decrypt(c2k[i]);
		vector<CZZ> ddconj = scheme.decode(dmsg);
		vector<CZZ> dvec = scheme.deConjugate(ddconj);
		d2k.push_back(dvec[0]);
	}

	for (long i = 0; i < logPowDegree + 1; ++i) {
		e = m2k[i] - d2k[i];
		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i].toString() << endl;
		cout << "d: " << i << " " << d2k[i].toString() << endl;
		cout << "e: " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POW !!!" << endl;

}

void TestScheme::testProd(long logN, long logp, long L, long logProdDegree) {
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
	algo.prod2(cs2k, cs, logProdDegree);
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

void TestScheme::testInv(long logN, long logp, long L, long invSteps) {
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
	CZZ m, mbar, minv, e;
	vector<CZZ> d2k;
	vector<Cipher> c2k, v2k;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;
	halfp = params.p / 2;

	vector<CZZ> mbarconj = scheme.doubleConjugate(mbar);
	Message msgbar = scheme.encode(mbarconj);

	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(msgbar);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("Inverse");
	algo.inverse(c2k, v2k, c, invSteps);
	timeutils.stop("Inverse");
	cout << "------------------" << endl;

	for (long i = 0; i < v2k.size(); ++i) {
		Message dmsg = scheme.decrypt(v2k[i]);
		vector<CZZ> dconj = scheme.decode(dmsg);
		vector<CZZ> dvec = scheme.deConjugate(dconj);
		d2k.push_back(dvec[0]);
	}

	for (long i = 0; i < invSteps - 1; ++i) {
		e = minv - d2k[i];
		cout << "------------------" << endl;
		cout << "minv:  " << i << " " << minv.toString() << endl;
		cout << "ds:    " << i << " " << d2k[i].toString() << endl;
		cout << "es:    " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << v2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << v2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST INV !!!" << endl;
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
	vector<CZZ> dpx;

	for (long i = 0; i < FFTdeg; ++i) {
		mp1.push_back(params.ksiPows.pows[logN][i]);
		mp2.push_back(params.ksiPows.pows[logN][i]);
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

		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl;
}
