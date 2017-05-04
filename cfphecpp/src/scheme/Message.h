#ifndef SCHEME_MESSAGE_H_
#define SCHEME_MESSAGE_H_

#include <NTL/ZZ.h>
#include <cmath>
#include <vector>

#include "../czz/CZZ.h"
#include "../utils/NumUtils.h"

using namespace std;
using namespace NTL;

class Message {
public:
	vector<CZZ> vals;
	long logSlots;
	ZZ nu;

	Message(vector<CZZ> msgs, ZZ nu) : vals(msgs), nu(nu) {
		logSlots = log2(vals.size() + 1) + 1;
	};

	Message(CZZ m, ZZ nu) : nu(nu) {
		vals.push_back(m);
		logSlots = 2;
	};

};

#endif /* SCHEME_MESSAGE_H_ */
