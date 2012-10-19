#ifndef FS_LOADER_H_INCLUDED
#define FS_LOADER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string>
#include <vector>
#include <iostream>
#include "profile_loader.h"

// A unit of data in the FSProfileLoader format
struct FSProfileUnit {
	unsigned int time_in_ms;
	char from_key;
	char to_key;
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
