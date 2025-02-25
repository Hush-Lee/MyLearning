#include "acceptor.hpp"
#include "common.hpp"
#include <cerrno>
#include <sys/socket.h>
#include <functional>
muduo::Acceptor::Acceptor(EventLoop*loop,const InetAddress&listenAddr):loop_(loop),acceptSocket_(sockets::createNonblockingOrDie()),acceprChannel_(loop,acceptSocket_.fd()),listenning_(false){
	acceptSocket_.setReuseAddr(true);
	acceptSocket_.bindAddress(listenAddr);
	acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead(),this));
}

void muduo::Acceptor::listen(){
	loop_->assertInLoopThread();
	listenning_=true;
	acceptSocket_.listen();
	acceptChannel_.enableReading();
}


int muduo::Sockets::createNonblockingOrDie(){
	int sockfd = ::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_TCP);
	if(sockfd < 0){
		LOG_SYSFATAL<<"sockets::createNonblockingOrDie";
	}
	return sockfd;
}

void muduo::Acceptor::handleRead(){
	loop_->assertInLoopThread();
	InetAddress peerAddr(0);
	int connfd = acceptSocket_.accept(&peerAddr);
	if(connfd>=0){
		if(newConnectionCallback_){
			newConnectionCallback_(connfd,peerAddr);
		}else{
			Sockets::close(connfd);
		}		
	}
}


int muduo::Sockets::accept(int sockfd,struct sockaddr_in* addr){
	socklen_t addrlen=sizeof(*addr);
#if VALGRIND
	int connfd=::accept(sockfd,sockaddr_cast(addr),&addrlen);
#else
	int connfd=::accept4(sockfd,sockaddr_cast(addr),&addrlen,SOCK_NONBLOCK|SOCK_CLOEXEC);
#endif
	if(connfd<0){
		int  saveErrno=errno;
		LOG_SYSERR<<"Socket::accept";
		switch (saveErrno) {
		
		}
	}
	return connfd;
}
