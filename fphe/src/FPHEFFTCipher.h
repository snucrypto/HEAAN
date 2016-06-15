#ifndef FPHEFFTCIPHER_H_
#define FPHEFFTCIPHER_H_

#include <NTL/ZZ.h>
#include <string>
#include <vector>

using namespace std;
using namespace NTL;

class FPHEFFTCipher {
public:
	vector<ZZ> c0fft;
	vector<ZZ> c1fft;
	long level;

	FPHEFFTCipher(vector<ZZ> c0fft, vector<ZZ> c1fft, long level) : c0fft(c0fft), c1fft(c1fft), level(level) {}
	virtual ~FPHEFFTCipher() {}

	string toString();
};

#endif /* FPHEFFTCIPHER_H_ */
