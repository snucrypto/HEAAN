#ifndef POLYSCHEME_ZRINGUTILS_H_
#define POLYSCHEME_ZRINGUTILS_H_

#include "NTL/ZZ.h"
#include "NTL/ZZX.h"

using namespace NTL;

class PolyRingUtils {
	public:
		static void addPolyRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const long& deg);
		static void subPolyRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const long& deg);
		static void mulPolyRing(ZZX& res, ZZX& poly1, ZZX& poly2, const ZZ& mod, const long& deg);
		static void mulPolyByConstantRing(ZZX& res, ZZX& poly, const ZZ& cnst, const ZZ& mod, const long& deg);
		static void leftShiftPolyRing(ZZX& res, ZZX& poly, const long& cnst, const ZZ& mod, const long& deg);
		static void rightShiftPolyRing(ZZX& res, ZZX& poly, const long& cnst, const ZZ& mod, const long& deg);
		static void sampleGaussian(ZZX& res, long deg, double stdev);
		static void sampleUniform(ZZX& res, ZZ& B, long deg);
		static long mobius(long n);
		static ZZX Cyclotomic(long N);

		static void addPolyRing2(ZZX& res, ZZX& poly1, ZZX& poly2, const long& logMod, const long& deg);
		static void subPolyRing2(ZZX& res, ZZX& poly1, ZZX& poly2, const long& logMod, const long& deg);
		static void mulPolyRing2(ZZX& res, ZZX& poly1, ZZX& poly2, const long& logMod, const long& deg);
		static void mulPolyByConstantRing2(ZZX& res, ZZX& poly, const ZZ& cnst, const long& logMod, const long& deg);
		static void leftShiftPolyRing2(ZZX& res, ZZX& poly, const long& cnst, const long& logMod, const long& deg);
		static void rightShiftPolyRing2(ZZX& res, ZZX& poly, const long& cnst, const long& logMod, const long& deg);
		static void sampleUniform2(ZZX& res, long& logB, long deg);

		static void truncate(ZZ& c, const long& logMod);
};

#endif /* POLYSCHEME_ZRINGUTILS_H_ */
