#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "noncopyable.h"
#include "TimerQueue.h"
#include "Timer.h"
#include "Timestamp.h"
#include <vector>
#include <memory>
#include <functional>

class Channel;
class EPoller;
struct epoll_event;

class EventLoop : noncopyable {
	using ChannelList = std::vector<Channel*>;
	using TimerCallback = std::function<void(void)>;
public:
	EventLoop();
	~EventLoop();

	void loop();
	void quit();
	void updateChannel(Channel* channel);


	Timer* runAt(const Timestamp& time, const TimerCallback& cb);
	Timer* runAfter(double delay, const TimerCallback& cb);
	Timer* runEvery(double interval, const TimerCallback& cb);
	void cancel(Timer* timer);

private:
	bool looping_;
	bool quit_;

	std::unique_ptr<EPoller> epoller_;
	ChannelList activeChannels_;

	std::unique_ptr<TimerQueue> timerQueue_;
};


#endif