/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_SERIALIZATIONUTILS_H_
#define HEAAN_SERIALIZATIONUTILS_H_

#include <iostream>
#include <string>

#include "Ciphertext.h"
#include "Params.h"
#include "Key.h"

namespace heaan {

class SerializationUtils {
public:

	static void writeCiphertext(Ciphertext& ciphertext, std::string path);
	static Ciphertext* readCiphertext(std::string path);

	static void writeKey(Key* key, std::string path);
	static Key* readKey(std::string path);
};

}  // namespace heaan

#endif /* SERIALIZATIONUTILS_H_ */
