#include "TestSGD.h"

#include <NTL/ZZ.h>
#include <cstdlib>
#include <iostream>

#include "../czz/CZZ.h"
#include "../scheme/Cipher.h"
#include "../scheme/Params.h"
#include "../scheme/PubKey.h"
#include "../scheme/Scheme.h"
#include "../scheme/SchemeAlgo.h"
#include "../scheme/SecKey.h"
#include "../sgd/SGD.h"
#include "../utils/StringUtils.h"
#include "../utils/TimeUtils.h"

void TestSGD::testSGD(long logN, long logl, long logp, long L, long logSlots, long logSample) {
	cout << "!!! START TEST SGD !!!" << endl;

	//-----------------------------------------
	TimeUtils timeutils;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, secretKey, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	SGD sgd(scheme, algo);
	//-----------------------------------------
	long slots = (1 << logSlots);
	long sample = (1 << logSample);

	Cipher* xsample = new Cipher[slots];
	Cipher* widx = new Cipher[slots];
	CZZ* wtrue = new CZZ[slots];
	CZZ** xij = new CZZ*[slots];
	for (long i = 0; i < slots; ++i) {
		CZZ* xi = new CZZ[sample];
		for (long j = 0; j < sample; ++j) {
			xi[j] = (rand() % 2) ? CZZ(params.p) : CZZ(0, 0);
		}
		CZZ wi = CZZ(RandomBits_ZZ(logp));
		wtrue[i] = CZZ(RandomBits_ZZ(logp));
		widx[i] = scheme.encryptFull(wi);
		xsample[i] = scheme.encryptFull(xi, sample);
		xij[i] = xi;
	}

	CZZ* yj = new CZZ[sample];
	for (long j = 0; j < sample; ++j) {
		yj[j] = CZZ();
		for (long i = 0; i < slots; ++i) {
			yj[j] += xij[i][j] * wtrue[i];
		}
		yj[j] >>= logp;
		yj[j] = (compare(yj[j].r, params.p) + 1) ? CZZ(params.p) : CZZ(0, 0);
	}

	Cipher ysample = scheme.encryptFull(yj, sample);

	long itter = 10;
	ZZ error = ZZ(100);
	for (long k = 0; k < itter; ++k) {
		timeutils.start("SGD");
		Cipher* grad = sgd.grad(ysample, xsample, widx, slots, sample);
		timeutils.stop("SGD");

		for (long i = 0; i < slots; ++i) {
			scheme.multByConstAndEqual(grad[i], error);
			scheme.modSwitchAndEqual(grad[i]);
			scheme.modEmbedAndEqual(widx[i], grad[i].level);
			scheme.addAndEqual(widx[i], grad[i]);
		}

		CZZ* dvec = scheme.decryptFullSingleArray(widx, slots);
		StringUtils::showcompare(wtrue, dvec, 2, "sgd");
		widx = scheme.encryptFullSingleArray(dvec, slots);
	}

	cout << "!!! END TEST SGD !!!" << endl;
}
