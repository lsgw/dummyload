#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "noncopyable.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include "Connector.h"
#include <memory>

class EventLoop;

class TcpClient : public noncopyable {
public:
	TcpClient(EventLoop* loop, const InetAddress& serverAddr);
	 void start();

	void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
	void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
private:
	void newConnection(int fd);
	void removeConnection(const TcpConnectionPtr& conn);

	EventLoop* loop_;

	std::unique_ptr<Connector> connector_;
	TcpConnectionPtr connection_;

	ConnectionCallback    connectionCallback_;
	MessageCallback       messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
};

#endif