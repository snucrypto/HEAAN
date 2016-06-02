#ifndef FPHECIPHER_H_
#define FPHECIPHER_H_

#include <NTL/ZZ.h>
#include <string>
#include <vector>

using namespace std;
using namespace NTL;

class FPHECipher {
public:
	vector<ZZ> c0fft;
	vector<ZZ> c1fft;
	long level;

	FPHECipher(vector<ZZ> c0fft, vector<ZZ> c1fft, long level) : c0fft(c0fft), c1fft(c1fft), level(level) {}
	virtual ~FPHECipher() {}

	string toString();
};

#endif /* FPHECIPHER_H_ */
