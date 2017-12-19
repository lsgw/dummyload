#include "Thread.h"
#include <assert.h>
#include <stdlib.h>

struct ThreadData {
	using ThreadFunc = std::function<void(void)>;

	ThreadData(const ThreadFunc& func) : func_(func) { }
	void runInThread()
	{
		func_();
	}
	ThreadFunc func_;
};

void* startThread(void* obj)
{
	ThreadData* data = (ThreadData*)obj;
	data->runInThread();
	delete data;
	return NULL;
}

Thread::Thread(const ThreadFunc& func, std::string name) :
	pthreadId_(0),
	started_(false),
	joined_(false),
	func_(func),
	name_(name)
{

}
Thread::~Thread()
{
	if (started_ && !joined_) {
		pthread_detach(pthreadId_);
	}
}

void Thread::start()
{
	started_ = true;

	ThreadData* data = new ThreadData(func_);

	if (pthread_create(&pthreadId_, NULL, startThread, data) != 0) {
		started_ = false;
		delete data;
		::fprintf(stderr, "Failed in pthread_create\n");
		::abort();
	}
}

int Thread::join()
{
	assert(started_);
	assert(!joined_);
	joined_ = true;
	return pthread_join(pthreadId_, NULL);
}




