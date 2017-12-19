#ifndef MUTEX_H
#define MUTEX_H

#include "noncopyable.h"
#include <pthread.h>

class Mutex : noncopyable {
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();
	
	pthread_mutex_t* getPthreadMutex()
	{
		return &mutex_;
	}
private:
	pthread_mutex_t mutex_;
};

class LockGuard : noncopyable {
public:
	LockGuard(Mutex& mutex) : mutex_(mutex)
	{
		mutex_.lock();
	}
	~LockGuard()
	{
		mutex_.unlock();
	}
private:
	Mutex& mutex_;
};

#endif




