#include "EventLoop.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "TcpClient.h"
#include <stdio.h>
#include <memory>

void onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected()) {
		printf("onConnection(): new connection [%s] from %s\n", conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
		conn->send("data\n");
	} else {
		printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
		conn->getLoop()->quit();
	}
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
	printf("onMessage(): received %zd bytes from connection [%s]", buf->readableBytes(), conn->name().c_str());
	printf("onMessage(): [%s]\n", buf->retrieveAllAsString().c_str());
}

int main(int argc, char* argv[])
{
	if (argc != 3) return -1;
	
	EventLoop loop;
	InetAddress serverAddr(argv[1], atoi(argv[2]));

	TcpClient client(&loop, serverAddr);
	client.setConnectionCallback(onConnection);
	client.setMessageCallback(onMessage);
	client.start();

	loop.loop();
}