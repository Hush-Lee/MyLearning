#include "TcpConnection.hpp"
#include "Buffer.hpp"
#include "Callback.hpp"
#include "InetAddress.hpp"
#include "Logging.hpp"
#include "Channel.hpp"
#include "Timestamp.hpp"
#include "EventLoop.hpp"
#include "Socket.hpp"
#include "SocketOps.hpp"
#include <functional>

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
	channel_(new Channel(loop,sockfd)),
	localAddr_(localAddr),
	peerAddr_(peerAddr),
	hightWaterMark_(64*1024*1024){
		channel_->setReadCallback(std::bind(&TcpConnection::handleRead,this,_1));
		channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));
		channel_->setCloseCallback(std::bind(&TcpConnection::handleClose,this));
		channel_->setErrorCallback(std::bind(&TcpConnection::handleError,this));
		LOG_DEBUG<<"TcpConnection::ctor["<<name_<<"] at"<<this<<" fd="<<sockfd;
		socket_->setKeepAlive(true);
	}


