#ifndef FS_LOADER_H_INCLUDED
#define FS_LOADER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string>
#include <vector>
#include <iostream>
#include "profile_loader.h"
#include "profile_enforcer.h" // To get the ScanCode enum

#define MICRO_IN_MILLI 1000

// A unit of data in the FSProfileLoader format.

// When the time_type is PRESS_TIME there is only one key involved
// so to be safe you can make the from/to_key the same but the current
// fs_loader will take the from_key.

enum FSProfileTimeType {
	FLY_TIME,
	PRESS_TIME,
};

struct FSProfileUnit {
	FSProfileTimeType time_type;
	unsigned int time_in_ms;
	ScanCode from_key;
	ScanCode to_key;
};

class FSProfileLoader: public DittoProfileLoader {
public:
	FSProfileLoader(std::string);

	int loadProfile(KDProfile&);
	int loadProfile(KDProfile&,KDProfileID&);

	std::vector<std::string>* enumerateAvailableProfiles();

	std::string getProfileLoaderName();
};

#endif // FS_LOADER_H_INCLUDED
