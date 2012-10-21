#ifndef KNOWN_PLOADERS_H_INCLUDED
#define KNOWN_PLOADERS_H_INCLUDED

#include <string>
#include <string>
#include "profile_loader.h"

// Include headers for any loaders we know of
#include "loaders/fs_loader.h"

DittoProfileLoader* getProfileLoader(std::string name, std::string data);

#endif // KNOWN_PLOADERS_H_INCLUDED
