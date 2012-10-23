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

ChronoMicroDuration ChronoStopwatch::checkTime() {
	if (!running)
		return ChronoMicroDuration(-1);

	ChronoClock::time_point timeNow = ChronoClock::now();
	return this->getDuration(startTime, timeNow);
}

int64_t ChronoStopwatch::lap() {
	if (!running)
		return -1;

	ChronoClockPoint timeNow = ChronoClock::now();
	int64_t laptime = this->getDuration(lastLap, timeNow).count();

	lastLap = timeNow;

	return laptime;
}

ChronoMicroDuration ChronoStopwatch::lapDuration() {
	if (!running)
		throw new ChronoSWException("Cannot take lap when sw isn't running");

	ChronoClockPoint timeNow = ChronoClock::now();
	microseconds lapD = this->getDuration(lastLap, timeNow);
	lastLap = timeNow;

	return lapD;
}

ChronoClockPoint ChronoStopwatch::lastLapTotalTime() {
	return lastLap;
}

/* --- Static ChronoStopwatch Methods --- */
ChronoMicroDuration ChronoStopwatch::durationFromMicro(int64_t t) {
	return ChronoMicroDuration(t);
}

ChronoClockPoint ChronoStopwatch::getFutureTimePoint(ChronoClockPoint& point, ChronoMicroDuration& duration) {
	return point + duration;
}

ChronoMicroDuration ChronoStopwatch::getDurationDifference(ChronoMicroDuration& d1,ChronoMicroDuration& d2) {
	return duration_cast<ChronoMicroDuration>(d1 - d2);
}

ChronoBoostPoint ChronoStopwatch::toBoostTimePoint(ChronoClockPoint& point) {
	ChronoBoostPoint t(boost::chrono::microseconds(point.time_since_epoch().count()));

	return t;
}


ChronoSWException::ChronoSWException(std::string msg) {
	this->msg = msg;
}

std::string ChronoSWException::getMessage() {
	return this->msg;
}
