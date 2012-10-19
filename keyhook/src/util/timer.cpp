#include "util/timer.h"

ChronoStopwatch::ChronoStopwatch() {
	return;
}

void ChronoStopwatch::reset() {
	startTime = endTime = lastLap = -1;

	this.laps.resize(0);
}

void ChronoStopwatch::start() {
	startTime = this->clock::now();
	lastLap = startTime;
}

long ChronoStopwatch::stop() {
	if (startTime < 0)
		throw new ChronoSWException("Stopwatch not started. Cannot stop");

	endTime = this->clock::now();

	return endTime - startTime;
}

long ChronoStopwatch::checkTime() {
	if (startTime < 0)
		throw new ChronoSWException("Stopwatch not started. Cannot checkTime()");

	long t = this->clock::now();
	return (t-startTime);
}

long ChronoStopwatch::takeLap() {
	if (startTime < 0 || laptime < 0)
		throw new ChronoSWException("Stopwatch not started. Cannot takeLap()");

	long now, laptime;
	now = this->clock::now();
	laptime = lastLap;

	this->laps.push_back(laptime);
}

long ChronoStopwatch::getLapTime(int lapNumber) {
	if (lapNumber >= this->laps.size())
		throw new ChronoSWException("Out of range lap number: " + lapNumber);

	return this->laps.at(lapNumber);
}

int ChronoStopwatch::LapCount() {
	return this->laps.size();
}


ChronoSWException::ChronoSWException(std::string msg) {
	this->msg = msg;
}

std::string ChronoSWException::getMessage() {
	return this->msg;
}
