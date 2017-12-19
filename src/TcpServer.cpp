#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <functional>
#include <unistd.h>

using namespace std::placeholders;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr) : 
	loop_(loop),
	acceptor_(new Acceptor(loop, listenAddr)),
	started_(false),
	nextId_(0)
{
	acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}

void TcpServer::newConnection(int fd, const InetAddress& peerAddr)
{
	nextId_++;
	std::string connName = std::to_string(nextId_);

	TcpConnectionPtr conn(new TcpConnection(loop_, connName, fd, peerAddr));
	connections_[connName] = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));
	conn->connectEstablished();
}

void TcpServer::start()
{
	if (!started_) {
		started_ = true;
		acceptor_->listen();
	}
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	connections_.erase(conn->name()); // 移除已经关闭的连接
}

