#ifndef FS_LOADER_H_INCLUDED
#define FS_LOADER_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include "profile_loader.h"

class FSProfileLoader: public DittoProfileLoader {
public:
	FSProfileLoader(std::string data);

	int loadProfile(KDProfile&,KDProfileID&);

	std::vector<std::string>* enumerateAvailableProfiles();

	std::string getProfileLoaderName();
};

#endif // FS_LOADER_H_INCLUDED
