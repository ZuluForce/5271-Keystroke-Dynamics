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
	ChronoMicroDuration wait_time;
};

struct KeyDownHist {
	// When the key down event will be sent
	ChronoClockPoint kd_dispatch_time;
};

class BasicProfileEnforcer: public DittoProfileEnforcer {
private:
	// This is used to account for clock drift. This becomes more
	// important when fly times get smaller
	static const ChronoMicroDuration ditto_overhead;

	boost::mutex dq_mutex, du_mutex, dd_mutex;
	boost::mutex interception_lock;

	/* dq_Sem - dispatchQueue semaphore
	 * du_sem - dispatchUp queue semaphore
	 * dd_sem - dispatchDown queue semaphore
	 */
	semaphore dq_sem, du_sem, dd_sem;
	std::queue<PendingStroke*> dispatchQueue;
	KDProfile* profileRef;

	// For the dispatchUp and dispatchDown threads repectively
	std::queue<PendingStroke*> dispatchUpQueue;
	std::queue<PendingStroke*> dispatchDownQueue;

	// This is necessary for the dispathReceiver to calcualte times
	// for the appropriate key up events
	std::map<int, KeyDownHist> dispatchDownHist;

	void dispatchUp();
	void dispatchDown();
	void dispatchReceiver();
	void addToDispatch(PendingStroke*);
	PendingStroke* getFromDispatch();

	// Generic methods for any queue. Could get rid of the other dispatchQueue methods
	void addToQueue(std::queue<PendingStroke*>&,PendingStroke*,boost::mutex&,semaphore&);
	PendingStroke* getFromQueue(std::queue<PendingStroke*>&, boost::mutex&, semaphore&);

	// Some shared state for the capture and dispatch
	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke kstroke;

public:
	BasicProfileEnforcer();

	void enforce(KDProfile&);
};

#endif // BASIC_ENFORCER_H_INCLUDED
