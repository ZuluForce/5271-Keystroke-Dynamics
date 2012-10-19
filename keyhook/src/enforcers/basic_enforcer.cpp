#include "enforcers/basic_enforcer.h"


BasicProfileEnforcer::BasicProfileEnforcer() {
	return;
}

void BasicProfileEnforcer::dispatcher() {
	ChronoStopwatch watch = ChronoStopwatch();
	PendingStroke* stroke = NULL;

	while(stroke = this->strokeQueue.pop()) {

	}
}


void BasicProfileEnforcer::enforce(KDProfile& profile) {
	typedef std::chrono::high_resolution_clock Clock;
	auto t1 = Clock::now();
	auto t2 = Clock::now();
	std::cout << (t2-t1).count() << " nanoseconds \n";

	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke stroke;

	raise_process_priority();

	context = interception_create_context();
	if (context == NULL) {
		std::cerr << "Failed to initialize interception context" << std::endl;
		return;
	}

	char nextKey, prevKey;
	int timing;
	std::map<unsigned int, char>::iterator stcIter, stcEnd;

	prevKey = '\0';
	stcEnd = DittoProfileEnforcer::scancodeToChar.end();

	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
	while (interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&stroke, 1) > 0) {
		// Lookup the scancode in the map
		stcIter = DittoProfileEnforcer::scancodeToChar.find(stroke.code);
		if (stcIter == stcEnd) {
			std::cerr << "Unmapped scancode: " << stroke.code << std::endl;

			// Since we don't have a mapping we can't look up delay times. Just
			// send the keystroke
			interception_send(context, device, (InterceptionStroke *)&stroke, 1);
			continue;
		}

		nextKey = stcIter->first;
		if (prevKey == '\0') {
			// This means it is the first key press so we don't have any time info to go
			// on so just send the stroke
			prevKey = nextKey;
			interception_send(context, device, (InterceptionStroke *)&stroke, 1);
			continue;
		}
		timing = profile.at(prevKey).at(nextKey);
	}
}
