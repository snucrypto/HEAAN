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

namespace heaan {

class EvaluatorUtils {
public:


	//----------------------------------------------------------------------------------
	//   RANDOM REAL AND COMPLEX NUMBERS
	//----------------------------------------------------------------------------------


	static double randomReal(double bound = 1.0);

	static std::complex<double> randomComplex(double bound = 1.0);

	static std::complex<double> randomCircle(double anglebound = 1.0);

	static double* randomRealArray(long size, double bound = 1.0);

	static std::complex<double>* randomComplexArray(long size, double bound = 1.0);

	static std::complex<double>* randomCircleArray(long size, double bound = 1.0);


	//----------------------------------------------------------------------------------
	//   DOUBLE & RR <-> ZZ
	//----------------------------------------------------------------------------------

	static double scaleDownToReal(const NTL::ZZ& x, const long logp);

	static NTL::ZZ scaleUpToZZ(const double x, const long logp);

	static NTL::ZZ scaleUpToZZ(const NTL::RR& x, const long logp);


	//----------------------------------------------------------------------------------
	//   ROTATIONS
	//----------------------------------------------------------------------------------


	static void leftRotateAndEqual(std::complex<double>* vals, const long n, const long r);

	static void rightRotateAndEqual(std::complex<double>* vals, const long n, const long r);

};

}  // namespace heaan

#endif
