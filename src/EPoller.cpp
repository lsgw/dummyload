#include "EPoller.h"
#include "EventLoop.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>

EPoller::EPoller(EventLoop* loop) :
	loop_(loop),
	epfd_(::epoll_create(256)),
	events_(16)
{
	if (epfd_ < 0) {
		::abort();
	}
}
EPoller::~EPoller()
{
	::close(epfd_);
}

void EPoller::wait(int timeout, ChannelList* activeChannels)
{
	int numEvents = epoll_wait(epfd_, &(*events_.begin()), 16, timeout);
	for (int i=0; i<numEvents; i++) {
		Channel* channel = (Channel*)events_[i].data.ptr;
		channel->setRevents(events_[i].events);
		activeChannels->push_back(channel);
	}
}


void EPoller::updateChannel(Channel* channel)
{
	if (channel->state() == Channel::kNew || channel->state() == Channel::kDeleted) {
		channel->state(Channel::kAdded);
		update(EPOLL_CTL_ADD, channel);

	} else if (channel->state() == Channel::kAdded && channel->isNoneEvent()) {
		channel->state(Channel::kDeleted);
		update(EPOLL_CTL_DEL, channel);

	} else if (channel->state() == Channel::kAdded) {
		update(EPOLL_CTL_MOD, channel);
	}
	
}

void EPoller::update(int operation, Channel* channel)
{
	struct epoll_event ev;
	ev.events   = channel->events();
	ev.data.ptr = channel;
	epoll_ctl(epfd_, operation, channel->fd(), &ev); 
}

