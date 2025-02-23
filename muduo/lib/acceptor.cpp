#include "acceptor.hpp"
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
