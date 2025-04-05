#include "Acceptor.hpp"

#include "EventLoop.hpp"
#include "InetAddress.hpp"
#include "Socket.hpp"
#include "SocketOps.hpp"
#include "Logging.hpp"
#include <cassert>
#include <cerrno>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

Acceptor::Acceptor(EventLoop*loop,const InetAddress&listenaddr,bool reuseport):
	loop_(loop),
	acceptSocket_(sockets::createNonblockingOrDie(listenaddr.family())),
	acceptChannel_(loop,acceptChannel_.fd()),
	listening_(false),
	idleFd_(::open("/dev/null",O_RDONLY|O_CLOEXEC)){
		assert(idleFd_>=0);
		acceptSocket_.setResueAddr(true);
		acceptSocket_.setResuePort(reuseport);
		acceptSocket_.bindAddress(listenaddr);
		acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead,this));
}

Acceptor::~Acceptor(){
	acceptChannel_.disableall();
	acceptChannel_.remove();
	::close(idleFd_);
}

void Acceptor::listen(){
	loop_->assertInLoopThread();
	listening_=true;
	acceptSocket_.listen();
	acceptChannel_.enableReading();
}

void Acceptor::handleRead(){
	loop_->assertInLoopThread();
	InetAddress peerAddr;
	int connfd=acceptSocket_.accept(peerAddr);
	if(connfd>0){
		if(newConnectionCallback_){
			newConnectionCallback_(connfd,peerAddr);
		}else{
			::close(connfd);
		}
	}else{
		LOG_SYSERR<<"in Acceptor::handleRead";
		if(errno==EMFILE){
			::close(idleFd_);
			idleFd_=::accept(acceptSocket_.fd,nullptr,nullptr);
			::close(idleFd_);
			idleFd_=::open("/dev/null",O_RDONLY|O_CLOEXEC);
		}
	}
}
