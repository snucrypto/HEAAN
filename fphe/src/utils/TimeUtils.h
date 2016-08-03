#ifndef POLYSCHEME_TIMEUTILS_H_
#define POLYSCHEME_TIMEUTILS_H_

#include <string>
#include <vector>

struct timeval;

using namespace std;

class TimeUtils {
private:
	struct timeval startTime, stopTime;
	double timeElapsed;
    vector<double> expData;

public:
    TimeUtils();
    void start(string msg);
    void stop(string msg);
};

#endif /* POLYSCHEME_TIMEUTILS_H_ */
