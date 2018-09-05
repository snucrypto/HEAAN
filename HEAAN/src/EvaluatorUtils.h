/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_EVALUATORUTILS_H_
#define HEAAN_EVALUATORUTILS_H_

#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <complex>

using namespace std;
using namespace NTL;

class EvaluatorUtils {
public:


	//----------------------------------------------------------------------------------
	//   RANDOM REAL AND COMPLEX NUMBERS
	//----------------------------------------------------------------------------------


	/**
	 * random real in [0, bound]
	 */
	static double randomReal(double bound = 1.0);

	/**
	 * random complex with real and imag parts in [0, bound]
	 */
	static complex<double> randomComplex(double bound = 1.0);

	/**
	 * random complex of norm 1 and angle in [0, anglebound]
	 */
	static complex<double> randomCircle(double anglebound = 1.0);


	/**
	 * random real array with elements in [0, bound]
	 */
	static double* randomRealArray(long size, double bound = 1.0);

	/**
	 * random complex array with elements real and imag parts in [0, bound]
	 */
	static complex<double>* randomComplexArray(long size, double bound = 1.0);

	/**
	 * random complex array of norm 1 and angle in [0, anglebound]
	 */
	static complex<double>* randomCircleArray(long size, double bound = 1.0);


	//----------------------------------------------------------------------------------
	//   DOUBLE & RR <-> ZZ
	//----------------------------------------------------------------------------------

	/**
	 * evaluating double(x / 2^logp)
	 */
	static double scaleDownToReal(const ZZ& x, const long logp);

	/**
	 * evaluating round(x * 2^logp)
	 */
	static ZZ scaleUpToZZ(const double x, const long logp);

	/**
	 * evaluating round(x * 2^logp)
	 */
	static ZZ scaleUpToZZ(const RR& x, const long logp);


	//----------------------------------------------------------------------------------
	//   ROTATIONS
	//----------------------------------------------------------------------------------


	/**
	 * left rotation of n elements by r slots
	 */
	static void leftRotateAndEqual(complex<double>* vals, const long n, const long r);

	/**
	 * right rotation of n elements by r slots
	 */
	static void rightRotateAndEqual(complex<double>* vals, const long n, const long r);

};

#endif
