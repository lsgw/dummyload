set(src 
	Acceptor.cpp
	Buffer.cpp
    Channel.cpp
	Connector.cpp
	EPoller.cpp
	EventLoop.cpp
	InetAddress.cpp
	Socket.cpp
	TcpClient.cpp
	TcpConnection.cpp
	TcpServer.cpp
	Timestamp.cpp
	Timer.cpp
	TimerQueue.cpp
)

#file(GLOB_RECURSE src "./*.cpp")  #递归搜索子目录
#aux_source_directory(./ src)

add_library(net STATIC ${src})
target_link_libraries(net pthread)
set_target_properties(net PROPERTIES COMPILE_FLAGS "-std=c++1y")