#ifndef PROFILE_LOADER_H_INCLUDED
#define PROFILE_LOADER_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>

#define MAX_CHAR 127

// Since ditto will likely have to support loading profiles from multiple
// places I am creating an abstract base class as an interface

typedef std::vector<std::vector<int> > KDProfile;
void setKDProfileSize(KDProfile& profile, size_t size=127);
void printKDProfile(KDProfile& profile);

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
