#include "Cipher.h"

#include <sstream>
#include <string>

string Cipher::toString() {
	stringstream ss;
	ss << "Cipher: [slots = ";
	ss << slots;
	ss << " , level = ";
	ss << level;
	ss << " , [ax[0] = ";
	ss << ax.rep[0];
	ss << " , bx[0] = ";
	ss << bx.rep[0];
	ss << " ] ]";
	return ss.str();
}
