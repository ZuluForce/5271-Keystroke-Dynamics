#include "enforcers/basic_enforcer.h"


// Should at some point setup ditto with a way to test this
const ChronoMicroDuration BasicProfileEnforcer::ditto_overhead = ChronoMicroDuration(50); // 50 microseconds

BasicProfileEnforcer::BasicProfileEnforcer() {
	return;
}

void BasicProfileEnforcer::dispatchDown() {
	std::cout << "dispatchDown thread started\n" << std::endl;

	PendingStroke* stroke;
	int64_t sleeptime;
	int64_t keydown_count = 0;
	while (true) {
		stroke = getFromQueue(dispatchDownQueue, dd_mutex, dd_sem);

		sleeptime = stroke->wait_time.count();
		std::cout << "vDOWNv Sleeping for " << sleeptime << " us" << std::endl;

		// Sleep for the appropriate time
		//auto boostPoint = ChronoStopwatch::toBoostTimePoint(stroke->release_time);
		//boost::this_thread::sleep_until(boostPoint);
		if (sleeptime > 0)
			boost::this_thread::sleep(boost::posix_time::microseconds(sleeptime));

		interception_lock.lock();
		interception_send(context, device, (InterceptionStroke *) &(stroke->stroke), 1);
		interception_lock.unlock();

		++keydown_count;
		std::cout << "keydown count: " << keydown_count << std::endl;
		free(stroke);
	}
}

void BasicProfileEnforcer::dispatchUp() {
	std::cout << "dispatchUp thread started\n" << std::endl;

	PendingStroke* stroke;
	int64_t sleeptime;
	int64_t keyup_count = 0;
	while (true) {
		stroke = getFromQueue(dispatchUpQueue, du_mutex, du_sem);

		sleeptime = stroke->wait_time.count();
		std::cout << "^UP^ Sleeping for " << sleeptime << " us" << std::endl;
		// Sleep for the appropriate time
		//auto boostPoint = ChronoStopwatch::toBoostTimePoint(stroke->release_time);
		//boost::this_thread::sleep_until(boostPoint);
		if (sleeptime > 0)
			boost::this_thread::sleep(boost::posix_time::microseconds(sleeptime));

		interception_lock.lock();
		interception_send(context, device, (InterceptionStroke *) &(stroke->stroke), 1);
		interception_lock.unlock();

		++keyup_count;
		std::cout << "keyup count: " << keyup_count << std::endl;
		free(stroke);
	}
}

void BasicProfileEnforcer::dispatchReceiver() {
	std::cout << "\nBasicProfileEnforcer dispatchReceiver started\n" << std::endl;
	boost::this_thread::disable_interruption di;

	ChronoStopwatch watch = ChronoStopwatch();
	PendingStroke* pStroke = NULL;
	InterceptionKeyStroke stroke;

	KeyDownHist kd_hist;
	std::map<int, KeyDownHist>::iterator kd_hist_it;

	ChronoClockPoint lapPoint, nextDispatchPoint, nextUpDispatchPoint;
	ChronoMicroDuration lapDuration, flyDuration, pressDuration, tempDuration;
	KDProfileKey current_key = SCANCODE_A;

	// Create other dispatcher threads
	boost::thread* dispatchUpThread =
		new boost::thread(boost::bind(&BasicProfileEnforcer::dispatchUp, this));
	boost::thread* dispatchDownThread =
		new boost::thread(boost::bind(&BasicProfileEnforcer::dispatchDown, this));

	bool firstStroke = true;

	// We need to initialize this on the very first stroke but not after that
	//bool nextDispatchInit = true;

	nextDispatchPoint = ChronoClock::now();
	nextUpDispatchPoint = nextDispatchPoint;
	watch.start();
	while (true) {
		pStroke = this->getFromDispatch();
		lapDuration = watch.lapDuration();

		// Do once
		//if (nextDispatchInit) {
		//	nextDispatchPoint = ChronoClock::now();
		//	nextDispatchInit = false;
		//}

		stroke = pStroke->stroke;
		std::cout << "Recieved stroke: " << stroke.code <<  " (" << lapDuration.count() << " us)" << std::endl;
		if (lapDuration.count() < 0) {
			std::cerr << "The high precison timer has failed. Must exit." << std::endl;
			exit(-1);
		}

		if (stroke.state == INTERCEPTION_KEY_DOWN) {
			// Get the appropriate flight time
			flyDuration = this->profileRef->getFlyTime(current_key, stroke.code);
			lapPoint = watch.lastLapTotalTime();

			std::cout << "Profile fly time: " << flyDuration.count() << " us" << std::endl;

			if (lapPoint > nextDispatchPoint) {
				// The previous key down stroke has been delivered

				// Time since the last key down dispatch
				tempDuration = lapPoint - nextDispatchPoint;

				// How long we should wait
				tempDuration = flyDuration - tempDuration;

				// If we allow negative values here it will screw up the 'nextDispatchPoint', the time
				// for the next key to be dispatched will continue to get further in the past
				pStroke->wait_time = tempDuration.count() < 0 ? ChronoMicroDuration(0) : tempDuration;

				nextDispatchPoint = ChronoClock::now() + pStroke->wait_time;
			} else {
				// If we get this stroke before the previous has been sent we should set the wait
				// time to be the full fly time since it will be applied right after that key is
				// delivered
				pStroke->wait_time = flyDuration;

				nextDispatchPoint += flyDuration;
			}

			// Save this so we can do appropriate time calculations when the keyup comes in
			kd_hist.kd_received = lapPoint;
			kd_hist.kd_wait = pStroke->wait_time;

			dispatchDownHist[stroke.code] = kd_hist;

			addToQueue(dispatchDownQueue, pStroke, dd_mutex, dd_sem);

			// Reset the current key
			current_key = stroke.code;

			// Some very basic clock drift estimate. I am not even sure if this
			// overhead is correct. If anything it is an underestimate of the actual overhead
			nextDispatchPoint += BasicProfileEnforcer::ditto_overhead;

		} else if (stroke.state == INTERCEPTION_KEY_UP) {
			kd_hist_it = dispatchDownHist.find(stroke.code);
			if (kd_hist_it == dispatchDownHist.end()) {
				if (firstStroke && stroke.code == SCANCODE_RTRN) {
					std::cout << "Ignoring initiall return keyup" << std::endl;
					free(pStroke);
					continue;
				}
				std::cerr << "Ditto received a keyup event for which it doesn't have a keydown" << std::endl;
				exit(-1);
			}
			kd_hist = kd_hist_it->second;
			pressDuration = this->profileRef->getPressTime(stroke.code);
			lapPoint = watch.lastLapTotalTime();

			// Find how much longer until the keydown is delivered

			durationToKD =
				ChronoStopwatch::getFutureTimePoint(kd_hist.kd_received, kd_hist.kd_wait) - lapPoint;
			durationToKD = durationToKD + pressDuration;

			// We then add the profile presstime to the time in the future that the key down
			// event will occur
			pStroke->wait_time = durationToKD;

			addToQueue(dispatchUpQueue, pStroke, du_mutex, du_sem);

			dispatchDownHist.erase(kd_hist_it);

			// We don't reset the current_key on keyup
		}
	}
}


void BasicProfileEnforcer::enforce(KDProfile& profile) {
	std::cout << "\n ==== BasicProfilerEnforcer starting ====" << std::endl;
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

		//std::cout << "Received Stroke: " << kstroke.code << std::endl;
		tempPStroke = (PendingStroke*) malloc(sizeof(PendingStroke));

		tempPStroke->stroke = kstroke;

		this->addToDispatch(tempPStroke);
	}

	// Cleanup
	interception_destroy_context(context);
}

void BasicProfileEnforcer::addToQueue(std::queue<PendingStroke*>& q,PendingStroke* stroke,boost::mutex& m, semaphore& sem) {
	m.lock();
	q.push(stroke);
	m.unlock();

	sem.notify();
}

PendingStroke* BasicProfileEnforcer::getFromQueue(std::queue<PendingStroke*>& q, boost::mutex& m, semaphore& sem) {
	sem.wait();
	m.lock();

	PendingStroke* stroke = q.front();
	q.pop();

	m.unlock();

	return stroke;
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
