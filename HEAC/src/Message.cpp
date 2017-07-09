#include "Message.h"

#include <sstream>
#include <string>

string Message::toString() {
	stringstream ss;
	ss << "Message: [slots = " << slots;
	ss << " , level = " << level;
	ss << " , [mx[0] = " << mx.rep[0];
	ss << " ] ]";
	return ss.str();
}
