#include "Message.h"

Message::Message(vector<CZZ> msgs, ZZ nu) : vals(msgs), nu(nu) {
	logSlots = log2(vals.size() + 1) + 1;
};

Message::Message(CZZ m, ZZ nu) : nu(nu) {
	vals.push_back(m);
	logSlots = 2;
};

Message::Message(CZZ m, long logSlots, ZZ nu) : nu(nu), logSlots(logSlots) {
	vector<CZZ> mvec;
	long slots = (1 << logSlots) - 1;
	for (int i = 0; i < slots; ++i) {
		mvec.push_back(m);
	}
	vals = mvec;
}
