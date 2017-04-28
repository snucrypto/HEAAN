#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZ.h>
#include <cmath>
#include <vector>
#include "../czz/CZZ.h"

using namespace std;
using namespace NTL;

class Message {
public:
	vector<CZZ> vals;
	long logSlots;
	ZZ nu;

	Message(vector<CZZ> vals, ZZ nu, long logSlots) : vals(vals) , nu(nu), logSlots(logSlots) {};

	Message(CZZ m, ZZ nu, long logSlots) : nu(nu), logSlots(logSlots) {
		vals.clear();
		vals.push_back(m);
	};

	Message(CZZ m, ZZ nu) : nu(nu) {
		vals.clear();
		vals.push_back(m);
		this->logSlots = log2(vals.size()) + 1;

	};

	Message(vector<CZZ> vals, ZZ nu) : vals(vals) , nu(nu) {
		this->logSlots = log2(vals.size()) + 1;
	};
};

#endif /* SCHEME_MESSAGE_H_ */
