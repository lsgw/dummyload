#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <netinet/in.h>
#include <string>

class InetAddress {
public:
	InetAddress(const std::string& ip, uint16_t port);
	InetAddress(const struct sockaddr_in& addr);
	explicit InetAddress(uint16_t port);
	

	std::string toIpPort() const;
	const struct sockaddr_in& get() const;
	void set(const struct sockaddr_in& addr);
	static InetAddress getLocalAddr(int fd);
private:
	struct sockaddr_in address_;
};


#endif