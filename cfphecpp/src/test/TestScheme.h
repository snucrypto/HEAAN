#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

class TestScheme {
public:

	//-----------------------------------------

	static void testEncodeBatch(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------

	static void testRotate2(long logN, long logl, long logp, long L, long rotlogSlots, long logSlots);
	static void testRotate(long logN, long logl, long logp, long L, long rotSlots, long logSlots);

	//-----------------------------------------

	static void testPowerOf2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots);
	static void testPowerOf2Extended(long logN, long logl, long logp, long L, long logDegree);

	//-----------------------------------------

	static void testPowerBatch(long logN, long logl, long logp, long L, long degree, long logSlots);
	static void testPowerExtended(long logN, long logl, long logp, long L, long  degree);

	//-----------------------------------------

	static void testProd2Batch(long logN, long logl, long logp, long L, long logDegree, long logSlots);

	//-----------------------------------------

	static void testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots);
	static void testInverseExtended(long logN, long logl, long logp, long L, long invSteps);

	//-----------------------------------------

	static void testLogarithmBatch(long logN, long logl, long logp, long L, long degree, long logSlots);

	//-----------------------------------------

	static void testExponentBatch(long logN, long logl, long logp, long L, long degree, long logSlots);
	static void testExponentBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots);
	static void testExponentExtended(long logN, long logl, long logp, long L, long degree);

	//-----------------------------------------

	static void testSigmoidBatch(long logN, long logl, long logp, long L, long degree, long logSlots);
	static void testSigmoidBatchLazy(long logN, long logl, long logp, long L, long degree, long logSlots);
	static void testSigmoidExtended(long logN, long logl, long logp, long L, long degree);
	//-----------------------------------------

	static void testFFT(long logN, long logl, long logp, long L, long logfftdim);
	static void testFFTBatch(long logN, long logl, long logp, long L, long logfftdim, long logSlots);
	static void testFFTLazy(long logN, long logl, long logp, long L, long logfftdim);
};

#endif /* TEST_TESTSCHEME_H_ */
