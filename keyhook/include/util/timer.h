#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <boost/chrono.hpp>

using namespace std::chrono;

typedef std::chrono::milliseconds ChronoMilliDuration;
typedef std::chrono::microseconds ChronoMicroDuration;

typedef high_resolution_clock ChronoClock;
typedef ChronoClock::time_point ChronoClockPoint;

typedef boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::microseconds> ChronoBoostPoint;

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
	ChronoMicroDuration checkTime();

	/**
	 * Basic lap functionality like a real stopwatch. Gives you the time difference since
	 * the last time you called lap or start if it is the first time you have called it since
	 * the last start.
	 */
	int64_t lap();

	ChronoMicroDuration lapDuration();

	/**
	 * What was the total time (from startTime) of the lastLap?
	 */
	ChronoClockPoint lastLapTotalTime();


	/* --- Static Methods --- */
	static ChronoMicroDuration durationFromMicro(int64_t);
	static ChronoClockPoint getFutureTimePoint(ChronoClockPoint&, ChronoMicroDuration& );

	// Result: d1 - d2
	static ChronoMicroDuration getDurationDifference(ChronoMicroDuration&,ChronoMicroDuration&);

	static ChronoBoostPoint toBoostTimePoint(ChronoClockPoint&);
};

class ChronoSWException {
private:
	std::string msg;

public:
	ChronoSWException(std::string);

	std::string getMessage();
};

#endif // TIMER_H_INCLUDED
