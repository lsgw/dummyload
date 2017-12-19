#include "InetAddress.h"
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
InetAddress::InetAddress(const struct sockaddr_in& addr) :
	address_(addr)
{

}
InetAddress::InetAddress(uint16_t port)
{
	bzero(&address_, sizeof(address_));
	address_.sin_family = AF_INET;
	address_.sin_addr.s_addr = INADDR_ANY;
	address_.sin_port = htons(port);
}
InetAddress::InetAddress(const string& ip, uint16_t port)
{
	bzero(&address_, sizeof(address_));
	address_.sin_family = AF_INET;
	address_.sin_addr.s_addr = inet_addr(ip.c_str());
	address_.sin_port = htons(port);
}

string InetAddress::toIpPort() const
{
	char buf[32] = {'\0'};
	char host[INET_ADDRSTRLEN] = {'\0'};
	uint16_t port = 0;

	::inet_ntop(AF_INET, &address_.sin_addr, host, sizeof(host));
	port = ::ntohs(address_.sin_port);

	snprintf(buf, sizeof(buf), "%s : %u", host, port);

	return buf;
}

const struct sockaddr_in& InetAddress::get() const
{
	return address_;
}

void InetAddress::set(const struct sockaddr_in& addr)
{
	address_ = addr;
}

InetAddress InetAddress::getLocalAddr(int sockfd)
{
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	if (::getsockname(sockfd, (sockaddr*)&localaddr, &addrlen) < 0) {
		
	}
	return localaddr;
}


