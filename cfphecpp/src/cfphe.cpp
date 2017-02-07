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


void testEncodeAll() {

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
	params.cksi.precompute(params.logn + 1);
	//----------------------------

	vector<CZZ> mvec;
	CZZ m;
	for (long i = 0; i < params.n; ++i) {
		m = params.cksi.pows[params.logn][i % 3];
		mvec.push_back(m);
	}

	vector<CZZ> fftInv = NumUtils::fftInv(mvec, params.cksi);
	vector<CZZ> fft = NumUtils::fft(fftInv, params.cksi);
	cout << "---------------" << endl;
	Cipher cipher = scheme.encryptAll(mvec, scheme.params.p);
	Cipher cipher2 = scheme.encrypt(params.cksi.pows[params.logn][1], scheme.params.p);
	vector<CZZ> dvec = scheme.decryptAll(cipher2);

	for (long i = 0; i < params.n; ++i) {
		cout << "---------------------" << endl;
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "mm: " << i << " : " << params.cksi.pows[params.logn][1].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
		cout << "---------------------" << endl;
	}
}

void testEncode() {
	cout << "!!! START TEST ENCODE !!!" << endl; // prints !!!Hello World!!!

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
	params.cksi.precompute(logn + 1);
	//----------------------------

	long logSlots = 4;
	long slots = (1 << logSlots);

	CZZ m;
	vector<CZZ> mvec, dvec;

	for (long i = 0; i < slots; ++i) {
		m = params.cksi.pows[logSlots][i % 3];
		mvec.push_back(m);
	}

	cout << "---------------" << endl;
	Cipher cipher = scheme.encrypt(logSlots, mvec, scheme.params.p);
	dvec = scheme.decrypt(logSlots, cipher);
	cout << "---------------" << endl;

	for (long i = 0; i < slots; ++i) {
		cout << "---------------------" << endl;
		cout << "mi: " << i << " : " << mvec[i].toString() << endl;
		cout << "di: " << i << " : " << dvec[i].toString() << endl;
		cout << "---------------------" << endl;
	}

	cout << "!!! STOP TEST ENCODE !!!" << endl; // prints !!!Hello World!!!
}

void testPow() {
	cout << "!!! START TEST POW !!!" << endl; // prints !!!Hello World!!!

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
	params.cksi.precompute(logn+1);
	//----------------------------

	long logN = 5;
	long deg = 10;

	vector<CZZ> m2k, d2k;
	vector<Cipher> c2k;
	CZZ e, m, m2;

	m = params.cksi.pows[logN][2];
	m2k.push_back(m);
	for (long i = 0; i < deg; ++i) {
		m2 = (m2k[i] * m2k[i]) >> params.logp;
		m2k.push_back(m2);
	}

	Cipher c = scheme.encrypt(m, scheme.params.p);

	cout << "------------------" << endl;
	timeutils.start("Power of 2");
	algo.powerOf2(c2k, c, deg);
	timeutils.stop("Power of 2");
	cout << "------------------" << endl;

	scheme.decrypt(d2k, c2k);

	for (long i = 0; i < deg + 1; ++i) {
		e = m2k[i] - d2k[i];
		cout << "------------------" << endl;
		cout << "m: " << i << " " << m2k[i].toString() << endl;
		cout << "d: " << i << " " << d2k[i].toString() << endl;
		cout << "e: " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << c2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << c2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST POW !!!" << endl; // prints !!!Hello World!!!
}

void testProd2() {
	cout << "!!! START TEST PROD !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 1;
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
	params.cksi.precompute(logn+1);
	//----------------------------

	long deg = 4;
	long size = 1 << deg;
	long logN = 5;

	CZZ m, e;
	vector<CZZ> ms;
	vector<vector<CZZ>> ms2k, ds2k;
	vector<Cipher> cs;
	vector<vector<Cipher>> cs2k;

	for (long i = 0; i < size; ++i) {
		m = params.cksi.pows[logN][i % 3];
		Cipher c = scheme.encrypt(m, scheme.params.p);
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
		vector<CZZ> d2k;
		scheme.decrypt(d2k, cs2k[i]);
		ds2k.push_back(d2k);
	}

	for (long i = 0; i < deg + 1; ++i) {
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
	cout << "!!! END TEST PROD !!!" << endl; // prints !!!Hello World!!!
}


void testInv() {
	cout << "!!! START TEST INV !!!" << endl; // prints !!!Hello World!!!

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
	vector<CZZ> d2k;
	vector<Cipher> c2k, v2k;

	mbar.r = RandomBits_ZZ(error);
	m.r = 1;
	m.r <<= scheme.params.logp;
	m.r -= mbar.r;
	minv.r = scheme.params.p * scheme.params.p / m.r;
	halfp = scheme.params.p / 2;

	cout << "------------------" << endl;
	timeutils.start("Encrypt c");
	Cipher c = scheme.encrypt(mbar, halfp);
	timeutils.stop("Encrypt c");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("Inverse");
	algo.inverse(c2k, v2k, c, r);
	timeutils.stop("Inverse");
	cout << "------------------" << endl;

	scheme.decrypt(d2k, v2k);

	for (long i = 0; i < r-1; ++i) {
		e = minv - d2k[i];
		cout << "------------------" << endl;
		cout << "minv:  " << i << " " << minv.toString() << endl;
		cout << "ds:    " << i << " " << d2k[i].toString() << endl;
		cout << "es:    " << i << " " << e.toString() << endl;
//		cout << "eBnds: " << i << " " << v2k[i].eBnd << endl;
//		cout << "mBnds: " << i << " " << v2k[i].mBnd << endl;
		cout << "------------------" << endl;
	}

	cout << "!!! END TEST INV !!!" << endl; // prints !!!Hello World!!!
}

void testFFTsimple() {
	cout << "!!! START TEST FFT SIMPLE!!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 1;
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
	params.cksi.precompute(logn + 2);
	//----------------------------

	long logN = 5;
	long N = 1 << logN;

	vector<CZZ> p, pfft, pfftinv, dfftinv;
	vector<Cipher> cp, cfft, cfftinv;


	for (long i = 0; i < N; ++i) {
		p.push_back(params.cksi.pows[logN][i]);
	}

	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < N; ++i) {
		Cipher c = scheme.encrypt(p[0], scheme.params.p);
		cp.push_back(c);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft");
//	cfft = algo.fft(cp, params.cksi);
	cfft = algo.fft2(cp);
	timeutils.stop("cfft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftinv");
//	cfftinv = algo.fftInv(cfft, params.cksi);
	cfftinv = algo.fftInv2(cfft);
	timeutils.stop("cfftinv");
	cout << "------------------" << endl;


	cout << "------------------" << endl;
	timeutils.start("fft");
	pfft = NumUtils::fft(p, params.cksi);
	timeutils.stop("fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft");
	pfftinv = NumUtils::fftInv(pfft, params.cksi);
	timeutils.stop("fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul and decrypt fft");
	for (long i = 0; i < cfftinv.size(); ++i) {
		dfftinv.push_back(scheme.decrypt2(cfftinv[i]));
	}
	timeutils.stop("mul and decrypt fft");
	cout << "------------------" << endl;

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: mfft = " << pfftinv[i].toString() << endl;
		cout << i << " step: dfft = " << dfftinv[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT SIMPLE!!!" << endl; // prints !!!Hello World!!!
}

void testFFTdirect() {
	cout << "!!! START TEST FFT DIRECT!!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 1;
	long logp = 30;
	long L = 8;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	params.cksi.precompute(logn+1);
	//----------------------------

	long logN = 5;
	long N = 1 << logN;
	long deg = 4;


	CZZ zero;
	vector<CZZ> p1, p2, dfft, mfft;
	vector<Cipher> cp1, cp2;


	for (long i = 0; i < deg; ++i) {
		p1.push_back(params.cksi.pows[logN][i]);
		p2.push_back(params.cksi.pows[logN][i]);
	}
	for (long i = deg; i < N; ++i) {
		p1.push_back(zero);
		p2.push_back(zero);
	}

	cout << "------------------" << endl;
	timeutils.start("Encrypting polynomials");
	for (long i = 0; i < N; ++i) {
		cp1.push_back(scheme.encrypt(p1[i], scheme.params.p));
		cp2.push_back(scheme.encrypt(p2[i], scheme.params.p));
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 1");
	vector<Cipher> cfft1 = algo.fft(cp1, params.cksi);
	timeutils.stop("cfft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 2");
	vector<Cipher> cfft2 = algo.fft(cp2, params.cksi);
	timeutils.stop("cfft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft 1");
	vector<CZZ> fft1 = NumUtils::fft(p1, params.cksi);
	timeutils.stop("fft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("fft 2");
	vector<CZZ> fft2 = NumUtils::fft(p2, params.cksi);
	timeutils.stop("fft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul and decrypt fft");
	for (long i = 0; i < cfft1.size(); ++i) {
		Cipher ms = scheme.multAndModSwitch(cfft1[i], cfft2[i]);
		dfft.push_back(scheme.decrypt(ms));
	}
	timeutils.stop("mul and decrypt fft");
	cout << "------------------" << endl;


	for (long i = 0; i < N; ++i) {
		CZZ mm = (fft1[i] * fft2[i]) >> params.logp;
		mfft.push_back(mm);
	}

	for (long i = 0; i < N; ++i) {
		cout << "----------------------" << endl;
		cout << i << " step: mfft = " << mfft[i].toString() << endl;
		cout << i << " step: dfft = " << dfft[i].toString() << endl;
		cout << "----------------------" << endl;
	}

	cout << "!!! END TEST FFT DIRECT!!!" << endl; // prints !!!Hello World!!!
}

void testFFTfull() {
	cout << "!!! START TEST FULL FFT !!!" << endl; // prints !!!Hello World!!!

	//----------------------------
	TimeUtils timeutils;
	long logn = 13;
	long logl = 3;
	long logp = 30;
	long L = 11;
	double sigma = 3;
	double rho = 0.5;
	long h = 64;
	Params params(logn, logl, logp, L, sigma, rho, h);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	Scheme scheme(params, secretKey, publicKey);
	SchemeAlgo algo(scheme);
	params.cksi.precompute(logn+1);
	//----------------------------

	long logN = 4;
	long N = 1 << logN;
	long deg = 5;

	CZZ zero, tmp;

	vector<CZZ> mp1, mp2, mpx;
	vector<CZZ> mfft1, mfft2, mfftx;
	vector<Cipher> cp1, cp2, cpx;
	vector<Cipher> cfft1, cfft2, cfftx;
	vector<CZZ> dpx;

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
		Cipher c1 = scheme.encrypt(mp1[i], scheme.params.p);
		Cipher c2 = scheme.encrypt(mp2[i], scheme.params.p);
		cp1.push_back(c1);
		cp2.push_back(c2);
	}
	timeutils.stop("Encrypting polynomials");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 1");
	cfft1 = algo.fft(cp1, params.cksi);
//	cfft1 = algo.fft2(cp1);
	timeutils.stop("cfft 1");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfft 2");
	cfft2 = algo.fft(cp2, params.cksi);
//	cfft2 = algo.fft2(cp2);
	timeutils.stop("cfft 2");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("mul fft");
	for (long i = 0; i < N; ++i) {
		Cipher cfftxi = scheme.multAndModSwitch(cfft1[i], cfft2[i]);
		cfftx.push_back(cfftxi);
	}
	timeutils.stop("mul fft");
	cout << "------------------" << endl;

	cout << "------------------" << endl;
	timeutils.start("cfftx inv");
	cpx = algo.fftInv(cfftx, params.cksi);
//	cpx = algo.fftInv2(cfftx);
	timeutils.stop("cfftx inv");
	cout << "------------------" << endl;

	for (long i = 0; i < N; ++i) {
		vector<CZZ> tmpvec;
		dpx.push_back(scheme.decrypt(cpx[i]));
	}

	for (long i = 0; i < N; ++i) {
		for (long j = 0; j < params.n; ++j) {
			cout << "----------------------" << endl;
			cout << i << " step: cpx = " << mpx[i].toString() << endl;
			cout << i << " step: dpx = " << dpx[i].toString() << endl;
			cout << "----------------------" << endl;
		}
	}

	cout << "!!! END TEST FULL FFT !!!" << endl; // prints !!!Hello World!!!

}

int main() {
//	testEncodeAll();
//	testEncode();
//	testPow();
//	testProd2();
//	testInv();
	testFFTsimple();
//	testFFTdirect();
//	testFFTfull();

	return 0;
}
