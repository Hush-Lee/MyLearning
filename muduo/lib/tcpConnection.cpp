#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include "tcpConnection.hpp"
#include "common.hpp"
void TcpConnection::handleRead(){
	char buf[65535];
	ssize_t n =::read(channel_->fd(),buf,sizeof(buf));
	messageCallback_(shared_from_this(),buf,n);
}

void TcpConnection::handleRead(){
	char buf[65535];
	ssize_t n=::read(channel_->fd(), buf,sizeof(buf));
	if(n>0){
		messageCallback_(shared_from_this(),buf,n);
	}else if(n==0){
		handleClose();
	}else{
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


void  TcpConnection::connectDestroyed(){
	loop_->assertInLoopThread();
	assert(state_==kConnected);
	setState(kDisconnected);
	channel_->disableAll();
	connectionCallback_(shared_from_this());
	loop_->removeChannel(std::get_pointer_safety(channel_));
}
