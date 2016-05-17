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
	vector<ZZ> qi;
	vector<long> logQi;
	ZZ qL;
	long logQL;
	ZZX phi;
	FPHEParams(long lambda, bool isGauss = false);
	virtual ~FPHEParams();

	ZZ& getModulo(long level);
	string toString();
};

#endif /* FPHEPARAMS_H_ */
