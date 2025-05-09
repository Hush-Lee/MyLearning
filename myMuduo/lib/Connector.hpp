#pragma once
#include "Classes.hpp"
#include "InetAddress.hpp"
#include <memory>
#include <functional>

class Channel;
class InetAddress;
class EventLoop;
class Connector : noncopyable,
		public std::enable_shared_from_this<Connector>{
public:
	using NewConnectionCallback=std::function<void(int sockfd)>;
	Connector(EventLoop*loop,const InetAddress&serverAddr);
	~Connector();
	void setNewConnectionCallback(const NewConnectionCallback& cb){
		newConnectionCallback_=std::move(cb);
	}
	void start();
	void restart();
	void stop();
	const InetAddress& serverAddress()const{return serverAddr_;}
private:
	enum States{kDisConnected,kConnecting,kConnected};
	static const int kMaxRetryDelayMs=30*1000;
	static const int kInitRetryDelayMs=500;
	void setState(States s){state_=s;}
	void startInLoop();
	void stopInLoop();
	void connect();
	void connecting(int sockfd);
	void handleWrite();
	void handleError();
	void retry(int sockfd);
	int removeAndResetChannel();
	void resetChannel();

	EventLoop *loop_;
	InetAddress serverAddr_;
	bool connect_;
	States state_;
	std::unique_ptr<Channel>channel_;
	NewConnectionCallback newConnectionCallback_;
	int retryDelayMs_;

};
