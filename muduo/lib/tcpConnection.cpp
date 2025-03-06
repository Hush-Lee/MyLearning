#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include "tcpConnection.hpp"
#include "common.hpp"
void TcpConnection::handleRead(){
	int savedErrno=0;
	ssize_t n=inputBuffer_.readFd(channel_->fd(),&savedErrno);
	if(n>0){
		messageCallback_(shared_from_this(),buf,n);
	}else if(n==0){
		handleClose();
	}else{
		errno = savedErrno;
		LOG_SYSERR<<"TcpConnection::handleRead";
		handleError();
	}
}

void TcpConnection::handleRead(){
	loop_->assertInLoopThread();
	LOG_TRACE<<"TcpConnection::handleClose state  = "<<state_;
	assert(state_==kConnected);
	channel_->disableAll();
	closeCallback_(shared_from_this());
	closeCallback_(shared_from_this());
}
void TcpConnection::handleError(){
	int err=muduo::Socket::getSocketError(channel_->fd());
	LOG_ERROR<<"TcpConnection::handleError ["<<name_<<"] - SO_ERROR = "<<err<<" "<<strerror_tl(err);
}
