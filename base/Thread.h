#ifndef THREAD_H
#define THREAD_H

#include "noncopyable.h"
#include <string>
#include <functional>
#include <pthread.h>

class Thread : noncopyable {
	using ThreadFunc = std::function<void(void)>;
public:
	Thread(const ThreadFunc& func, std::string name=std::string());
	~Thread();
	
	void start();
	int join();
private:
	pthread_t pthreadId_;

	bool started_;
	bool joined_;
	ThreadFunc func_;
	std::string name_;
};

#endif