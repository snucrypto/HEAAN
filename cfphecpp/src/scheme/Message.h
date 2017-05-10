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

	Message(vector<CZZ> msgs, ZZ nu);
	Message(CZZ m, ZZ nu);
	Message(CZZ m, long logSlots, ZZ nu);

};

#endif /* SCHEME_MESSAGE_H_ */
