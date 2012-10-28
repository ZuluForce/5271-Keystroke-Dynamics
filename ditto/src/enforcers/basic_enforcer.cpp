#include "enforcers/basic_enforcer.h"


// Should at some point setup ditto with a way to test this
const ChronoMicroDuration BasicProfileEnforcer::ditto_overhead = ChronoMicroDuration(50); // 50 microseconds

BasicProfileEnforcer::BasicProfileEnforcer() {
	return;
}


void BasicProfileEnforcer::dispatcher() {
	std::cout << "\nBasicProfileEnforcer dispatchReceiver started\n" << std::endl;

	ChronoStopwatch watch = ChronoStopwatch();
	PendingStroke* pStroke = NULL;
	InterceptionKeyStroke stroke;

	KeyDownHist kd_hist;
	std::map<KDProfileKey, KeyDownHist>::iterator kd_hist_it;

	ChronoClockPoint lapPoint, lastKeyDown;
	lastKeyDown = ChronoClock::now();

	ChronoMicroDuration lapDuration, flyTime, pressTime, sleepTime;
	KDProfileKey current_key = SCANCODE_A;

	bool firstStroke = true;


	watch.start();
	while (true) {
		pStroke = this->getFromDispatch();
		lapPoint = watch.now();

		stroke = pStroke->stroke;
		if (lapDuration.count() < 0) {
			std::cerr << "The high precison timer has failed. Must exit." << std::endl;
			exit(-1);
		}

		if (stroke.state == INTERCEPTION_KEY_DOWN) {
			flyTime = this->profileRef->getFlyTime(current_key, stroke.code);
			sleepTime = lapPoint - lastKeyDown; // This gives us how long it has been since the last KD
			sleepTime = flyTime - sleepTime; // How long we should sleep to satisfy the profile

			if (sleepTime.count() > 0) {
				boost::this_thread::sleep(boost::posix_time::microseconds(sleepTime.count()));
			}
			interception_send(context, device, (InterceptionStroke *) &(stroke), 1);

			kd_hist.kd_dispatch_time = watch.now();
			lastKeyDown = kd_hist.kd_dispatch_time;

			current_key = stroke.code;

			/* We check below if the key is already in the history because if the user
			 * holds down a key we will receive multiple key down events but for the purpose
			 * of measuring the press time we only want the first one.
			 */
			if (dispatchDownHist.count(current_key) == 0)
				dispatchDownHist[current_key] = kd_hist;

			free(pStroke);

			std::cout << "<DOWN(" << stroke.code << ")> time: " << lastKeyDown.time_since_epoch().count() << std::endl;
		} else if (stroke.state == INTERCEPTION_KEY_UP) {
			if (firstStroke && stroke.code == SCANCODE_RTRN) {
				firstStroke = false;

				free(pStroke);
				continue;
			}

			pressTime = this->profileRef->getPressTime(stroke.code);
			kd_hist_it = dispatchDownHist.find(stroke.code);
			if (kd_hist_it == dispatchDownHist.end()) {
				std::cerr << "Received key up stroke with no previous corresponding key down" << std::endl;
				exit(-1);
			}
			// This holds the time the corresponding keydown was sent out
			kd_hist = kd_hist_it->second;

			// Take the profile's press time and subtract the time since the keydown
			sleepTime = pressTime - (lapPoint - kd_hist.kd_dispatch_time);

			if (sleepTime.count() > 0) {
				boost::this_thread::sleep(boost::posix_time::microseconds(sleepTime.count()));
			}

			interception_send(context, device, (InterceptionStroke *) &(stroke), 1);

			dispatchDownHist.erase(kd_hist_it);
			free(pStroke);

			lapPoint = watch.now();
			std::cout << "<UP(" << stroke.code << ")> time: " << lapPoint.time_since_epoch().count() << std::endl;
		} else {
			std::cerr << "dispatcher received stroke for unknown state (UP/DOWN)" << std::endl;
			exit(-1);
			//continue;
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
		new boost::thread(boost::bind(&BasicProfileEnforcer::dispatcher, this));

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
