#pragma once
#include <memory>
#include <string>
#include "common.hpp"
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr ;
class TcpConnection:muduo::noncopyable{
	public:
		void setCloseCallback(const CloseCallback& cb){
			closeCallback_ = cb;
		}
		void connectDestroyed();
		void connectEstablished();
	private:
		enum StateE{
			kConnecting,
			kConnected,
		};
		void setState(StateE s){state_=s;}
		void handleRead();
		void handleWrite();
		void handleClose();
		void handleError();
		muduo::EventLoop* loop_;
		std::string name_;
		StateE state_;
		std::unique_ptr<muduo::Socket> socket_;
		std::unique_ptr<muduo::Channel>channel_;
		InetAddress localAddr_;
		InetAddress peerAddr_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		CloseCallback closeCallback_;
}
