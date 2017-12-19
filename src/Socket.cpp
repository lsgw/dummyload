#include "Socket.h"
#include "InetAddress.h"
#include <fcntl.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

Socket::Socket(int fd) : fd_(fd)
{
}
Socket::~Socket()
{
	if (::close(fd_) < 0) {
		::abort();
	}
}


void Socket::bind(const InetAddress& addr)
{
	auto& a = addr.get();
	int ret = ::bind(fd_, (struct sockaddr*)&a, sizeof(struct sockaddr_in));
	if (ret < 0) {
		::abort();
	}
}
void Socket::listen()
{
	int ret = ::listen(fd_, 5);
	if (ret < 0) {
		::abort();
	}
}

int Socket::accept(InetAddress* peerAddr)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, sizeof(addr));

	int connfd = ::accept(fd_, (struct sockaddr*)&addr, &len);
	if (connfd >= 0) {
		setNonBlocking(connfd);
		peerAddr->set(addr);
	}
	return connfd;
}

void Socket::shutdownWrite()
{
	if (::shutdown(fd_, SHUT_WR) < 0) {
	 	::abort();
	}
}






void Socket::setNonBlocking(int fd)
{
	// non-block
	int flags = ::fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	::fcntl(fd, F_SETFL, flags);
}

int Socket::createSocketNonBlocking()
{
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		::abort();
	}
	setNonBlocking(fd);

	return fd;
}

int Socket::connect(int fd, const struct sockaddr_in &addr)
{
	return ::connect(fd, (struct sockaddr*)&addr, sizeof(addr));
}

void Socket::close(int fd)
{
	if (::close(fd) < 0) {
		
	}
	
}


int Socket::getSocketError(int sockfd)
{
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof optval);

  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
  {
    return errno;
  }
  else
  {
    return optval;
  }
}