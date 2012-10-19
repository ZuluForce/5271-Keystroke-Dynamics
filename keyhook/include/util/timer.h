#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>
#include <string>
#include <vector>

typedef std::chrono::high_resolution_clock ChronoClock;

class ChronoStopwatch {
private:
	ChronoClock clock;
	long startTime,endTime,lastLap;

	std::vector<long> laps;

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
	long stop();

	/**
	 * While the timer is still running check the current time. This is measured
	 * from the time start() was called.
	 */
	long checkTime();

	long takeLap();

	/**
	 * Request the time for a particular lap. Zero indexed.
	 */
	long getLapTime(int lapNumber);

	int LapCount();
};

class ChronoSWException {
private:
	std::string msg;

public:
	ChronoSWException(std::string);

	std::string getMessage();
};

#endif // TIMER_H_INCLUDED
