#include "HttpServer.h"
#include "HttpResponse.h"
#include "EventLoop.h"
#include <iostream>

using namespace std;
using namespace std::placeholders;

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
	resp->setStatusCode(HttpResponse::k404NotFound);
	resp->setStatusMessage("Not Found");
	resp->setBody("404 Not Found");
	resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop* loop, const InetAddress& listenAddr) :
	server_(loop, listenAddr),
	httpCallback_(defaultHttpCallback)
{
	server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1));
	server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2));
}

void HttpServer::start()
{
	server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected()) {
		printf("onConnection(): connection [%s] is up   : %s\n", conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
	} else {
		printf("onConnection(): connection [%s] is down : %s\n", conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
	}
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
	HttpContext context;

	if (!context.parseRequest(buf)) {
		conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
		conn->shutdown();
	}
	if (context.gotAll()) {
		onRequest(conn, context.request());
	}
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
	const string& connection = req.header("Connection");
	bool close = connection == "close" || (req.version() == HttpRequest::kHttp10 && connection != "Keep-Alive");
	
	Buffer buf;
	HttpResponse response(close);
	httpCallback_(req, &response);
	response.appendToBuffer(&buf);

	conn->send(buf.retrieveAllAsString());

	if (response.closeConnection()) {
		conn->shutdown();
	}
}