#ifndef HEAAN_TIMEUTILS_H_
#define HEAAN_TIMEUTILS_H_

#include <iostream>

struct timeval;

using namespace std;

class TimeUtils {
public:

	struct timeval startTime, stopTime;
	double timeElapsed;

	TimeUtils();

	/**
	 * starts timer
	 * @param[in] msg: string message
	 */
	void start(string msg);

	/**
	 * stops timer and prints time elapsed in console
	 * @param[in] msg: string message
	 */
	void stop(string msg);

};

#endif
