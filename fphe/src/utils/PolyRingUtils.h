#ifndef POLYSCHEME_ZRINGUTILS_H_
#define POLYSCHEME_ZRINGUTILS_H_

#include "NTL/ZZ.h"
#include "NTL/ZZX.h"

using namespace NTL;

class PolyRingUtils {
	public:
		static void addPolyRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi);
		static void subPolyRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi);
		static void mulPolyRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const ZZX& phi);
		static void mulPolyByConstantRing(ZZX& res, ZZX& poly, const ZZ& cnst, const ZZ& mod, const ZZX& phi);
		static void leftShiftPolyRing(ZZX& res, ZZX& poly, const long& cnst, const ZZ& mod, const ZZX& phi);
		static void rightShiftPolyRing(ZZX& res, ZZX& poly, const long& cnst, const ZZ& mod, const ZZX& phi);
		static void sampleGaussian(ZZX& res, long deg, double stdev);
		static void sampleUniform(ZZX& res, ZZ& B, long deg);
		static long mobius(long n);
		static ZZX Cyclotomic(long N);
};

#endif /* POLYSCHEME_ZRINGUTILS_H_ */
