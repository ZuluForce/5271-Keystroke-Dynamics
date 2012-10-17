#ifndef PROFILE_ENFORCER_H_INCLUDED
#define PROFILE_ENFORCER_H_INCLUDED

#include "profile_loader.h"

class DittoProfileEnforcer {
private:

public:
	DittoProfileEnforcer(KDProfile&);

	virtual void enforce();

};

#endif // PROFILE_ENFORCER_H_INCLUDED
