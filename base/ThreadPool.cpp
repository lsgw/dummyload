#include "ThreadPool.h"
#include "Thread.h"

using namespace std;

ThreadPool::ThreadPool(const string& name) :
	maxQueue_(0),
	name_(name),
	running_(false),
	notEmpty_(mutex_),
	notFull_(mutex_)
{

}

ThreadPool::~ThreadPool()
{
	if (running_) {
		stop();
	}
}

void ThreadPool::stop()
{
	{
		LockGuard locker(mutex_);
		running_ = false;
		notEmpty_.notifyAll();
	}
	for (auto& t : threads_) {
		t->join();
	}
	for (auto& t : threads_) {
		delete t;
	}
}

void ThreadPool::start(int n)
{
	running_ = true;
	threads_.reserve(n);

	for (int i=0; i<n; i++) {
		threads_.push_back(new Thread(bind(&ThreadPool::runInThread, this)));
		threads_[i]->start();
	}
}
ThreadPool::Task ThreadPool::take()
{
	LockGuard locker(mutex_);

	while (queue_.empty() && running_) {
		notEmpty_.wait();
	}

	Task task;
	if (!queue_.empty()) {
		task = queue_.front();
		queue_.pop_front();
		if (maxQueue_ > 0) {
			notFull_.notify();
		}
	}

	return task;
}

void ThreadPool::run(const Task& task)
{
	if (threads_.empty()) {
		task();
	} else {
		LockGuard locker(mutex_);
		while (isFull()) {
			notFull_.wait();
		}

		queue_.push_back(task);
		notEmpty_.notify();
	}
}
bool ThreadPool::isFull() const 
{
	return maxQueue_ > 0 && queue_.size() >= maxQueue_;
}


void ThreadPool::runInThread()
{
	while (running_) {
		Task task = take();
		if (task) {
			task();
		}
	}
}
