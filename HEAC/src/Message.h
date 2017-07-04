#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Message {
public:

	ZZX mx; ///< message mod X^N + 1

	long doubleslots; ///< number of slots
	long level; ///< level of message

	//-----------------------------------------

	/**
	 * Message: mx
	 * @param[in] slots: number of slots
	 * @param[in] level: message level
	 */
	Message(ZZX mx = ZZX::zero(), long doubleslots = 2, long level = 1) : mx(mx), doubleslots(doubleslots), level(level) {}

	//-----------------------------------------

	string toString();
};

#endif /* SCHEME_MESSAGE_H_ */
