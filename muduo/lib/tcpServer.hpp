#pragma once 
#include "common.hpp"
#include <memory>
namespace muduo {
	class TcpServe:noncopyable{
		public:
			TcpServe(EventLoop* loop,const InetAddress& listenAddr);
			~TcpServe();	
			void setConnectionCallback(const ConnectionCallback &cb){
				connectionCallback_=cb;
			}

			void setMessageCallback(const MessageCallback& cb){
				messageCallback_=cb;
			}
		private:
			void newConnection(int sockfd,const InetAddress& peerAddr);
			typedef std::map<std::string,TcpConnectionPtr>ConnectionMap ;

			EventLoop*loop_;
			const std::string name_;
			std::unique_ptr<Acceptor> acceptor_;
			ConnectionCallback connectionCallback_;
			MessageCallback messageCallback_;
			bool started_;
			int nextConnId_;
			ConnectionMap connections_;
	}
}
