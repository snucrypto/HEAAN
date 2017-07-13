#include "Cipher.h"

#include <sstream>
#include <string>

string Cipher::toString() {
	stringstream ss;
	ss << "Cipher: [slots = " << slots;
	ss << " , level = " << level;
	ss << " , [ax[0] = " << ax.rep[0] << ", ...]";
	ss << " , [bx[0] = " << bx.rep[0] << ", ...]";
	ss << " ]";
	return ss.str();
}
