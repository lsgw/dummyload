#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "noncopyable.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Socket.h"
#include "Buffer.h"
#include <functional>
#include <memory>
class EventLoop;
class TcpConnection;

using TcpConnectionPtr      = std::shared_ptr<TcpConnection>;
using ConnectionCallback    = std::function<void(const TcpConnectionPtr&)>;          // 连接建立时和断开时回调函数类型
using MessageCallback       = std::function<void(const TcpConnectionPtr&, Buffer*)>; // 消息到达回调函数类型
using CloseCallback         = std::function<void(const TcpConnectionPtr&)>;          // 连接关闭 从 TcpSerever 移除回调函数类型
using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;

class TcpConnection : public noncopyable, public std::enable_shared_from_this<TcpConnection> {


public:
	TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& peerAddr);

	EventLoop* getLoop() const { return loop_; }
	const std::string& name() { return name_; }
	const InetAddress& peerAddress() const { return peerAddr_; }
	bool connected() const { return state_ == kConnected; } // 判断连接是否已经建立
	void connectEstablished(); // 连接建立完成函数

	// 设置 回调函数   (三个)
	void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
	void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
	void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
	
	void send(const char* data, int n); // 发送消息函数
	void send(const std::string& message);
	void shutdown();

private:
	void handleRead();  // 数据收到处理函数
	void handleClose(); // 关闭连接处理函数
	void handleWrite();
	
	// 连接三种状态
	enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected };

	EventLoop* loop_; // 事件循环 EventLoop 对象的指针
	std::string name_; // 连接名称
	StateE state_;     // 当前连接的状态

	std::unique_ptr<Socket> socket_;  // 连接的 socket 
	std::unique_ptr<Channel> channel_;// 处理连接的事件注册和事件取消注册

	InetAddress peerAddr_; // 连接的 IP 和 Port

	ConnectionCallback connectionCallback_; //连接回调函数
	MessageCallback    messageCallback_; // 消息回调函数
	CloseCallback      closeCallback_; // 断开连接回调函数

	WriteCompleteCallback writeCompleteCallback_;

	Buffer inputBuffer_;  // 接收缓冲区
	Buffer outputBuffer_; // 发送缓冲区
};


#endif