#include "Message.h"

#include <sstream>
#include <string>

string Message::toString() {
	stringstream ss;
	ss << "Message: [slots = ";
	ss << slots;
	ss << " , level = ";
	ss << level;
	ss << " , [mx[0] = ";
	ss << mx.rep[0];
	ss << " ] ]";
	return ss.str();
}
