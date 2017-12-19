#include "Channel.h"
#include "EventLoop.h"


Channel::Channel(EventLoop *loop, int fd) :
	loop_(loop),
	fd_(fd),
	events_(0),
	revents_(0),
	state_(kNew)
{

}

void Channel::update()
{
	loop_->updateChannel(this);
}

void Channel::handleEvent()
{
	if ((revents_&(EPOLLERR)) && errorCallback_) {
		errorCallback_();
	}

	if ((revents_&(EPOLLIN|EPOLLPRI|EPOLLRDHUP)) && readCallback_) {
		readCallback_();
	}

	if ((revents_&EPOLLOUT) && writeCallback_) {
		writeCallback_();
	}
}