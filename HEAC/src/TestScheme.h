#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

class TestScheme {
public:

	//-----------------------------------------

	/**
	 * Suggested: 13, 2, 30, 5, 3
	 */
	static void testEncodeBatch(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------

	/**
	 * Suggested: 13, 2, 30, 5, 3
	 */
	static void testConjugate(long logN, long logl, long logp, long L, long logSlots);

	/**
	 * Suggested: 13, 2, 30, 5, 1, 3
	 */
	static void testRotate2(long logN, long logl, long logp, long L, long rotlogSlots, long logSlots);

	/**
	 * Suggested: 13, 2, 30, 4, 17, 3
	 */
	static void testRotate(long logN, long logl, long logp, long L, long rotSlots, long logSlots);

	/**
	 * Suggested: 13, 2, 30, 4, 3
	 */
	static void testSlotssum(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------


	/**
	 * Suggested: 13, 2, 30, 5, 4, 3
	 */
	static void testPowerOf2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots);

	/**
	 * Suggested: 15, 3, 56, 11, 10
	 */
	static void testPowerOf2Extended(long logN, long logl, long logp, long L, long logDegree);

	//-----------------------------------------

	/**
	 * Suggested: 15, 4, 56, 11, 1024, 3
	 */
	static void testPowerBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 4, 30, 5, 13
	 */
	static void testPowerExtended(long logN, long logl, long logp, long L, long  degree);

	//-----------------------------------------

	/**
	 * Suggested: 13, 4, 30, 5, 4, 3
	 * Suggested: 15, 2, 56, 11, 10, 3
	 */
	static void testProd2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots);

	//-----------------------------------------


	/**
	 * Suggested: 13, 3, 25, 10, 8, 3
	 */
	static void testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots);

	/**
	 * Suggested: 13, 3, 25, 10, 8
	 */
	static void testInverseExtended(long logN, long logl, long logp, long L, long invSteps);

	//-----------------------------------------

	/**
	 * Suggested: 13, 4, 35, 6, 10, 3
	 */
	static void testLogarithmBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Suggested: 13, 4, 35, 6, 10, 3
	 */
	static void testExponentBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 37, 35, 4, 8, 3
	 */
	static void testExponentBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 2, 25, 6, 10
	 */
	static void testExponentExtended(long logN, long logl, long logp, long L, long degree);

	//-----------------------------------------

	/**
	 * Suggested: 13, 4, 35, 6, 10, 3
	 */
	static void testSigmoidBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 37, 35, 4, 8, 3
	 */
	static void testSigmoidBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots);

	/**
	 * Suggested: 13, 2, 25, 6, 10
	 */
	static void testSigmoidExtended(long logN, long logl, long logp, long L, long degree);

	//-----------------------------------------

	/**
	 * Suggested: 13, 5, 50, 3, 4
	 */
	static void testFFT(long logN, long logl, long logp, long L, long logfftdim);

	/**
	 * Suggested: 13, 7, 50, 4, 4, 3
	 */
	static void testFFTBatch(long logN, long logl, long logp, long L, long logfftdim, long logSlots);

	/**
	 * Suggested: 13, 51, 50, 2, 4
	 */
	static void testFFTLazy(long logN, long logl, long logp, long L, long logfftdim);

	//-----------------------------------------
};

#endif /* TEST_TESTSCHEME_H_ */
