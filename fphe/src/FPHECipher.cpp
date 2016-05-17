#include "FPHECipher.h"

#include <iostream>
#include <sstream>
#include <string>


string FPHECipher::toString() {
	stringstream ss;

	ss << "Cipher: c0 = ";
	ss << c0;
	ss << ", c1 = ";
	ss << c1;
	ss << ", level = ";
	ss << level;
	return ss.str();
}

