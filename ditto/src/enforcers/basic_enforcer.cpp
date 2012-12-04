#include "enforcers/basic_enforcer.h"


// Should at some point setup ditto with a way to test this
const ChronoMicroDuration BasicProfileEnforcer::ditto_overhead = ChronoMicroDuration(40); // 40 microseconds

BasicProfileEnforcer::BasicProfileEnforcer() {
    looseStrokes = false;
	return;
}

void BasicProfileEnforcer::setLooseStrokes(bool state) {
    this->looseStrokes = state;
}

/**
 * This is only used with loose key strokes is enabled. It allows
 * us to delay a key up event while a key down is occurring in the
 * regular dispatcher.
 */
void BasicProfileEnforcer::delayedUpDispatch() {
    std::cout << "\n---- Delayed Key Up Dispatcher Started ----\n" << std::endl;

    KeyUpDelay* delayStroke;
    KDProfileKey key;

    ChronoMicroDuration pressTime, durSinceKD, sleepTime;

    long actualTime = 0;

    while (true) {
        delayStroke = this->getFromDelayDispatch();

        key = delayStroke->stroke.code;
        if (delayStroke->stroke.state != INTERCEPTION_KEY_UP) {
            std::cerr << "delayedUpDispatch received non key up stroke" << std::endl;
            exit(2);
        }

        pressTime = this->profileRef->getPressTime(key);
        durSinceKD = ChronoClock::now() - delayStroke->kd_dispatch_time;

        sleepTime = pressTime - durSinceKD;

        if (sleepTime.count() > 0) {
            boost::this_thread::sleep(boost::posix_time::microseconds(sleepTime.count()));
        }

        interception_send(context, device, (InterceptionStroke *) &(delayStroke->stroke), 1);

        actualTime = (ChronoClock::now() - delayStroke->kd_dispatch_time).count();
        actualTime = actualTime / 1000;

        free(delayStroke);

        std::cout << "<UP Loose(" << key << ")> time: " << actualTime;
        std::cout << " ms (profile: " << pressTime.count() / 1000 << " ms)" << std::endl;
    }
}

/**
 * The dispatcher grabs strokes from the queue and releases them according to the profile.
 * It delays both for key press times and fly times.
 */
void BasicProfileEnforcer::dispatcher() {
	std::cout << "\nBasicProfileEnforcer dispatchReceiver started\n" << std::endl;

    // Create dispatch thread
	boost::thread* delayUpThread;
	KeyUpDelay* delayStruct;
	if (this->looseStrokes) {
        delayUpThread = new boost::thread(boost::bind(&BasicProfileEnforcer::delayedUpDispatch, this));
	}

	ChronoStopwatch watch = ChronoStopwatch();
	PendingStroke* pStroke = NULL;
	PendingStroke* nextStroke = NULL;
	InterceptionKeyStroke stroke;

	KeyDownHist kd_hist;
	std::map<KDProfileKey, KeyDownHist>::iterator kd_hist_it;

	ChronoClockPoint lapPoint, lastKeyDown, nextIterationTime, peekAheadTime;
	lastKeyDown = ChronoClock::now();

	ChronoMicroDuration lapDuration, flyTime, pressTime, sleepTime, heldKeyTime, nextStrokeDur;
	heldKeyTime = ChronoStopwatch::durationFromMicro(HELD_KEY_INTERVAL);
	KDProfileKey current_key = SCANCODE_A;

	bool firstStroke = true;
	bool lastStrokeDown = true;
	bool ignoreNextKeyUp = false;
	bool setToThisDispatchTime;

	// Used to print out the actual time it took for the fly
	long actualTime = 0; // in ms

	watch.start();
	while (true) {
		pStroke = this->getFromDispatch();
		lapPoint = watch.now();

		stroke = pStroke->stroke;
		if (lapDuration.count() < 0) {
			std::cerr << "The high precison timer has failed. Must exit." << std::endl;
			exit(-1);
		}

		// Key-Down Event
		if (stroke.state == INTERCEPTION_KEY_DOWN) {
			/* Recalculate the sleeptime. We need to do this because if there are a lot pending
			 * strokes with a set of held key keydown events then they will be released all at one
			 * while if they were to come in normally they would have a delay at the maximum
			 * repeat rate for the keyboard/OS.
			 */
			if (lastStrokeDown && current_key == stroke.code) {
				flyTime = heldKeyTime;
			} else {
				flyTime = this->profileRef->getFlyTime(current_key, stroke.code);
			}

			sleepTime = lapPoint - lastKeyDown; // This gives us how long it has been since the last KD
			sleepTime = flyTime - sleepTime; // How long we should sleep to satisfy the profile

			if (sleepTime.count() > 0) {
                // peek a the next time to see if this sleeptime may cause
                // us to be behind schedule on the next one. If so, change the delay.
                nextStroke = this->peekNextDispatch();
                if (nextStroke != NULL) {
                    // We are only concerned about press times which may come between
                    // this key down and the next
                    if (nextStroke->stroke.state == INTERCEPTION_KEY_UP) {
                        // Get the key down dispatch time for the corresponding key down
                        kd_hist_it = dispatchDownHist.find(nextStroke->stroke.code);
                        if (kd_hist_it == dispatchDownHist.end()) {
                            if (nextStroke->stroke.code == stroke.code) {
                                setToThisDispatchTime = true;
                            } else {
                                std::cerr << "(peekAhead): Received key up stroke with no previous corresponding key down" << std::endl;
                                std::cerr << "Peeked Stroke = " << nextStroke->stroke.code << std::endl;
                                std::cerr << "Current keydown stroke = " << stroke.code << std::endl;
                                exit(2);
                            }
                        } else {
                            kd_hist = kd_hist_it->second;
                        }

                        // How long is the press time for the keyup that will run next?
                        pressTime = this->profileRef->getPressTime(nextStroke->stroke.code);

                        // When will the next iteration run after we sleep?
                        nextIterationTime = lapPoint + sleepTime;

                        // In the case were the next keyup is for the current keydown we are processing
                        if (setToThisDispatchTime) {
                            kd_hist.kd_dispatch_time == nextIterationTime;
                        }

                        // What time will the keyup need to go in order to be on time
                        peekAheadTime =  kd_hist.kd_dispatch_time + pressTime;

                        if (peekAheadTime < nextIterationTime) {
                            if (this->looseStrokes && !setToThisDispatchTime) {
                                std::cout << "--------------------- Sending key up to delay queue ----------------------" << std::endl;
                                // the next keyup is not for this keydown and we are allowed to interleave keys
                                delayStruct = (KeyUpDelay*) malloc(sizeof(KeyUpDelay));
                                delayStruct->stroke = nextStroke->stroke;
                                delayStruct->kd_dispatch_time = kd_hist.kd_dispatch_time;

                                this->addToDelayDispatch(delayStruct);

                                // So that the key up code section doesn't try to process it
                                ignoreNextKeyUp = true;
                            } else {
                                ChronoMicroDuration difference = nextIterationTime - peekAheadTime;

                                // Split the difference of this duration between this stroke and the next
                                difference /= 2;
                                if (difference.count() > MAX_FLY_SPLIT) {
                                    std::cout << "Time Difference " << difference.count() / 1000;
                                    std::cout << " ms is too much to make up" << std::endl;
                                } else {
                                    sleepTime -= difference;

                                    std::cout << "(peekAhead press): Making up time" << std::endl;
                                }
                            }
                        }
                    }
                }

				boost::this_thread::sleep(boost::posix_time::microseconds(sleepTime.count()));
			}
			interception_send(context, device, (InterceptionStroke *) &(stroke), 1);

			kd_hist.kd_dispatch_time = watch.now();

            // The actual time for the key fly. This is what we will put out to the console.
			actualTime = (kd_hist.kd_dispatch_time - lastKeyDown).count() / 1000;
			lastKeyDown = kd_hist.kd_dispatch_time;

			current_key = stroke.code;

			/* We check below if the key is already in the history because if the user
			 * holds down a key we will receive multiple key down events but for the purpose
			 * of measuring the press time we only want the first one.
			 */
			if (dispatchDownHist.count(current_key) == 0) {
				// Not a held down key
				dispatchDownHist[current_key] = kd_hist;
			}

			free(pStroke);
			setToThisDispatchTime = false;
			lastStrokeDown = true;

			std::cout << "<DOWN(" << stroke.code << ")> time: " << actualTime;
			std::cout << " ms (profile: " << flyTime.count() / 1000 << " ms)" << std::endl;
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

			if (ignoreNextKeyUp) {
                dispatchDownHist.erase(kd_hist_it);
                free(pStroke);

                lastStrokeDown = false;
                ignoreNextKeyUp = false;

                continue;
			}

			// This holds the time the corresponding keydown was sent out
			kd_hist = kd_hist_it->second;

			// Take the profile's press time and subtract the time since the keydown
			sleepTime = pressTime - (lapPoint - kd_hist.kd_dispatch_time);

			if (sleepTime.count() > 0) {
                nextStroke = this->peekNextDispatch();
                if (nextStroke != NULL) {
                    // We are only concerned about press times which may come between
                    // this key down and the next
                    if (nextStroke->stroke.state == INTERCEPTION_KEY_DOWN) {
                        // How long is the fly time for the keydown that will run next?
                        //flyTime = this->profileRef->getFlyTime(current_key, nextStroke->stroke.code);
                        nextStrokeDur = this->profileRef->getFlyTime(current_key, nextStroke->stroke.code);
                    } else {
                        nextStrokeDur = this->profileRef->getPressTime(nextStroke->stroke.code);
                    }

                    // When will the next iteration run after we sleep?
                    nextIterationTime = lapPoint + sleepTime;

                    // What time will the keyup need to go in order to be on time
                    peekAheadTime =  lastKeyDown + nextStrokeDur;

                    //std::cout << "Peak ahead difference = " << (peekAheadTime - nextIterationTime).count() << std::endl;
                    if (peekAheadTime < nextIterationTime) {
                        if (this->looseStrokes && this->profileRef->canLoosenKey(stroke.code)) {
                            // Break this keyup away from this loop.
                            delayStruct = (KeyUpDelay*) malloc(sizeof(KeyUpDelay));
                            delayStruct->stroke = stroke;
                            delayStruct->kd_dispatch_time = kd_hist.kd_dispatch_time;

                            this->addToDelayDispatch(delayStruct);

                            dispatchDownHist.erase(kd_hist_it);
                            free(pStroke);
                            lastStrokeDown = false;
                            continue;
                        } else {
                            ChronoMicroDuration difference = nextIterationTime - peekAheadTime;

                            // Split the difference of this duration between this stroke and the next
                            difference /= 2;

                            if (difference.count() > MAX_PRESS_SPLIT) {
                                std::cout << "Time difference " << difference.count();
                                std::cout << " ms is too much to make up" << std::endl;
                            } else {
                                sleepTime -= difference;
                                std::cout << "(peekAhead fly): Making up time" << std::endl;
                            }
                        }
                    }
                }
				boost::this_thread::sleep(boost::posix_time::microseconds(sleepTime.count()));
			}

			interception_send(context, device, (InterceptionStroke *) &(stroke), 1);

            // The actual time the press took. This will go out to the terminal
            actualTime = (watch.now() - kd_hist.kd_dispatch_time).count() / 1000;
			dispatchDownHist.erase(kd_hist_it);
			free(pStroke);

			// Used in detecting a held key
			lastStrokeDown = false;

			std::cout << "<UP(" << stroke.code << ")> time: " << actualTime;
			std::cout << " ms (profile: " << pressTime.count() / 1000 << " ms)" << std::endl;
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
	//stroke = dispatchQueue.back();
	dq_mutex.unlock();

	dq_sem.notify();
}

void BasicProfileEnforcer::addToDelayDispatch(KeyUpDelay* stroke) {
	delay_mutex.lock();
	delayUpQueue.push(stroke);
	//stroke = delayUpQueue.back();
	delay_mutex.unlock();

	delay_sem.notify();
}

PendingStroke* BasicProfileEnforcer::getFromDispatch() {
	dq_sem.wait();
	dq_mutex.lock();

	PendingStroke* stroke = dispatchQueue.front();
	dispatchQueue.pop();

	dq_mutex.unlock();

	return stroke;
}

KeyUpDelay* BasicProfileEnforcer::getFromDelayDispatch() {
	delay_sem.wait();
	delay_mutex.lock();

	KeyUpDelay* stroke = delayUpQueue.front();
	delayUpQueue.pop();

	delay_mutex.unlock();

	return stroke;
}

PendingStroke* BasicProfileEnforcer::peekNextDispatch() {
    dq_mutex.lock();
    if (dispatchQueue.size() == 0) {
        dq_mutex.unlock();
        return NULL;
    }
    PendingStroke* stroke = dispatchQueue.front();

    dq_mutex.unlock();
    return stroke;
}
