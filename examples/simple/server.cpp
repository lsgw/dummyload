#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include <string.h>


using namespace std;

void onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected()) {
		printf("onConnection(): new connection [%s] from %s\n", conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
	} else {
		printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
	}
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
	string message = buf->retrieveAllAsString();
	if (message == "quit\n") {
		conn->shutdown();
	} else if (message == "data\n") {
		string s(1000000, 'a');

		conn->send(s);
		conn->send("\nfinshed\n");
		conn->shutdown();
	} else if (message == "sq\n") {
		printf("over\n");
		conn->getLoop()->quit();
	}
	printf("onMessage(): received %zd bytes from connection [%s]------>%s\n", message.size(), conn->name().c_str(), message.c_str());
}

void onWriteCompleteCallback(const TcpConnectionPtr& conn)
{
	printf("connection [%s] writeCompleteCallback\n", conn->name().c_str());
}

int main(int argc, char *argv[])
{
	if (argc != 3) return -1;
	
	
	EventLoop loop;

	TcpServer server(&loop, InetAddress(argv[1], atoi(argv[2])));
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setWriteCompleteCallback(onWriteCompleteCallback);
	server.start();

	loop.loop();

	return 0;
}