#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>

using namespace std::chrono;

typedef high_resolution_clock ChronoClock;
typedef high_resolution_clock::time_point ChronoClockPoint;

class ChronoStopwatch {
private:
	ChronoClock clock;
	ChronoClockPoint startTime,endTime, lastLap;

	microseconds getDuration(ChronoClockPoint start, ChronoClockPoint end);

	bool running;

	double precision;

public:
	ChronoStopwatch();

	/**
	 * Reset the stopwatch. Clears any saved times.
	 */
	void reset();

	/**
	 * Start the timer.
	 */
	void start();

	/**
	 * Stop the timer and give the elapsed time. This method can be called
	 * again to get the same elapsed time. If the stopwatch was not started
	 * before this is called, a ChronoSWException will be thrown.
	 */
	int64_t stop();

	/**
	 * While the timer is still running check the current time. This is measured
	 * from the time start() was called.
	 */
	int64_t checkTime();

	/**
	 * Basic lap functionality like a real stopwatch. Gives you the time difference since
	 * the last time you called lap or start if it is the first time you have called it since
	 * the last start.
	 */
	int64_t lap();
};

class ChronoSWException {
private:
	std::string msg;

public:
	ChronoSWException(std::string);

	std::string getMessage();
};

#endif // TIMER_H_INCLUDED
