#ifndef CONDITION_H
#define CONDITION_H

#include "noncopyable.h"
#include "Mutex.h"
#include <pthread.h>

class Condition : noncopyable {
public:
	Condition(Mutex& mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
private:
	Mutex& mutex_;
	pthread_cond_t pcond_;
};

#endif