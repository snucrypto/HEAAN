/*
 * TimeUtils.h
 *
 *  Created on: May 17, 2016
 *      Author: kimandrik
 */

#ifndef TIMEUTILS_H_
#define TIMEUTILS_H_

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

#endif /* TIMEUTILS_H_ */
