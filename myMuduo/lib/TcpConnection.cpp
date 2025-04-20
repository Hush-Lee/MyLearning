#include "TcpConnection.hpp"
#include "Buffer.hpp"
#include "Callback.hpp"
#include "InetAddress.hpp"
#include "Logging.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"
#include "Socket.hpp"
#include "SocketOps.hpp"

void defalutConnectionCallback(const TcpConnectionPtr& conn){
	LOG_TRACE<<conn->localAddr().toPortIp()<<" -> "
		<<conn->peerAddr().toPortIp()<<" is "
		<<(conn->connected()?"Up":"Down");
}
void defalutMessageCallback(const TcpConnectionPtr&,Buffer* buf,Timestamp){
	buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop*loop,const std::string& nameArg,int sockfd,const InetAddress& localAddr,const InetAddress& peerAddr):
	loop_(CHECK_NOTNULL(loop)),
	name_(nameArg),
	state_(kConnecting),
	reading_(true),
	socket_(new Socket(sockfd)),


