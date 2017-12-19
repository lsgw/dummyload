#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "noncopyable.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <memory>
#include <map>
#include <string>

class EventLoop;
class InetAddress;

class TcpServer : noncopyable {
public:
	TcpServer(EventLoop* loop, const InetAddress& listenAddr);
	
	EventLoop* getLoop() const { return loop_; }
	void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
	void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
	void start();
private:
	void newConnection(int fd, const InetAddress& peerAddr);
	void removeConnection(const TcpConnectionPtr& conn);
	
	EventLoop* loop_;
	std::unique_ptr<Acceptor> acceptor_;

	bool started_;
	int nextId_;

	ConnectionCallback connectionCallback_;
	MessageCallback    messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;

	std::map<std::string, TcpConnectionPtr> connections_;
};


#endif