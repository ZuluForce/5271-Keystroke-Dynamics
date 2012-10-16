#ifndef PROFILE_LOADER_H_INCLUDED
#define PROFILE_LOADER_H_INCLUDED

#include <vector>
#include <string>

// Since ditto likely will have to support loading profiles from multiple
// places I am creating an abstract base class as an interface

typedef std::vector<std::vector<int> > KDProfile;


class KDProfileID {
private:
    std::string profileName;

public:
    virtual std::string getProfileName() = 0;

};

class DittoProfileLoader {
private:
	std::string profileData;

public:
	DittoProfileLoader(std::string profileData);

    virtual int loadProfile(KDProfile&,KDProfileID&) = 0;
	virtual std::vector<std::string>* enumerateAvailableProfiles() = 0;

	virtual std::string getProfileLoaderName() = 0;
};

#endif // PROFILE_LOADER_H_INCLUDED
