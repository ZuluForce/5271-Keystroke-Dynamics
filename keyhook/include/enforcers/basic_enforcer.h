#ifndef BASIC_ENFORCER_H_INCLUDED
#define BASIC_ENFORCER_H_INCLUDED

#include <queue>
#include <chrono>
#include "utils.h"
#include "interception.h"
#include "profile_loader.h"
#include "profile_enforcer.h"
#include "util/timer.h"

struct PendingStroke {
	InterceptionKeyStroke stroke;
	int delay;
};

class BasicProfileEnforcer: public DittoProfileEnforcer {
private:
	std::queue<PendingStroke> strokeQueue;

	void dispatcher();

public:
	BasicProfileEnforcer();

	void enforce(KDProfile&);
};

#endif // BASIC_ENFORCER_H_INCLUDED
