#include "util/semaphore.h"


semaphore::semaphore(): count_(0) {};
semaphore::semaphore(unsigned long startCount)
: count_(startCount) {}

void semaphore::notify() {
	boost::mutex::scoped_lock lock(mutex_);
	++count_;
	condition_.notify_one();
}

void semaphore::wait() {
	boost::mutex::scoped_lock lock(mutex_);
	while(count_ == 0)
		condition_.wait(lock);

	--count_;
}
