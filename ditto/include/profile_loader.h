#ifndef PROFILE_LOADER_H_INCLUDED
#define PROFILE_LOADER_H_INCLUDED

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include "util/timer.h"

/*
 * This offset will be subtracted from every time. This is due to a combination
 * of the innacuracy of the incoming profiles (a tendency to be too high) and
 * some of the vaiability in ditto
 */
#define UNIFORM_TIME_OFFSET 0  // in ms

// Since ditto will likely have to support loading profiles from multiple
// places I am creating an abstract base class as an interface

typedef uint32_t KDProfileKey;

class KDProfile {
private:
	/* A matrix of fly times where the row specifies the 'from' key,
	 * the column is the 'to' key and the cell holds the time in
	 * microseconds.
	 */
	std::vector<std::vector<ChronoMicroDuration> > fly_times;
	std::vector<ChronoMicroDuration> press_times;

	/* This will ensure that the square fly_times matrix has minumum dimesions
	 * equal to the max of width and height.
	 */
	void ensureMatrixSize(uint32_t width, uint32_t height);
	void ensurePressVectorSize(uint32_t size);

	int64_t fastest_flight;
	uint32_t fastest_from, fastest_to;

	int64_t num_flytimes, total_flytimes;
	int64_t num_presstimes, total_presstimes;

public:
	KDProfile(uint32_t matrixSize=127);
	void addFlyTime(uint32_t from, uint32_t to, int64_t ftime);
	void addPressTime(uint32_t key, int64_t ptime);

	std::pair<uint32_t,uint32_t> getFastestFlight();

	ChronoMicroDuration getFlyTime(uint32_t from, uint32_t to);
	ChronoMicroDuration getPressTime(uint32_t key);

	int64_t getAverageFlyTimeMs();
	int64_t getAveragePressTimeMs();

	bool canLoosenKey(KDProfileKey key);
};

class KDProfileID {
private:
    std::string profileName;
public:
	KDProfileID(std::string name);
    std::string getProfileName();
};

class DittoProfileLoader {
protected:
	std::string profileData;

public:
	DittoProfileLoader(std::string profileData);

	virtual int loadProfile(KDProfile&) = 0;
    virtual int loadProfile(KDProfile&,KDProfileID&) = 0;
	virtual std::vector<std::string>* enumerateAvailableProfiles() = 0;

	virtual std::string getProfileLoaderName() = 0;
};

#endif // PROFILE_LOADER_H_INCLUDED
