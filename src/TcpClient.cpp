#include "TcpClient.h"
using namespace std::placeholders;

TcpClient::TcpClient(EventLoop* loop, const InetAddress& serverAddr) :
	loop_(loop),
	connector_(new Connector(loop, serverAddr))
{
	connector_->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, _1));
}

void TcpClient::newConnection(int fd)
{
	InetAddress localAddr = InetAddress::getLocalAddr(fd);
	TcpConnectionPtr conn(new TcpConnection(loop_, "client", fd, localAddr));
	connection_ = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	conn->setCloseCallback(std::bind(&TcpClient::removeConnection, this, _1));
	conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
	connection_.reset();
}

void TcpClient::start()
{
	connector_->start();
}
