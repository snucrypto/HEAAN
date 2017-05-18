#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

#include <iostream>

using namespace std;

class TestScheme {
public:

	//-----------------------------------------

	static void testEncodeBatch(long logN, long logl, long logp, long L, long logSlots);

	//-----------------------------------------

	static void testPowerOf2Batch(long logN, long logl, long logp, long L, long logPowDegree, long logSlots);
	static void testPowerOf2Extended(long logN, long logl, long logp, long L, long logPowDegree);

	//-----------------------------------------

	static void testPowerBatch(long logN, long logl, long logp, long L, long powDegree, long logSlots);
	static void testPowerExtended(long logN, long logl, long logp, long L, long  powDegree);

	//-----------------------------------------

	static void testProd2Batch(long logN, long logl, long logp, long L, long logProdDegree, long logSlots);

	//-----------------------------------------

	static void testInverseBatch(long logN, long logl, long logp, long L, long invSteps, long logSlots);
	static void testInverseExtended(long logN, long logl, long logp, long L, long invSteps);

	//-----------------------------------------

	static void testExponentBatch(long logN, long logl, long logp, long L, long expSteps, long logSlots);
	static void testLazyExponentBatch(long logN, long logl, long logp, long L, long expSteps, long logSlots);
	static void testExponentExtended(long logN, long logl, long logp, long L, long expSteps);

	//-----------------------------------------

	static void testSigmoidBatch(long logN, long logl, long logp, long L, long sigmoidSteps, long logSlots);
	static void testSigmoidExtended(long logN, long logl, long logp, long L, long sigmoidSteps);
	static void testLazySigmoidBatch(long logN, long logl, long logp, long L, long sigmoidSteps, long logSlots);
	//-----------------------------------------

	static void testFFT(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg);
	static void testLazyFFT(long logN, long logl, long logp, long L, long logFFTdim, long FFTdeg);
};

#endif /* TEST_TESTSCHEME_H_ */
