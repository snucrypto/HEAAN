#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

class TestScheme {
public:

	static void testEncode(long logN, long logp, long L, long logSlots);
	static void testOperations(long logN, long logp, long L);
	static void testPows(long logN, long logp, long L, long logDegree);
	static void testProd(long logN, long logp, long L, long logDegree);
	static void testInv(long logN, long logp, long L, long steps);
	static void testFFT(long logN, long logp, long L, long logFFTdim, long FFTdeg);
};

#endif /* TEST_TESTSCHEME_H_ */
