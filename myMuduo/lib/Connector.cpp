#include "Connector.hpp"
#include "EventLoop.hpp"
#include "InetAddress.hpp"
#include "SocketOps.hpp"
#include "Logging.hpp"
#include "Channel.hpp"
#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <sys/socket.h>
#include <memory>
const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop*loop,const InetAddress& serverAddr):loop_(loop),
	serverAddr_(serverAddr),
	connect_(false),
	state_(kDisConnected),
	retryDelayMs_(kInitRetryDelayMs){
		LOG_DEBUG<<"ctor["<<this<<"]";
}
Connector::~Connector(){
	LOG_DEBUG<<"dror["<<this<<"]";
	assert(!channel_);
}

void Connector::start(){
	connect_=false;
	loop_->queueInLoop(std::bind(&Connector::startInLoop,this));
}
void Connector::startInLoop(){
	loop_->assertInLoopThread();
	assert(state_==kDisConnected);//如果连接建立了,那么就停止
	if(connect_){//还没连接就继续继续连接
		connect();
	}else{
		LOG_DEBUG<<"do not connect";
	}
}

void Connector::stop(){
	connect_=false;
	loop_->queueInLoop(std::bind(&Connector::stopInLoop,this));
}

void Connector::stopInLoop(){
	loop_->assertInLoopThread();
	if(state_==kConnecting){
		setState(kDisConnected);
		int sockfd=removeAndResetChannel();
		retry(sockfd);
	}
}

void Connector::connect(){
	int sockfd=sockets::createNonblockingOrDie(serverAddr_.family());
	int ret=sockets::connect(sockfd,serverAddr_.getSockAddr);
	int savedError=(ret==0)?0:errno;
	switch (savedError) {
		case 0:
		case EINPROGRESS:
		case EINTR:
		case EISCONN:
			connecting(sockfd);
			break;
		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ECONNREFUSED:
		case ENETUNREACH:
			retry(sockfd);
			break;
		case EACCES:
		case EPERM:
		case EAFNOSUPPORT:
		case EALREADY:
		case EBADF:
		case EFAULT:
		case ENOTSOCK:
			LOG_SYSERR<<"connect error in Connector::startLoop "<<savedError;
			sockets::close(sockfd);
			break;
		default:
			LOG_SYSERR<<"connect error in Connector::startLoop "<<savedError;
			sockets::close(sockfd);
			break;

	}
}


void Connector::restart(){
	loop_->assertInLoopThread();
	setState(kDisConnected);
	retryDelayMs_=kInitRetryDelayMs;
	connect_=true;
	startInLoop();
}

void Connector::connecting(int sockfd){
	setState(kConnecting);
	assert(!channel_);
	channel_.reset(new Channel(loop_,sockfd));
	channel_->setWriteCallback(std::bind(&Connector::handleWrite,this));
	channel_->setErrorCallback(std::bind(&Connector::handleError,this));
	channel_->enableWriting();
}

int Connector::removeAndResetChannel(){
	channel_->disableAll();
	channel_->remove();
	int sockfd=channel_->fd();
	loop_->queueInLoop(std::bind(&Connector::resetChannel,this));
	return sockfd;;
}

void Connector::resetChannel(){
	channel_.reset();
}

void Connector::handleWrite(){
	LOG_TRACE<<"Connector::handleWrite "<<state_;
	if(state_==kConnecting){
		int sockfd=removeAndResetChannel();
		int err=sockets::getSocketError(sockfd);
		if(err){
			LOG_WARN<<"Connector::handleWrite - SO_ERROR = "<<err<<strerror(err);
			retry(sockfd);
		}else if(sockets::isSlefConnect(sockfd)){
			LOG_WARN<<"Connector::handleWrite - Self connect";
			retry(sockfd);
		}else{
			setState(kConnected);
			if(connect_){
				newConnectionCallback_(sockfd);
			}else{
				sockets::close(sockfd);
			}
		}
	}else{
		assert(state_==kDisConnected);
	}
		
}
void Connector::handleError(){
	LOG_ERROR<<"Connector::handleError state = "<<state_;
	if(state_==kConnecting){
		int sockfd=removeAndResetChannel();
		int err=sockets::getSocketError(sockfd);
		LOG_TRACE<<"SO_ERROR = "<<err<<strerror(err);
		retry(sockfd);
	}
}

void Connector::retry(int sockfd){
	sockets::close(sockfd);
	setState(kDisConnected);
	if(connect_){
		LOG_INFO<<"Connector::retry - Retrying to "<<serverAddr_.toPortIp()<<" in "<<retryDelayMs_<<" milliseconds.";
		loop_->runAfter(retryDelayMs_/1000.0, 
				std::bind(&Connector::startInLoop ,shared_from_this()));
		retryDelayMs_=std::min(retryDelayMs_*2,kMaxRetryDelayMs);
	}else{
		LOG_DEBUG<<"do not connect";
	}
}
