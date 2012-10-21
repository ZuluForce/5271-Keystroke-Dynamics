#include "enforcers/basic_enforcer.h"


BasicProfileEnforcer::BasicProfileEnforcer() {
	return;
}

void BasicProfileEnforcer::dispatchDown() {
	std::cout << "dispatchDown thread started\n" << std::endl;

	PendingStroke* stroke;
	int64_t sleeptime;
	while (true) {
		stroke = getFromQueue(dispatchDownQueue, dd_mutex, dd_sem);
		std::cout << "dispatchDown received stroke" << std::endl;

		sleeptime = stroke->wait_time.count();
		std::cout << "Sleeping for " << sleeptime << " us" << std::endl;

		// Sleep for the appropriate time
		//auto boostPoint = ChronoStopwatch::toBoostTimePoint(stroke->release_time);
		//boost::this_thread::sleep_until(boostPoint);
		if (sleeptime > 0)
			boost::this_thread::sleep(boost::posix_time::microseconds(stroke->wait_time.count()));

		interception_send(context, device, (InterceptionStroke *) &(stroke->stroke), 1);
		free(stroke);
	}
}

void BasicProfileEnforcer::dispatchUp() {
	std::cout << "dispatchUp thread started\n" << std::endl;

	PendingStroke* stroke;
	int64_t sleeptime;
	while (true) {
		stroke = getFromQueue(dispatchUpQueue, du_mutex, du_sem);
		std::cout << "dispatchUp received stroke" << std::endl;

		sleeptime = stroke->wait_time.count();
		std::cout << "Sleeping for " << sleeptime << " us" << std::endl;
		// Sleep for the appropriate time
		//auto boostPoint = ChronoStopwatch::toBoostTimePoint(stroke->release_time);
		//boost::this_thread::sleep_until(boostPoint);
		if (sleeptime > 0)
			boost::this_thread::sleep(boost::posix_time::microseconds(stroke->wait_time.count()));

		interception_send(context, device, (InterceptionStroke *) &(stroke->stroke), 1);
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

	ChronoClockPoint lapPoint;
	ChronoMicroDuration lapDuration, flyDuration, pressDuration, durationToKD, tempDuration;
	uint32_t current_key = SCANCODE_A;

	// Create other dispatcher threads
	boost::thread* dispatchUpThread =
		new boost::thread(boost::bind(&BasicProfileEnforcer::dispatchUp, this));
	boost::thread* dispatchDownThread =
		new boost::thread(boost::bind(&BasicProfileEnforcer::dispatchDown, this));

	bool firstStroke = true;

	watch.start();
	while (true) {
		pStroke = this->getFromDispatch();
		lapDuration = watch.lapDuration();

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

			std::cout << "flyDuration: " << flyDuration.count() << " us" << std::endl;

			// This calculates the time in the future when we need release the key down event
			tempDuration = ChronoStopwatch::getDurationDifference(flyDuration,lapDuration);
			//pStroke->release_time =
			//	ChronoStopwatch::getFutureTimePoint(lapPoint, tempDuration);

			pStroke->wait_time = tempDuration;

			// Save this so we can do appropriate time calculations when the keyup comes in
			kd_hist.kd_received = lapPoint;
			kd_hist.kd_wait = tempDuration;

			dispatchDownHist[stroke.code] = kd_hist;

			addToQueue(dispatchDownQueue, pStroke, dd_mutex, dd_sem);

			// Reset the current key
			current_key = stroke.code;
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

		std::cout << "Received Stroke: " << kstroke.code << std::endl;
		tempPStroke = (PendingStroke*) malloc(sizeof(PendingStroke));

		tempPStroke->stroke = kstroke;

		this->addToDispatch(tempPStroke);
		std::cout << "Added stroke to dispatch queue" << std::endl;
	}

	// Cleanup
	interception_destroy_context(context);
}

void BasicProfileEnforcer::addToQueue(std::queue<PendingStroke*>& q,PendingStroke* stroke,boost::mutex& m, semaphore& sem) {
	m.lock();
	q.push(stroke);
	m.unlock();

	sem.notify();
	std::cout << "addToQueue: added stroke to queue" << std::endl;
}

PendingStroke* BasicProfileEnforcer::getFromQueue(std::queue<PendingStroke*>& q, boost::mutex& m, semaphore& sem) {
	sem.wait();
	m.lock();

	PendingStroke* stroke = q.front();
	q.pop();

	m.unlock();

	std::cout << "getFromQueue: retrieved stroke from queue" << std::endl;

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
