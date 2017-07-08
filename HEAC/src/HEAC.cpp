#include "TestScheme.h"

int main() {

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logSlots
	 * Suggested: 13, 2, 30, 5, 3
	 */

//	TestScheme::testEncodeBatch(13, 5, 30, 5, 3);

	/*
	 * Params: logN, logl, logp, L, logSlots
	 * Suggested: 13, 2, 30, 5, 3
	 */

//	TestScheme::testConjugateBatch(13, 5, 30, 5, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, rotlogSlots, logSlots
	 * Suggested: 13, 2, 30, 5, 2, 5
	 */

//	TestScheme::testLeftRotateByPo2Batch(13, 2, 30, 5, 2, 5);
//	TestScheme::testLeftRotateBatch(13, 2, 30, 5, 17, 5);

	/*
	 * Params: logN, logl, logp, L, logSlots
	 * Suggested: 13, 2, 30, 5, 12
	 */
//	TestScheme::testSlotsSum(13, 2, 30, 5, 12);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logDegree, logSlots
	 * Suggested: 13, 2, 30, 5, 4, 3
	 * Suggested: 15, 2, 56, 11, 10, 3
	 */

//	TestScheme::testPowerOf2Batch(13, 2, 30, 5, 4, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 30, 5, 13, 3
	 * Suggested: 15, 2, 56, 11, 903, 3
	 */

//	TestScheme::testPowerBatch(15, 4, 56, 11, 903, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logDegree, logSlots
	 * Suggested: 13, 2, 30, 5, 4, 3
	 * Suggested: 15, 2, 56, 11, 10, 3
	 */

//	TestScheme::testProdOfPo2Batch(13, 2, 30, 5, 4, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, invSteps, logSlots
	 * Suggested: 14, 3, 25, 10, 8, 3
	 * Suggested: 15, 3, 32, 10, 8, 3
	 */

//	TestScheme::testInverseBatch(14, 3, 25, 10, 8, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 30, 5, 8, 3
	 */

//	TestScheme::testLogarithmBatch(13, 2, 30, 5, 8, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 30, 5, 7, 3
	 * Suggested: 13, 37, 30, 4, 7, 3 (for lazy)
	 */

//	TestScheme::testExponentBatch(13, 2, 30, 5, 7, 3);
//	TestScheme::testExponentBatchLazy(13, 32, 30, 4, 7, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, degree, logSlots
	 * Suggested: 13, 2, 30, 5, 7, 3
	 * Suggested: 13, 32, 30, 4, 7, 3 (for lazy)
	 */

//	TestScheme::testSigmoidBatch(13, 2, 30, 5, 7, 3);
//	TestScheme::testSigmoidBatchLazy(13, 32, 30, 4, 7, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logl, logp, L, logfftdim, logSlots
	 * Suggested: 14, 7, 48, 3, 4, 3;
	 * Suggested: 14, 55, 48, 2, 4 (for lazy)
	 */

//	TestScheme::testFFTBatch(13, 7, 48, 3, 4, 3);
//	TestScheme::testFFTLazy(13, 55, 48, 2, 4);

	return 0;
}
