#ifndef SEMAPHORE_H_INCLUDED
#define SEMAPHORE_H_INCLUDED

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

class semaphore {
private:
    boost::mutex mutex_;
    boost::condition_variable condition_;
    unsigned long count_;

public:
    semaphore();
    semaphore(unsigned long startCount);

    void notify();

    void wait();
};

#endif // SEMAPHORE_H_INCLUDED
