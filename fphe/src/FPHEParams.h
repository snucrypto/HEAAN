#ifndef FPHEPARAMS_H_
#define FPHEPARAMS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

using namespace std;
using namespace NTL;

class FPHEParams {
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

	FPHEParams(long lambda, bool isGauss = false);
	virtual ~FPHEParams();

	ZZ& getModulo(long level);
	string toString();
};

#endif /* FPHEPARAMS_H_ */
