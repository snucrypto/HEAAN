#ifndef POLYSCHEME_POLYPARAMS_H_
#define POLYSCHEME_POLYPARAMS_H_

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <vector>

using namespace std;
using namespace NTL;

class PolyParams {
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
	long Pbits;
	ZZ qL;
	ZZ Pq;

	vector<ZZ> qi;
	vector<ZZ> Pqi;
	PolyParams(long lambda, bool isGauss = false);
	virtual ~PolyParams();

	ZZ& getModulo(long level);
	string toString();
};

#endif /* POLYSCHEME_POLYPARAMS_H_ */
