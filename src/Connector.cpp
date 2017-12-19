#include "Connector.h"
#include "EventLoop.h"
#include "Socket.h"
#include <errno.h>

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr) :
	loop_(loop),
	serverAddr_(serverAddr),
	state_(kDisconnected)
{

}

void Connector::start()
{
	int fd = Socket::createSocketNonBlocking();
	int rt = Socket::connect(fd, serverAddr_.get());
	int savedErrno = (rt==0)? 0 : errno;

	switch (savedErrno) {
	case 0:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		connecting(fd);
		break;

	case EAGAIN:
	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:

	case EACCES:
	case EPERM:
	case EAFNOSUPPORT:
	case EALREADY:
	case EBADF:
	case EFAULT:
	case ENOTSOCK:
		Socket::close(fd);
		loop_->quit();
		break;

	default:
		Socket::close(fd);
		loop_->quit();
		break;
	}
}
void Connector::connecting(int fd)
{
	state_ = kConnecting;
	channel_.reset(new Channel(loop_, fd));
	channel_->setWriteCallback(std::bind(&Connector::handleWrite, this)); // FIXME: unsafe
	channel_->setErrorCallback(std::bind(&Connector::handleError, this)); // FIXME: unsafe
	channel_->enableWriting();
}

void Connector::handleWrite()
{
	int fd = channel_->fd();
	channel_->disableAll();
	// Can't reset channel_ here, because we are inside Channel::handleEvent
	// channel_.reset();
	int err = Socket::getSocketError(fd);
	if (err) {
		state_ = kDisconnected;
		Socket::close(fd);
		loop_->quit();
	} else {
		state_ = kConnected;
		newConnectionCallback_(fd);
	}
}
void Connector::handleError()
{
	int fd = channel_->fd();
	channel_->disableAll();
	// Can't reset channel_ here, because we are inside Channel::handleEvent
	// channel_.reset();
	int err = Socket::getSocketError(fd);

	state_ = kDisconnected;
	Socket::close(fd);
	loop_->quit();
}