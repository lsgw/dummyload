#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"
#include <functional>
#include <memory>

class Socket;
class Channel;
class EventLoop;
class InetAddress;

class Acceptor : noncopyable {
	using NewConnectionCallback = std::function<void(int fd, const InetAddress&)>;
public:
	Acceptor(EventLoop* loop, const InetAddress& listenAddr);

	void listen();
	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{
		newConnectionCallback_ = cb;
	}
private:
	void handleRead();

	EventLoop* loop_;
	std::unique_ptr<Socket> socket_;
	std::unique_ptr<Channel> channel_;

	NewConnectionCallback newConnectionCallback_;
};


#endif