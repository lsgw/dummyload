#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "noncopyable.h"
#include "Mutex.h"
#include "Condition.h"
#include <list>
#include <vector>
#include <functional>
#include <string>

class Thread;

class ThreadPool : noncopyable {
public:
	using Task = std::function<void(void)>;
	ThreadPool(const std::string& name=std::string("ThreadPool"));
	~ThreadPool();

	void setMaxQueueSize(int maxSize) { maxQueue_ = maxSize; }
	void start(int n);
	void run(const Task& task);
	void stop();
	
private:
	bool isFull() const;
	Task take();
	void runInThread();

	mutable Mutex mutex_;
	Condition notEmpty_;
	Condition notFull_;

	std::vector<Thread*> threads_;
	std::list<Task> queue_;
	int maxQueue_;

	std::string name_;

	bool running_;
};

#endif