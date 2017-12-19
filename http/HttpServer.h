#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "noncopyable.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "HttpContext.h"
#include <functional>

class EventLoop;
class HttpResponse;

class HttpServer : noncopyable {
	using HttpCallback = std::function<void(const HttpRequest&,HttpResponse*)>;
public:
	HttpServer(EventLoop* loop, const InetAddress& listenAddr);

	EventLoop* getLoop() const { return server_.getLoop(); }
	void setHttpCallback(const HttpCallback& cb) { httpCallback_ = cb; }
	void start();

private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, Buffer* buf);
	void onRequest(const TcpConnectionPtr&, const HttpRequest&);


	TcpServer server_;
	HttpCallback httpCallback_;
};

#endif