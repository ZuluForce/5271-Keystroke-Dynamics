#include "profile_loader.h"


KDProfile::KDProfile(uint32_t matrixSize) {

	this->ensureMatrixSize(matrixSize, matrixSize);
	this->ensurePressVectorSize(matrixSize);

	fastest_flight = fastest_to = fastest_from = -1;

	num_flytimes = total_flytimes = 0;
	num_presstimes = total_presstimes = 0;
}

void KDProfile::ensureMatrixSize(uint32_t width, uint32_t height) {
	uint32_t max = width > height ? width : height;

	if (max > fly_times.size())
		fly_times.resize(max);
	if (max > fly_times.at(0).size()) {
		for (uint32_t i = 0; i < max; ++i) {
			fly_times.at(i).resize(max);
		}
	}
}

void KDProfile::ensurePressVectorSize(uint32_t size) {
	if (size > press_times.size())
		press_times.resize(size);
}

void KDProfile::addFlyTime(uint32_t from, uint32_t to, int64_t ftime) {
	ensureMatrixSize(from, to);

	if (ftime < fastest_flight) {
		fastest_from = from;
		fastest_to = to;
	}

	ftime -= UNIFORM_TIME_OFFSET;

    ++num_flytimes;
	total_flytimes += ftime;

	ChronoMicroDuration duration = ChronoStopwatch::durationFromMicro(ftime);
	fly_times.at(from).at(to) = duration;
}

void KDProfile::addPressTime(uint32_t key, int64_t ptime) {
	this->ensurePressVectorSize(key);

    ptime -= UNIFORM_TIME_OFFSET;

    ++num_presstimes;
    total_presstimes += ptime;

	ChronoMicroDuration duration = ChronoStopwatch::durationFromMicro(ptime);
	press_times.at(key) = duration;
}


std::pair<uint32_t,uint32_t> KDProfile::getFastestFlight() {
	return std::pair<uint32_t,uint32_t>(fastest_from, fastest_to);
}

ChronoMicroDuration KDProfile::getFlyTime(uint32_t from, uint32_t to) {
	return fly_times.at(from).at(to);
}

ChronoMicroDuration KDProfile::getPressTime(uint32_t key) {
	return press_times.at(key);
}

int64_t KDProfile::getAverageFlyTimeMs() {
    return (total_flytimes / num_flytimes) / 1000;
}

int64_t KDProfile::getAveragePressTimeMs() {
    return (total_presstimes / num_presstimes) / 1000;
}

bool KDProfile::canLoosenKey(KDProfileKey key) {
    // For right now we only make the shift key bound to strict
    // ordering. Later this could include ctrl, alt or others
    return key != 0x2A;
}

KDProfileID::KDProfileID(std::string name)
: profileName(name) {
	return;
}

std::string KDProfileID::getProfileName() {
	return profileName;
}

DittoProfileLoader::DittoProfileLoader(std::string data) {
	this->profileData = data;
}
