#include "scheme/Params.h"
#include "scheme/PubKey.h"
#include "scheme/Scheme.h"
#include "scheme/SchemeAlgo.h"
#include "scheme/SecKey.h"
#include "test/TestSGD.h"
#include "test/TestScheme.h"
#include "utils/TimeUtils.h"

void test() {
	//-----------------------------------------
	TimeUtils timeutils;
	long logN = 6;
	long logl = 3;
	long logp = 30;
	long L = 5;
	Params params(logN, logl, logp, L);
	SecKey secretKey(params);
	PubKey publicKey(params, secretKey);
	SchemeAux schemeaux(logp, logN + 2);
	Scheme scheme(params, secretKey, publicKey, schemeaux);
	SchemeAlgo algo(scheme);
	//-----------------------------------------
}

int main() {

//	test();

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logSlots
	 * Suggested: 13, 2, 30, 5, 3
	 */

//	TestScheme::testEncodeBatch(13, 5, 30, 4, 12);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, rotlogSlots, logSlots
	 * Suggested: 13, 2, 30, 5, 0, 3
	 */

//	TestScheme::testRotate2(13, 2, 30, 4, 2, 11);
//	TestScheme::testRotate(13, 2, 30, 4, 17, 8);
//	TestScheme::testSlotssum(13, 2, 30, 4, 8);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logDegree, logSlots
	 * Suggested: 13, 2, 30, 5, 4, 3
	 * Suggested: 15, 2, 56, 11, 10, 3
	 */

//	TestScheme::testPowerOf2Batch(13, 2, 30, 5, 4, 3);
//	TestScheme::testPowerOf2Extended(15, 3, 56, 11, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 30, 5, 13, 3
	 * Suggested: 15, 2, 56, 11, 903, 3
	 */

//	TestScheme::testPowerBatch(15, 4, 56, 11, 1024, 3);
//	TestScheme::testPowerExtended(13, 4, 30, 5, 13);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logDegree, logSlots
	 * Suggested: 13, 2, 30, 5, 4, 3
	 * Suggested: 15, 2, 56, 11, 10, 3
	 */

//	TestScheme::testProd2Batch(13, 4, 30, 5, 4, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, invSteps, logSlots
	 * Suggested: 13, 3, 25, 10, 8, 3
	 * Suggested: 14, 3, 32, 10, 8, 3
	 */

//	TestScheme::testInverseBatch(13, 3, 25, 10, 8, 3);
//	TestScheme::testInverseExtended(13, 3, 25, 10, 8);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 35, 5, 8, 3
	 * Suggested: 13, 37, 35, 4, 8, 3 (for lazy)
	 */

//	TestScheme::testLogarithmBatch(13, 4, 35, 6, 10, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 35, 5, 8, 3
	 * Suggested: 13, 37, 35, 4, 8, 3 (for lazy)
	 */

//	TestScheme::testExponentBatch(19, 4, 35, 50, 7, 3); // less than 1 minute
//	TestScheme::testExponentBatch(13, 4, 35, 6, 10, 3);
//	TestScheme::testExponentBatchLazy(13, 37, 35, 4, 8, 3);
//	TestScheme::testExponentExtended(13, 2, 25, 6, 10);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 1, 35, 5, 7, 3
	 * Suggested: 13, 36, 35, 4, 7, 3 (for lazy)
	 */

//	TestScheme::testSigmoidBatch(13, 2, 35, 5, 7, 3);
//	TestScheme::testSigmoidBatchLazy(13, 36, 35, 4, 7, 3);
//	TestScheme::testSigmoidExtended(13, 1, 35, 5, 7);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logfftdim, logSlots
	 * Suggested: 14, 7, 50, 3, 4, 3;
	 * Suggested: 14, 7, 50, 2, 4 (for lazy)
	 */

	TestScheme::testFFT(13, 2, 50, 3, 4);
//	TestScheme::testFFTBatch(13, 7, 50, 4, 4, 3);
//	TestScheme::testFFTLazy(13, 51, 50, 2, 6);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L
	 * Suggested: 13, 2, 30, 4;
	 */

//	TestSGD::testSGD(13, 2, 30, 13);

	return 0;
}
