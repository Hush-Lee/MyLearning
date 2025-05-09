#pragma once
#include "Callback.hpp"
#include "InetAddress.hpp"
#include "Buffer.hpp"
#include "Classes.hpp"
#include <any>
#include <functional>
#include <memory>
#include <string_view>
class EventLoop;
class Channel;
class Socket;
class Timestamp;
class TcpConnection:noncopyable,
	public std::enable_shared_from_this<TcpConnection>{
public:
	TcpConnection(EventLoop*loop,const std::string& name,int sockfd,const InetAddress&localAddr,const InetAddress&peerAddr);
	~TcpConnection();
	
	EventLoop* getLoop()const {return loop_;}
	const std::string& name()const {return name_;}
	const InetAddress& localAddr(){return localAddr_;}
	const InetAddress& peerAddr(){return peerAddr_;}
	bool connected()const{return state_== kConnected;}
	bool disConnected()const{return state_==kDisconnected;}

	bool getTcpInfo(struct tcp_info* )const;
	std::string getTcpInfoString()const;

	void send(const void *message,int len);
	void send(const std::string_view& message);

	void send(Buffer*message);
	void shutdown();
	void forceClose();
	void forceCloseWithDelay(double seconds);
	void setTcpNoDelay(bool on);

	void startRead();
	void stopRead();
	bool isReading()const {return reading_;}

	void setContext(const std::any& context){context_=context;}
	const std::any getContext()const{return context_;}
	const std::any* getMutableContext(){
		return &context_;
	}

	void setConnectionCallback(ConnectionCallback&cb){connectionCallback_=cb;}
	void setMessageCallback(MessageCallback& cb){messageCallback_=cb;}
	void setWriteCompleteCallback(WriteCompleteCallback& cb){writeCompleteCallback_=cb;}
	void setHightWaterMarkCallback(HighWaterMarkCallback&cb){highWaterMarkCallback_=cb;}

	Buffer* inputBuffer(){
		return &inputBuffer_;
	}
	Buffer* outputBuffer(){
		return &outputBuffer_;
	}
	void setCloseCallback(const CloseCallback&cb){
		closeCallback_=cb;
	}
	void connectEstablished();

	void connectDestroyed();



private:
	enum StateE{kDisconnected,kConnecting,kConnected,kDisconnecting};
	void handleRead(Timestamp reciveTime);
	void handleWrite();
	void handleClose();
	void handleError();

	void sendInLoop(std::string_view message);
	void sendInLoop(const void * message,size_t len);
	void shutdownInLoop();
	void forceCloseInLoop();
	void setState(StateE s){state_=s;}
	const char* stateToString()const;
	void startReadInLoop();
	void stopReadInLoop();

	EventLoop* loop_;
	const std::string name_;
	StateE state_;
	bool reading_;
	std::unique_ptr<Socket>socket_;
	std::unique_ptr<Channel>channel_;
	const InetAddress localAddr_;
	const InetAddress peerAddr_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	HighWaterMarkCallback highWaterMarkCallback_;
	CloseCallback closeCallback_;
	size_t hightWaterMark_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;

	std::any context_;

	using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
	};
