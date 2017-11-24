#include "TestScheme.h"

int main() {

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, logSlots
	 * Suggested: 13, 65, 30, 3
	 */

//	TestScheme::testEncodeBatch(13, 65, 30, 4);

	/*
	 * Params: logN, logQ, logp, logSlots
	 * Suggested: 13, 65, 30, 3
	 */

//	TestScheme::testConjugateBatch(13, 65, 30, 3);

	/*
	 * Params: logN, logQ, logp, logSlots
	 * Suggested: 13, 65, 30, 3
	 */

//	TestScheme::testimultBatch(13, 65, 30, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, rotlogSlots, logSlots, isLeft
	 * Suggested: 13, 65, 30, 2, 5, true
	 */

//	TestScheme::testRotateByPo2Batch(13, 65, 30, 2, 5, true);
//	TestScheme::testRotateBatch(13, 65, 30, 17, 5, true);

	/*
	 * Params: logN, logQ, logp, logSlots
	 * Suggested: 13, 65, 30, 3
	 */

//	TestScheme::testSlotsSum(13, 65, 30, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, logDegree, logSlots
	 * Suggested: 13, 155, 30, 4, 3
	 * Suggested: 15, 618, 56, 10, 3
	 */

//	TestScheme::testPowerOf2Batch(15, 618, 56, 10, 14);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, degree, logSlots
	 * Suggested: 13, 155, 30, 13, 3
	 * Suggested: 15, 618, 56, 903, 3
	 */

//	TestScheme::testPowerBatch(15, 618, 56, 903, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, logDegree, logSlots
	 * Suggested: 13, 155, 30, 4, 3
	 * Suggested: 15, 618, 56, 10, 3
	 */

//	TestScheme::testProdOfPo2Batch(13, 155, 30, 4, 3);

	/*
	 * Params: logN, logQ, logp, degree, logSlots
	 * Suggested: 13, 155, 30, 13, 3
	 * Suggested: 15, 618, 56, 903, 3
	 */

//	TestScheme::testProdBatch(13, 155, 30, 13, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, invSteps, logSlots
	 * Suggested: 14, 255, 25, 8, 3
	 * Suggested: 15, 325, 32, 8, 3
	 */

//	TestScheme::testInverseBatch(14, 285, 25, 8, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, degree, logSlots
	 * Suggested: 13, 155, 30, 7, 3
	 */

//	TestScheme::testLogarithmBatch(13, 155, 30, 7, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, degree, logSlots
	 * Suggested: 13, 155, 30, 7, 3
	 */

//	TestScheme::testExponentBatch(13, 155, 30, 7, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, degree, logSlots
	 * Suggested: 13, 155, 30, 7, 3
	 */

//	TestScheme::testSigmoidBatch(13, 155, 30, 7, 3);
//	TestScheme::testSigmoidBatchLazy(13, 155, 30, 7, 3);

	//-----------------------------------------

	/*
	 * Params: logN, logQ, logp, logSlots, logFFTdim
	 * Suggested: 13, 120, 50, 3, 4;
	 */

//	TestScheme::testFFTBatch(13, 100, 42, 3, 4);
//	TestScheme::testFFTBatchLazy(13, 120, 50, 3, 12);

	/*
	 * Params: logN, logQ, logp, logSlots, logFFTdim, logHdim
	 * Suggested: 13, 170, 50, 3, 4, 2;
	 */
//	TestScheme::testFFTBatchLazyMultipleHadamard(13, 122, 50, 3, 13, 1);

	return 0;
}
