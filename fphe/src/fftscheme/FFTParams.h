#ifndef FPHEFFTPARAMS_H_
#define FPHEFFTPARAMS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

using namespace std;
using namespace NTL;

class FFTParams {
public:
	long lambda;
	long levels;
	long tau;
	long m;
	long phim;

	bool isGauss;
	double stdev;
	ZZ B;
	ZZ p;
	ZZ qL;
	ZZ PqL;
	ZZ P;

	ZZ pRoot;
	ZZ qRoot;

	vector<  vector<ZZ>  > fft;
	vector<  vector<ZZ>  > fftInv;

	vector<ZZ> qi;
	vector<ZZ> Pqi;
//	ZZX phi;

	FFTParams(long lambda, bool isGauss = false);
	virtual ~FFTParams();

	ZZ& getModulo(long level);
	string toString();
};

#endif /* FPHEFFTPARAMS_H_ */
