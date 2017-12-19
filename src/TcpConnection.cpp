#include "TcpConnection.h"
#include "EventLoop.h"
#include <unistd.h>

TcpConnection::TcpConnection(EventLoop* loop,  const std::string& name, int sockfd, const InetAddress& peerAddr) :
	loop_(loop),
	name_(name),
	state_(kConnecting),
	socket_(new Socket(sockfd)),
	channel_(new Channel(loop, sockfd)),
	peerAddr_(peerAddr)
{
	channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
	channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
}

void TcpConnection::connectEstablished()
{
	state_ = kConnected;
	channel_->enableReading();
	connectionCallback_(shared_from_this()); // 回调 连接建立完成
}
void TcpConnection::handleRead() // 消息到达时 回调这个函数
{
	int savedErrno = 0;
	ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
	if (n > 0) {
		messageCallback_(shared_from_this(), &inputBuffer_); // 回调 消息已经到达
	} else if (n == 0) {
		handleClose(); // 关闭连接
	} else {
		printf("消息接收出错\n");
	}
}

void TcpConnection::handleClose()
{
	state_ = kDisconnected;
	channel_->disableAll();
	TcpConnectionPtr self(shared_from_this());
	connectionCallback_(self); // 回调， 连接关闭
	closeCallback_(self); // 回调， 连接关闭， 从 TcpServer 中移除
}




void TcpConnection::send(const std::string& message)
{
	send(message.data(), message.size());
}
void TcpConnection::send(const char* data, int len)
{
	size_t remaining = len;
	ssize_t nwrote = 0;
	if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
		nwrote = ::write(channel_->fd(), data, len);
		if (nwrote >= 0) {
			remaining = len - nwrote;
      		if (remaining == 0 &&  writeCompleteCallback_) {
				writeCompleteCallback_(shared_from_this());
			}
		}
	}
	if (static_cast<size_t>(nwrote) < len) {
		outputBuffer_.append(data+nwrote, len-nwrote);
		if (!channel_->isWriting()) {
			channel_->enableWriting();
		}
	}
}

void TcpConnection::handleWrite()
{
	if (channel_->isWriting()) {
		ssize_t n = ::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
		if (n > 0) {
			outputBuffer_.retrieve(n);
			if (outputBuffer_.readableBytes() == 0) {
				channel_->disableWriting();
				if (writeCompleteCallback_) {
					writeCompleteCallback_(shared_from_this());
				}
				if (state_ == kDisconnecting) {
					socket_->shutdownWrite();
				}
			}
		}
	}
}

void TcpConnection::shutdown()
{
	if (state_ == kConnected) {
		state_ = kDisconnecting;
		if (!channel_->isWriting()) {
			socket_->shutdownWrite();
		}
	}
}