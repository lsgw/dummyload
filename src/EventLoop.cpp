#include "EventLoop.h"
#include "Channel.h"
#include "EPoller.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

EventLoop::EventLoop() : 
	looping_(false),
	quit_(false),
	epoller_(new EPoller(this)),
	timerQueue_(new TimerQueue(this))
{
}
EventLoop::~EventLoop()
{

}

void EventLoop::updateChannel(Channel* channel)
{
	epoller_->updateChannel(channel);
}

void EventLoop::loop()
{
	looping_ = true;

	while (!quit_) {
		activeChannels_.clear();

		epoller_->wait(1000, &activeChannels_);
		
		for (auto& channel : activeChannels_) {
			channel->handleEvent();
		}

	}

	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
}


Timer* EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
	return timerQueue_->addTimer(cb, time, 0.0);
}

Timer* EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), delay));
	return runAt(time, cb);
}

Timer* EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), interval));
	return timerQueue_->addTimer(cb, time, interval);
}
void EventLoop::cancel(Timer* timer)
{
	return timerQueue_->cancel(timer);
}

