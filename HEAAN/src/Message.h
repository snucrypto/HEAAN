#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

class Message {
public:

	ZZX mx; ///< message mod X^N + 1

	ZZ mod;
	long cbits; ///< bits in cipher
	long slots; ///< number of slots

	//-----------------------------------------

	/**
	 * Message: mx
	 * @param[in] polynomial mx
	 * @param[in] bits: bits in cipher
	 * @param[in] slots: number of slots
	 */
	Message(ZZX mx = ZZX::zero(), ZZ mod = ZZ::zero(), long cbits = 0, long slots = 1) : mx(mx), mod(mod), cbits(cbits), slots(slots) {}

	Message(const Message& o) : mx(o.mx), mod(o.mod), cbits(o.cbits), slots(o.slots) {}
};

#endif /* SCHEME_MESSAGE_H_ */
