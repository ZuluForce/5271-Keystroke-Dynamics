#ifndef BASIC_ENFORCER_H_INCLUDED
#define BASIC_ENFORCER_H_INCLUDED

#include "utils.h"
#include "interception.h"
#include "profile_loader.h"
#include "profile_enforcer.h"


class BasicProfileEnforcer: public DittoProfileEnforcer {
public:
	BasicProfileEnforcer();

	void enforce(KDProfile&);
};

#endif // BASIC_ENFORCER_H_INCLUDED
