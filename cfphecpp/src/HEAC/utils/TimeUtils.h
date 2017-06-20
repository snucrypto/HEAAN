#ifndef POLYSCHEME_TIMEUTILS_H_
#define POLYSCHEME_TIMEUTILS_H_

#include <string>
#include <vector>
#include <sys/time.h>

struct timeval;

using namespace std;

class TimeUtils {
public:

	struct timeval startTime, stopTime;
	double timeElapsed;

	//-----------------------------------------

	TimeUtils();

	//-----------------------------------------

	void start(string msg);
	void stop(string msg);

	//-----------------------------------------
};

#endif /* POLYSCHEME_TIMEUTILS_H_ */
