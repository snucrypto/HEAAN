#include "FFTCipher.h"

#include <iostream>
#include <sstream>
#include <string>


string FFTCipher::toString() {
	stringstream ss;

	ss << "Cipher: c0 = ";
	ss << c0fft.size();
	ss << ", c1 = ";
	ss << c1fft.size();
	ss << ", level = ";
	ss << level;
	return ss.str();
}

