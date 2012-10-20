#include "enforcers/basic_enforcer.h"


//PendingStrokePool::initialPoolSize = 10;
//PendingStrokePool::defaultMaxPool = 50;


BasicProfileEnforcer::BasicProfileEnforcer() {
	return;
}

void BasicProfileEnforcer::dispatchReceiver() {
	std::cout << "BasicProfileEnforcer dispatcher started" << std::endl;

	ChronoStopwatch watch = ChronoStopwatch();
	PendingStroke* pStroke = NULL;

	watch.start();
	int64_t laptime;
	while (true) {
		pStroke = this->getFromDispatch();
		laptime = watch.lap();
		std::cout << "Dispatcher recieved stroke: " << pStroke->stroke.code << std::endl;
		std::cout << "Time since last stroke: " << laptime << " microseconds" << std::endl;
		if (laptime < 0) {
			std::cerr << "The high precison timer has failed. Must exit." << std::endl;
			exit(-1);
		}

		interception_send(context, device, (InterceptionStroke *) &(pStroke->stroke), 1);

		free(pStroke);
	}
}


void BasicProfileEnforcer::enforce(KDProfile& profile) {
	std::cout << "BasicProfilerEnforcer starting" << std::endl;
	raise_process_priority();

	this->context = interception_create_context();
	if (context == NULL) {
		std::cerr << "Failed to initialize interception context" << std::endl;
		return;
	}

	PendingStroke* tempPStroke;

	// So the dispatch thread can access it
	profileRef = &profile;

	// Create dispatch thread
	boost::thread* dispatchThread =
		new boost::thread(boost::bind(&BasicProfileEnforcer::dispatchReceiver, this));

	// We want to catch ctrl+c so even if things break down the pipeline we can
	// still close it
	InterceptionKeyStroke ctrl_down = {0x1D, INTERCEPTION_KEY_DOWN};
	InterceptionKeyStroke ctrl_up = {0x1D, INTERCEPTION_KEY_UP};

	bool ctrl_pressed = false;

	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
	while (interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&kstroke, 1) > 0) {
		if (strokesEqual(&kstroke,&ctrl_down)) {
			ctrl_pressed = true;
		} else if (strokesEqual(&kstroke, &ctrl_up)) {
			ctrl_pressed = false;
		} else if (kstroke.code == SCANCODE_C) {
			if (ctrl_pressed) {
				std::cerr << "Ctrl+c detected. Exiting" << std::endl;
				exit(1);
			}
		}

		std::cout << "Received Stroke: " << kstroke.code << std::endl;
		tempPStroke = (PendingStroke*) malloc(sizeof(PendingStroke));

		tempPStroke->stroke = kstroke;

		this->addToDispatch(tempPStroke);
		std::cout << "Added stroke to dispatch queue" << std::endl;
	}

	// Cleanup
	interception_destroy_context(context);
}

void BasicProfileEnforcer::addToDispatch(PendingStroke* stroke) {
	dq_mutex.lock();
	dispatchQueue.push(stroke);
	stroke = dispatchQueue.back();
	dq_mutex.unlock();

	dq_sem.notify();
}

PendingStroke* BasicProfileEnforcer::getFromDispatch() {
	dq_sem.wait();
	dq_mutex.lock();

	PendingStroke* stroke = dispatchQueue.front();
	dispatchQueue.pop();

	dq_mutex.unlock();

	return stroke;
}
