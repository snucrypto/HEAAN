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

	ZZX mx;

	long logSlots;
	long level;

	//-----------------------------------------

	Message(ZZX& mx, long& logSlots, long level = 1) : mx(mx), logSlots(logSlots), level(level) {}

};

#endif /* SCHEME_MESSAGE_H_ */
