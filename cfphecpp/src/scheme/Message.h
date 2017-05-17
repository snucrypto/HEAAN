#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZ.h>
#include <cmath>
#include <vector>

#include "../czz/CZZ.h"
#include "../utils/NumUtils.h"

using namespace std;
using namespace NTL;

class Message {
public:
	ZZX poly;
	long logSlots;
	long level;

	//-----------------------------------------

	Message(ZZX& poly, long& logSlots, long& level) : poly(poly), logSlots(logSlots), level(level) {}
	Message(ZZX& poly, long& logSlots) : poly(poly), logSlots(logSlots) {level = 1;}

};

#endif /* SCHEME_MESSAGE_H_ */
