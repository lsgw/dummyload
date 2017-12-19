#ifndef CHANNEL_H
#define CHANNEL_H

#include "noncopyable.h"
#include <functional>
#include <sys/epoll.h>

class EventLoop;

class Channel : noncopyable {
public:
	using EventCallback = std::function<void(void)>;
	enum State {
		kNew,
		kAdded,
		kDeleted,
	};
public:
	Channel(EventLoop *loop, int fd);

	void handleEvent();
	void setReadCallback(const EventCallback& cb) {	readCallback_ = cb; }
	void setWriteCallback(const EventCallback& cb) { writeCallback_ = cb; }
	void setErrorCallback(const EventCallback& cb) { errorCallback_ = cb; }


	int fd() const { return fd_; }
	int events() const { return events_; }
	void setRevents(int revents) { revents_ = revents; }

	bool isNoneEvent() const { return events_ == kNoneEvent; }
	void enableReading() { events_ |= kReadEvent; update(); }
	void disableReading() { events_ &= ~kReadEvent; update(); }
	void enableWriting() { events_ |= kWriteEvent; update(); }
	void disableWriting() { events_ &= ~kWriteEvent; update(); }
	void disableAll() { events_ = kNoneEvent; update(); }
	bool isWriting() const { return events_ & kWriteEvent; }
	bool isReading() const { return events_ & kReadEvent; }

	EventLoop* ownerLoop() { return loop_; }

	State state() { return state_; }
	void  state(State s) { state_ = s; }

private:
	void update();

private:
	EventLoop* loop_;
	const int  fd_;
	int        events_;
	int        revents_; // it's the received event types of epoll or poll
	State      state_;
	
	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback errorCallback_;

	static const int kNoneEvent = 0;
	static const int kReadEvent = EPOLLIN | EPOLLPRI;
	static const int kWriteEvent = EPOLLOUT;
};


#endif