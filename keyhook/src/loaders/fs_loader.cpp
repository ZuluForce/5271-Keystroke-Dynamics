#include "loaders/fs_loader.h"

FSProfileLoader::FSProfileLoader(std::string data)
: DittoProfileLoader(data) {
	return;
}

std::string FSProfileLoader::getProfileLoaderName() {
	return "FileSystem binary format profile loader";
}

std::vector<std::string>* FSProfileLoader::enumerateAvailableProfiles() {
	std::vector<std::string>* avail = new std::vector<std::string>();
	if (avail == NULL) return NULL;

	avail->push_back("Profile enumeration not supported");

	return avail;
}


int FSProfileLoader::loadProfile(KDProfile& profile, KDProfileID& id) {
	std::cout << "FSProfileLoader loading " << id.getProfileName() << std::endl;

	return 0;
}
