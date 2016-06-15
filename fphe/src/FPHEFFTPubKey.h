#ifndef FPHEFFTPUBKEY_H_
#define FPHEFFTPUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "FPHEFFTParams.h"
#include "FPHEFFTSecKey.h"

using namespace std;
using namespace NTL;


class FPHEFFTPubKey {
public:
	vector<  vector<ZZ>  > A0fft;
	vector<  vector<ZZ>  > A1fft;

	vector<ZZ> c1Starfft;
	vector<ZZ> c0Starfft;

	FPHEFFTPubKey(FPHEFFTParams& params, FPHEFFTSecKey& secretKey);
	virtual ~FPHEFFTPubKey();
};

#endif /* FPHEFFTPUBKEY_H_ */
