/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_TESTSCHEME_H_
#define HEAAN_TESTSCHEME_H_

class TestScheme {
public:


	//----------------------------------------------------------------------------------
	//   STANDARD TESTS
	//----------------------------------------------------------------------------------
	

	static void testEncrypt(long logN, long logQ, long logp, long logn);

	static void testEncryptSingle(long logN, long logQ, long logp);

	static void testBasic(long logN, long logQ, long logp, long logn);

	static void testimult(long logN, long logQ, long logp, long logn);


	//----------------------------------------------------------------------------------
	//   ROTATE & CONJUGATE TESTS
	//----------------------------------------------------------------------------------


	static void testRotateFast(long logN, long logQ, long logp, long logn, long r);

	static void testRotate(long logN, long logQ, long logp, long logn, long r);

	static void testConjugate(long logN, long logQ, long logp, long logn);


	//----------------------------------------------------------------------------------
	//   POWER & PRODUCT TESTS
	//----------------------------------------------------------------------------------


	static void testPowerOf2(long logN, long logQ, long logp, long logn, long logdeg);

	static void testPower(long logN, long logQ, long logp, long logn, long degree);

	static void testProdOfPo2(long logN, long logQ, long logp, long logn, long logdeg);

	static void testProd(long logN, long logQ, long logp, long logn, long degree);


	//----------------------------------------------------------------------------------
	//   FUNCTION TESTS
	//----------------------------------------------------------------------------------


	static void testInverse(long logN, long logQ, long logp, long logn, long steps);

	static void testLogarithm(long logN, long logQ, long logp, long logn, long degree);

	static void testExponent(long logN, long logQ, long logp, long logn, long degree);

	static void testExponentLazy(long logN, long logQ, long logp, long logn, long degree);

	static void testSigmoid(long logN, long logQ, long logp, long logn, long degree);

	static void testSigmoidLazy(long logN, long logQ, long logp, long logn, long degree);


	//----------------------------------------------------------------------------------
	//   DFT TESTS
	//----------------------------------------------------------------------------------


	static void testDFTBatch(long logN, long logQ, long logp, long logn, long logfft);

	static void testDFTLazyBatch(long logN, long logQ, long logp, long logn, long logfft);

	static void testDFTLazyBatchMultipleHadamard(long logN, long logQ, long logp, long logn, long logfftdim, long logh);

	static void testWriteAndRead(long logN, long logQ, long logp, long logn);


	//----------------------------------------------------------------------------------
	//   BOOTSTRAPPING TESTS
	//----------------------------------------------------------------------------------


	static void testBootstrap(long logN, long logq, long logQ, long logn, long nu, long logT);

	static void testBootstrapSingleReal(long logN, long logq, long logQ, long nu, long logT);

	static void test();
};

#endif
