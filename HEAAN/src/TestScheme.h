#ifndef TEST_TESTSCHEME_H_
#define TEST_TESTSCHEME_H_

class TestScheme {
public:

	//-----------------------------------------

	/**
	 * Testing encoding and decoding timing of the ciphertext
	 * c(m_1, ..., m_slots)
	 * number of levels switched: 0
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testEncodeBatch(long logN, long logq, long precisionBits, long logSlots);

	//-----------------------------------------

	/**
	 * Testing conjugation timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(conjugate(m_1), ...,conjugate(m_slots))
	 * number of levels switched: 0
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testConjugateBatch(long logN, long logq, long precisionBits, long logSlots);

	/**
	 * Testing multiplication by i (imaginary 1) timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(i * m_1, ...,i * m_slots)
	 * number of levels switched: 1
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testimultBatch(long logN, long logq, long precisionBits, long logSlots);

	/**
	 * Testing left rotation timing in the ciphertext
	 * number of levels switched: 0
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of rotation number
	 * @param[in] log of number of slots
	 */
	static void testRotateByPo2Batch(long logN, long logq, long precisionBits, long rotlogSlots, long logSlots, bool isLeft);

	/**
	 * Testing left rotation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * number of levels switched: 0
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] rotation number
	 * @param[in] log of number of slots
	 */
	static void testRotateBatch(long logN, long logq, long precisionBits, long rotSlots, long logSlots, bool isLeft);

	/**
	 * Testing slot summation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(sum(m_i), sum(m_i), ..., sum(m_i))
	 * number of levels switched: 0
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 */
	static void testSlotsSum(long logN, long logq, long precisionBits, long logSlots);

	//-----------------------------------------


	/**
	 * Testing power of 2 timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_1^degree/p^{degree-1}, ..., m_slots^degree/p^{degree-1})
	 * number of levels switched: logDegree
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of power degree
	 * @param[in] log of number of slots
	 */
	static void testPowerOf2Batch(long logN, long logq, long precisionBits, long logDegree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing power timing of the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_1^degree/p^{degree-1}, ..., m_slots^degree/p^{degree-1})
	 * number of levels switched: ceil(log(degree))
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] power degree
	 * @param[in] log of number of slots
	 */
	static void testPowerBatch(long logN, long logq, long precisionBits, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing product timing of ciphertexts
	 * array of c_i(m_1, ..., m_slots) -> c(prod_i(m_1), ..., prod_i(m_slots))
	 * number of levels switched: logDegree
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of ciphertexts
	 * @param[in] log of number of slots
	 */
	static void testProdOfPo2Batch(long logN, long logq, long precisionBits, long logDegree, long logSlots);

	/**
	 * Testing product timing of ciphertexts
	 * array of c_i(m_1, ..., m_slots) -> c(prod_i(m_1), ..., prod_i(m_slots))
	 * number of levels switched: ceil(log(degree))
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] number of ciphertexts
	 * @param[in] log of number of slots
	 */
	static void testProdBatch(long logN, long logq, long precisionBits, long degree, long logSlots);
	//-----------------------------------------

	/**
	 * Testing inverse timing of ciphertext
	 * c(m_1, ..., m_slots) -> c(p^2/m_1, ..., p^2/m_slots)
	 * number of levels switched: invSteps
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] number of steps used in method for calculating inverse
	 * @param[in] log of number of slots
	 */
	static void testInverseBatch(long logN, long logq, long precisionBits, long invSteps, long logSlots);

	//-----------------------------------------

	/**
	 * Testing logarithm timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(log(m_1), ..., log(m_slots))
	 * number of levels switched: ceil(log(degree))
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testLogarithmBatch(long logN, long logq, long precisionBits, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing exponent timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(exp(m_1/p) * p, ..., exp(m_slots/p) * p)
	 * number of levels switched: ceil(log(degree))
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testExponentBatch(long logN, long logq, long precisionBits, long degree, long logSlots);

	/**
	 * Testing exponent timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(exp(m_1/p) * p^2, ..., exp(m_slots/p) * p^2)
	 * number of levels switched: ceil(log(degree)) - 1
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testExponentBatchLazy(long logN, long logq, long precisionBits, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing sigmoid timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(sigmoid(m_1/p) * p, ..., sigmoid(m_slots/p) * p)
	 * number of levels switched: ceil(log(degree))
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testSigmoidBatch(long logN, long logq, long precisionBits, long degree, long logSlots);

	/**
	 * Testing sigmoid timing of ciphertext using Taylor series approximation
	 * c(m_1, ..., m_slots) -> c(sigmoid(m_1/p) * p^2, ..., sigmoid(m_slots/p) * p^2)
	 * number of levels switched: ceil(log(degree)) - 1
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] degree of Taylor series approximation
	 * @param[in] log of number of slots
	 */
	static void testSigmoidBatchLazy(long logN, long logq, long precisionBits, long degree, long logSlots);

	//-----------------------------------------

	/**
	 * Testing full fft pipeline timing of two ciphertext arrays
	 * fftinv( mult ( fft(c_1, ..., c_slots), fft(c_1, ..., c_slots) ) )
	 * number of levels switched: 2
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 * @param[in] log of fft dimension
	 */
	static void testFFTBatch(long logN, long logq, long precisionBits, long logfftdim, long logSlots);

	/**
	 * Testing full fft pipeline timing of two ciphertext arrays
	 * fftinvlazy( mult ( fft(c_1, ..., c_slots), fft(c_1, ..., c_slots) ) )
	 * number of levels switched: 1
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 * @param[in] log of fft dimension
	 */
	static void testFFTBatchLazy(long logN, long logq, long precisionBits, long logSlots, long logfftdim);

	/**
	 * Testing full fft pipeline timing of several ciphertext arrays with multiple hadamard multipications
	 * fftinvlazy( mult ( fft(c_1, ..., c_slots), fft(c_1, ..., c_slots) ) )
	 * number of levels switched: 1
	 * @param[in] logN input parameter for Params class
	 * @param[in] logq input parameter for Params class
	 * @param[in] log of number of slots
	 * @param[in] log of fft dimension
	 * @param[in] log of number of arrays
	 */
	static void testFFTBatchLazyMultipleHadamard(long logN, long logq, long precisionBits, long logSlots, long logfftdim, long logHdim);

	//-----------------------------------------
};

#endif /* TEST_TESTSCHEME_H_ */
