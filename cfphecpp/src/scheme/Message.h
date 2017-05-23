#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Message {
public:

	ZZX mx;

	long slots;
	long level;

	//-----------------------------------------

	Message(ZZX mx = ZZX::zero(), long slots = 1, long level = 1) : mx(mx), slots(slots), level(level) {}

};

#endif /* SCHEME_MESSAGE_H_ */
