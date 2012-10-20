#ifndef BASIC_ENFORCER_H_INCLUDED
#define BASIC_ENFORCER_H_INCLUDED

#include "stdlib.h"
#include <queue>
#include <chrono>
#include <string.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "utils.h"
#include "interception.h"
#include "profile_loader.h"
#include "profile_enforcer.h"
#include "util/timer.h"
#include "util/semaphore.h"

struct PendingStroke {
	InterceptionKeyStroke stroke;
};

class BasicProfileEnforcer: public DittoProfileEnforcer {
private:
	boost::mutex dq_mutex;
	semaphore dq_sem;
	std::queue<PendingStroke*> dispatchQueue;
	KDProfile* profileRef;

	void dispatcher();
	void dispatchReceiver();
	void addToDispatch(PendingStroke*);
	PendingStroke* getFromDispatch();

	// Some shared state for the capture and dispatch
	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke kstroke;

public:
	BasicProfileEnforcer();

	void enforce(KDProfile&);
};

#endif // BASIC_ENFORCER_H_INCLUDED
