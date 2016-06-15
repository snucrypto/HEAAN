#ifndef FPHEFFTPUBKEY_H_
#define FPHEFFTPUBKEY_H_

#include <NTL/ZZX.h>
#include <vector>

#include "FFTParams.h"
#include "FFTSecKey.h"

using namespace std;
using namespace NTL;


class FFTPubKey {
public:
	vector<  vector<ZZ>  > A0fft;
	vector<  vector<ZZ>  > A1fft;

	vector<ZZ> c1Starfft;
	vector<ZZ> c0Starfft;

	FFTPubKey(FFTParams& params, FFTSecKey& secretKey);
	virtual ~FFTPubKey();
};

#endif /* FPHEFFTPUBKEY_H_ */
