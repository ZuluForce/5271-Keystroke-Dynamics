#include "profile_loader.h"


void setKDProfileSize(KDProfile& profile, size_t size) {
	profile.resize(size);

	for (int i = 0; i < size; ++i) {
		profile.at(i).resize(size);
	}
}

void printKDProfile(KDProfile& profile) {
	for (int i = 0; i < profile.size(); ++i) {
		for (int j = 0; j < profile.at(i).size(); ++j) {
			std::cout << (char) i << " -> " << (char) j;
			std::cout << " : " << profile.at(i).at(j) << " ms" << std::endl;
		}
	}
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
