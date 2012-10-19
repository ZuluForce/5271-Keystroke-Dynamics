#ifndef PROFILE_ENFORCER_H_INCLUDED
#define PROFILE_ENFORCER_H_INCLUDED

#include <map>
#include <vector>
#include <initializer_list>
#include "profile_loader.h"

class DittoProfileEnforcer {
private:

public:
	static std::map<unsigned int, char> scancodeToChar;

	virtual void enforce(KDProfile&) = 0;

};

#endif // PROFILE_ENFORCER_H_INCLUDED
