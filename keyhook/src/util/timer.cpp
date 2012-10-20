#include "util/timer.h"

ChronoStopwatch::ChronoStopwatch() {
	precision = (double) high_resolution_clock::period::num / high_resolution_clock::period::den;
	return;
}

microseconds ChronoStopwatch::getDuration(ChronoClockPoint start, ChronoClockPoint end) {
	auto duration = duration_cast<microseconds>(end - start);
	return duration;
}

void ChronoStopwatch::reset() {
	//startTime = endTime = -1;
	running = false;
}

void ChronoStopwatch::start() {
	if (running) {
		std::cerr << "Tried to start ChronoClock while already running" << std::endl;
		return;
	}

	startTime = ChronoClock::now();
	lastLap = startTime;

	running = true;
}

int64_t ChronoStopwatch::stop() {
	if (!running)
		return -1;

	running = false;

	endTime = ChronoClock::now();
	return this->getDuration(startTime, endTime).count();
}

int64_t ChronoStopwatch::checkTime() {
	if (!running)
		return -1;

	ChronoClock::time_point timeNow = ChronoClock::now();
	return this->getDuration(startTime, timeNow).count();
}

int64_t ChronoStopwatch::lap() {
	if (!running)
		return -1;

	ChronoClockPoint timeNow = ChronoClock::now();
	int64_t laptime = this->getDuration(lastLap, timeNow).count();

	lastLap = timeNow;

	return laptime;
}


ChronoSWException::ChronoSWException(std::string msg) {
	this->msg = msg;
}

std::string ChronoSWException::getMessage() {
	return this->msg;
}
