#ifndef HEAAN_SERIALIZATIONUTILS_H_
#define HEAAN_SERIALIZATIONUTILS_H_

#include <iostream>

#include "Ciphertext.h"
#include "Plaintext.h"
#include "Scheme.h"
#include "SecretKey.h"
#include "Context.h"

using namespace std;
using namespace NTL;

class SerializationUtils {
public:

	static void writeCiphertext(Ciphertext& ciphertext, string path);
	static Ciphertext readCiphertext(string path);

	static void writePlaintext(Plaintext& plaintext, string path);
	static Plaintext readPlaintext(string path);

	static void writeContext(Context& context, string path);
	static Context readContext(string path);

	static void writeSchemeKeys(Scheme& scheme, string path);
	static void readSchemeKeys(Scheme& scheme, string path);

	static void writeSecretKey(SecretKey& secretKey, string path);
	static SecretKey readSecretKey(string path);

	static void writeKey(Key& key, string path);
	static Key readKey(string path);
};

#endif /* SERIALIZATIONUTILS_H_ */
