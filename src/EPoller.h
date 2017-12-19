#ifndef EPOLLER_H
#define EPOLLER_H

#include "noncopyable.h"
#include <vector>
class EventLoop;
class Channel;

class EPoller : noncopyable {
	using ChannelList = std::vector<Channel*>;
public:
	EPoller(EventLoop* loop);
	~EPoller();

	void wait(int timeout, ChannelList* activeChannels);
	void updateChannel(Channel* channel);

private:
	void update(int operation, Channel* channel);

	EventLoop* loop_;
	int epfd_;
	std::vector<struct epoll_event> events_;
};


#endif