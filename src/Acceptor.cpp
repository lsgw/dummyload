#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr) :
	loop_(loop),
	socket_(new Socket(Socket::createSocketNonBlocking())),
	channel_(new Channel(loop, socket_->fd()))
{
	socket_->bind(listenAddr);
	channel_->setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen()
{
	socket_->listen();
	channel_->enableReading();
}

void Acceptor::handleRead()
{
	InetAddress peerAddr(0);
	int connfd = socket_->accept(&peerAddr);

	if (connfd >= 0) {
		if (newConnectionCallback_) {
			newConnectionCallback_(connfd, peerAddr);
		} else {
			Socket::close(connfd);
		}
	}
}
