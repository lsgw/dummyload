#ifndef SOCKET_H
#define SOCKET_H

#include "noncopyable.h"
class InetAddress;
struct sockadd_in;

class Socket : noncopyable {
public:
	explicit Socket(int fd);
	~Socket();
	int fd() const { return fd_; }

	void bind(const InetAddress& addr);
	void listen();
	int  accept(InetAddress* peerAddr);

	void shutdownWrite();
	
private:
	int fd_;

public:
	static void setNonBlocking(int fd);
	static int  createSocketNonBlocking();
	static int  connect(int sockfd, const struct sockaddr_in& addr);
	static void close(int fd);
	static int  getSocketError(int fd);
};

#endif