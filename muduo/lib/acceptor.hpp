#pragma once 
#include <functional>

#include "common.hpp"
namespace muduo {
	class Sockets{
		public:
			int createNonblockingOrDie();
			int accept(int sockfd,sockaddr_in*addr);
	};
	class Acceptor:muduo::noncopyable{
		public:
			typedef std::function<void(int sockfd,const InetAddress&)>  NewConnectionCallback;
			Acceptor(EventLoop*loop,const InetAddress& listenAddr);
			void setNewConnectionCallback(const NewConnectionCallback&cb){
				newConnectionCallback_=cb;
			}
			bool listening() const{return listenning_;}
			void listen();
		private:
			void handleRead();
			EventLoop* loop_;
			Socket acceptSocket_;
			Channel acceptChannel_;
			NewConnectionCallback newConnectionCallback_;
			bool listenning_;
	};
}
