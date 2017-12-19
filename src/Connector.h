#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "noncopyable.h"
#include "InetAddress.h"
#include "Channel.h"
#include <memory>
#include <functional>

class EventLoop;

class Connector : public noncopyable, public std::enable_shared_from_this<Connector> {
	using NewConnectionCallback = std::function<void(int fd)>;
public:
	Connector(EventLoop* loop, const InetAddress& serverAddr);
	void setNewConnectionCallback(const NewConnectionCallback& cb) { newConnectionCallback_ = cb; }
	void start();
	
private:
	void connecting(int fd);
	void handleWrite();
	void handleError();

	enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected };
	
	EventLoop* loop_;
	InetAddress serverAddr_;
	StateE state_;
	std::unique_ptr<Channel> channel_;
	NewConnectionCallback newConnectionCallback_;
};

#endif