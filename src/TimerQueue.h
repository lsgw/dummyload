#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <vector>
#include <set>
#include <memory>
#include <functional>
#include "Timestamp.h"
#include "Channel.h"

class Timer;
class EventLoop;

class TimerQueue {
	using TimerCallback = std::function<void(void)>;
public:
	TimerQueue(EventLoop* loop);
	~TimerQueue();

	Timer* addTimer(const TimerCallback& cb, Timestamp when, double interval);
	void cancel(Timer* timer);

private:
	void addTimerInLoop(Timer* timer);
	void cancelInLoop(Timer* timer);
	void handleRead();

	std::vector<std::pair<Timestamp, Timer*>> getExpired(Timestamp now);
	void reset(const std::vector<std::pair<Timestamp, Timer*>>& expired, Timestamp now);
	bool insert(Timer* timer);



	EventLoop* loop_;	
	const int timerfd_;
	std::unique_ptr<Channel> channel_;
	std::set<std::pair<Timestamp, Timer*>> timers_;
	std::set<Timer*> cancelingTimers_;
	
	bool callingExpiredTimers_;
};

#endif