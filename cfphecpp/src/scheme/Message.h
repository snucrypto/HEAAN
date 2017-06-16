#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Message {
public:

	ZZX mx; ///< message mod X^N + 1

	long slots; ///< number of slots
	long level; ///< level of message

	//-----------------------------------------

	/**
	 * Message: mx
	 * @param[in] slots: batch number of slots
	 * @param[in] level: message level
	 */
	Message(ZZX mx = ZZX::zero(), long slots = 1, long level = 1) : mx(mx), slots(slots), level(level) {}

	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_MESSAGE_H_ */
