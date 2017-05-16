#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

class TestScheme {
public:

	static void testEncode(long logN, long logp, long L, long logSlots);
	static void testOperations(long logN, long logp, long L);
	static void testPow(long logN, long logp, long L, long logPowDegree);
	static void testProd(long logN, long logp, long L, long logProdDegree);
	static void testInv(long logN, long logp, long L, long invSteps);
	static void testFFT(long logN, long logp, long L, long logFFTdim, long FFTdeg);
};

#endif /* TEST_TESTSCHEME_H_ */
