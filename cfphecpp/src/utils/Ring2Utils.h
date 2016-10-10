#ifndef POLYSCHEME_ZRINGUTILS_H_
#define POLYSCHEME_ZRINGUTILS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include "../czz/CZZ.h"
#include "../czz/CZZX.h"

using namespace NTL;

class Ring2Utils {
	public:
		static void add(ZZX& res, ZZX& p1, ZZX& p2, const long& degree);
		static void add(CZZX& res, CZZX& p1, CZZX& p2, const long& degree);
		static void add(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void add(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree);

		static void sub(ZZX& res, ZZX& p1, ZZX& p2, const long& degree);
		static void sub(CZZX& res, CZZX& p1, CZZX& p2, const long& degree);
		static void sub(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void sub(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree);

		static void mult(ZZX& res, ZZX& p1, ZZX& p2, const long& degree);
		static void mult(CZZX& res, CZZX& p1, CZZX& p2, const long& degree);
		static void mult(ZZX& res, ZZX& p1, ZZX& p2, const long& logMod, const long& degree);
		static void mult(CZZX& res, CZZX& p1, CZZX& p2, const long& logMod, const long& degree);
		static void mult(CZZX& res, CZZX& p1, ZZX& p2, const long& logMod, const long& degree);

		static void mulCnst(ZZX& res, ZZX& p, const ZZ& cnst, const long& degree);
		static void mulCnst(CZZX& res, CZZX& p, const CZZ& cnst, const long& degree);
		static void mulCnst(ZZX& res, ZZX& p, const ZZ& cnst, const long& logMod, const long& degree);
		static void mulCnst(CZZX& res, CZZX& p, const CZZ& cnst, const long& logMod, const long& degree);

		static void leftShift(ZZX& res, ZZX& p, const long& cnst, const long& logMod, const long& degree);
		static void leftShift(CZZX& res, CZZX& p, const long& cnst, const long& logMod, const long& degree);

		static void rightShift(ZZX& res, ZZX& p, const long& cnst, const long& logMod, const long& degree);
		static void rightShift(CZZX& res, CZZX& p, const long& cnst, const long& logMod, const long& degree);

		static void truncate(ZZ& res, const long& logMod);
		static void truncate(CZZ& res, const long& logMod);
};

#endif /* POLYSCHEME_ZRINGUTILS_H_ */
