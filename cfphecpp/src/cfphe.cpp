#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <iostream>
#include <vector>
#include <random>

#include "czz/CZZ.h"
#include "czz/CZZX.h"
#include "scheme/Cipher.h"
#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SchemeAlgo.h"
#include "scheme/SecKey.h"
#include "utils/NumUtils.h"
#include "utils/Ring2Utils.h"
#include "utils/TimeUtils.h"

using namespace std;
using namespace NTL;


void testDumb() {
	cout << "!!! START TEST DUMB !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 4;
	long logp = 30;
	long L = 5;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	vector<CZZ> mvec;
	CZZ m;
	for (long i = 0; i < params.n / 2; ++i) {
		m = params.cksi.pows[params.logn][i % 3];
		mvec.push_back(m);
	}

	cout << "!!! END TEST DUMB !!!" << endl;
}

void testEncode() {
	cout << "!!! START TEST ENCODE !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 1;
	long logp = 30;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long logSlots = 4;
	long slots = (1 << logSlots);

	CZZ m;
	vector<CZZ> mvec, dvec;

	for (long i = 0; i < slots; ++i) {
		m = params.cksi.pows[logSlots][i % 3];
		mvec.push_back(m);
	}
	Message msg = Message(mvec, params.p);

	cout << "---------------" << endl;
	Cipher cipher = scheme.encrypt(msg);
	Message dmsg = scheme.decrypt(cipher);
	cout << "---------------" << endl;

	for (long i = 0; i < slots; ++i) {
		cout << "---------------------" << endl;
		cout << "mi: " << i << " : " << msg.vals[i].toString() << endl;
		cout << "di: " << i << " : " << dmsg.vals[i].toString() << endl;
		cout << "---------------------" << endl;
	}

	cout << "!!! STOP TEST ENCODE !!!" << endl;
}

void testOperations() {
	cout << "!!! START TEST OPERATIONS !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 1;
	long logp = 30;
	long L = 2;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	CZZ m1, m2, madd, mmult, mmulte, mmultms;

	m1 = params.cksi.pows[5][2];
	m2 = params.cksi.pows[5][3];
	madd = m1 + m2;
	mmult = m1 * m2;
	mmulte = m1 * m2;
	mmultms = m1 * m2 / params.p;

	Message msg1 = Message(m1, params.p);
	Message msg2 = Message(m2, params.p);

	Cipher c1 = scheme.encrypt(msg1);
	Cipher cmulte = scheme.encrypt(msg1);
	Cipher c2 = scheme.encrypt(msg2);

	cout << "------------------" << endl;
	timeutils.start("add");
	Cipher cadd = scheme.add(c1, c2);
	timeutils.stop("add");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult");
	Cipher cmult = scheme.mult(c1, c2);
	timeutils.stop("mult");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and equal");
	scheme.multAndEqual(cmulte, c2);
	timeutils.stop("mult and equal");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mult and mod switch");
	Cipher cmultms = scheme.multAndModSwitch(c1, c2);
	timeutils.stop("mult and mod switch");
	cout << "------------------" << endl;

	Message d1 = scheme.decrypt(c1);
	Message d2 = scheme.decrypt(c2);
	Message dadd = scheme.decrypt(cadd);
	Message dmult = scheme.decrypt(cmult);
	Message dmulte = scheme.decrypt(cmulte);
	Message dmultms = scheme.decrypt(cmultms);

	cout << "------------------" << endl;
	cout << "m1:  " << m1.toString() << endl;
	cout << "d1:  " << d1.vals[0].toString() << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "m2:  " << m2.toString() << endl;
	cout << "d2:  " << d2.vals[0].toString() << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "madd:  " << madd.toString() << endl;
	cout << "dadd:  " << dadd.vals[0].toString() << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmult:  " << mmult.toString() << endl;
	cout << "dmult:  " << dmult.vals[0].toString() << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmulte:  " << mmulte.toString() << endl;
	cout << "dmulte:  " << dmulte.vals[0].toString() << endl;
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	cout << "mmultms:  " << mmultms.toString() << endl;
	cout << "dmultms:  " << dmultms.vals[0].toString() << endl;
	cout << "------------------" << endl;

	cout << "!!! STOP TEST OPERATIONS !!!" << endl;

}

void testPow() {
	cout << "!!! START TEST POW !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 15;
	long logl = 1;
	long logp = 56;
	long L = 11;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long logN = 5;
	long deg = 10;

	vector<CZZ> m2k;
	vector<Message> d2k;
	vector<Cipher> c2k;
	CZZ e, m, m2;

	m = params.cksi.pows[logN][2];
	m2k.push_back(m);
	for (long i = 0; i < deg; ++i) {
		m2 = (m2k[i] * m2k[i]) >> params.logp;
		m2k.push_back(m2);
	}
	Message msg = Message(m, params.p);
	Cipher c = scheme.encrypt(msg);

	cout << "------------------" << endl;
	timeutils.start("Power of 2");
	algo.powerOf2(c2k, c, deg);
	timeutils.stop("Power of 2");
	cout << "------------------" << endl;

	for (long i = 0; i < c2k.size(); ++i) {
		d2k.push_back(scheme.decrypt(c2k[i]));
	}

	for (long i = 0; i < deg + 1; ++i) {
		e = m2k[i] - d2k[i];
		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i].toString() << endl;
		cout << "d: " << i << " " << d2k[i].vals[0].toString() << endl;
		cout << "e: " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POW !!!" << endl;
}

void testProd2() {
	cout << "!!! START TEST PROD !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 15;
	long logl = 3;
	long logp = 56;
	long L = 11;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long deg = 6;
	long size = 1 << deg;
	long logN = 5;

	CZZ m, e;
	vector<CZZ> ms;
	vector<vector<CZZ>> ms2k;
	vector<vector<Message>> ds2k;
	vector<Cipher> cs;
	vector<vector<Cipher>> cs2k;

	for (long i = 0; i < size; ++i) {
		m = params.cksi.pows[logN][i % 3];
		Message msg = Message(m, params.p);
		Cipher c = scheme.encrypt(msg);
		ms.push_back(m);
		cs.push_back(c);
	}

	ms2k.push_back(ms);
	for (long i = 1; i < deg + 1; ++i) {
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
	algo.prod2(cs2k, cs, deg);
	timeutils.stop("Prod 2");
	cout << "------------------" << endl;

	for (long i = 0; i < deg + 1; ++i) {
		vector<Message> d2k;
		for (long j = 0; j < cs2k[i].size(); ++j) {
			d2k.push_back(scheme.decrypt(cs2k[i][j]));
		}
		ds2k.push_back(d2k);
	}

	for (long i = 0; i < deg + 1; ++i) {
		for (long j = 0; j < cs2k[i].size(); ++j) {
			e = ms2k[i][j] - ds2k[i][j];
			cout << "------------------" << endl;
			cout << "m: " << i << " " << j << " " << ms2k[i][j].toString() << endl;
			cout << "d: " << i << " " << j << " " << ds2k[i][j].vals[0].toString() << endl;
			cout << "e: " << i << " " << j << " " << e.toString() << endl;
//			cout << "eBnds: " << i << " " << j << " " << cs2k[i][j].eBnd << endl;
//			cout << "mBnds: " << i << " " << j << " " << cs2k[i][j].mBnd << endl;
			cout << "------------------" << endl;
		}
	}
	cout << "!!! END TEST PROD !!!" << endl;
}


void testInv() {
	cout << "!!! START TEST INV !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 2;
	long logp = 25;
	long L = 6;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long error = 13;
	long r = 6;

	ZZ halfp;
	CZZ m, mbar, minv, e;
	vector<Message> d2k;
	vector<Cipher> c2k, v2k;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= params.logp;
	m.r -= mbar.r;
	minv.r = params.p * params.p / m.r;
	halfp = params.p / 2;

	Message msgbar = Message(mbar, halfp);

	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(msgbar);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("Inverse");
	algo.inverse(c2k, v2k, c, r);
	timeutils.stop("Inverse");
	cout << "------------------" << endl;

	for (long i = 0; i < v2k.size(); ++i) {
		d2k.push_back(scheme.decrypt(v2k[i]));
	}

	for (long i = 0; i < r-1; ++i) {
		e = minv - d2k[i];
		cout << "------------------" << endl;
		cout << "minv:  " << i << " " << minv.toString() << endl;
		cout << "ds:    " << i << " " << d2k[i].vals[0].toString() << endl;
		cout << "es:    " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << v2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << v2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST INV !!!" << endl;
}

void testFFT() {
	cout << "!!! START TEST FFT !!!" << endl;

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 4;
	long logp = 30;
	long L = 3;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	//----------------------------

	long logN = 4;
	long N = 1 << logN;
	long deg = 5;

	CZZ zero, tmp;

	vector<CZZ> mp1, mp2, mpx;
	vector<CZZ> mfft1, mfft2, mfftx;
	vector<Cipher> cp1, cp2, cpx;
	vector<Cipher> cfft1, cfft2;
	vector<Message> dpx;

	for (long i = 0; i < deg; ++i) {
		mp1.push_back(params.cksi.pows[logN][i]);
		mp2.push_back(params.cksi.pows[logN][i]);
	}

	for (long i = deg; i < N; ++i) {
		mp1.push_back(zero);
		mp2.push_back(zero);
	}

	mfft1 = NumUtils::fft(mp1, params.cksi);
	mfft2 = NumUtils::fft(mp2, params.cksi);

	for (long i = 0; i < N; ++i) {
		tmp = (mfft1[i] * mfft2[i]) >> params.logp;
		mfftx.push_back(tmp);
	}

	mpx = NumUtils::fftInv(mfftx, params.cksi);

	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < N; ++i) {
		Message msgp1 = Message(mp1[i], params.p);
		Message msgp2 = Message(mp2[i], params.p);
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
	for (long i = 0; i < N; ++i) {
		scheme.multModSwitchAndEqual(cfft1[i], cfft2[i]);
	}
	timeutils.stop("mul fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftx inv");
	cpx = algo.fftInv(cfft1);
	timeutils.stop("cfftx inv");
	cout << "------------------" << endl;

	for (long i = 0; i < N; ++i) {
		dpx.push_back(scheme.decrypt(cpx[i]));
	}

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: mpx = " << mpx[i].toString() << endl;
		cout << i << " step: dpx = " << dpx[i].vals[0].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT !!!" << endl;

}

int main() {
//	----------------------------
	testDumb();
//	testEncode();
//	testOperations();
//	testPow();
//	testProd2();
//	testInv();
//	testFFT();
//	----------------------------

	return 0;
}
