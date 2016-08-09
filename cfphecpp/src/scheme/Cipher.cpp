#include <iostream>
#include <sstream>
#include <string>
#include "Cipher.h"


string PolyCipher::toString() {
	stringstream ss;

	ss << "Cipher: c0 = ";
	ss << c0;
	ss << ", c1 = ";
	ss << c1;
	ss << ", level = ";
	ss << level;
	return ss.str();
}

