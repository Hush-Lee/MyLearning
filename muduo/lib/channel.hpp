#pragma once
#include "common.hpp"
#include <functional>
class EventLoop;
namespace  {
	class Channel:muduo::noncopyable{
	public:
		typedef std::function<void()> EventCallback;
		typedef std::function<void(muduo::Timestamp)> ReadEventCallback;
		Channel(EventLoop* loop,int fd);
		~Channel();
		void handleEvent();
		void setReadCallback(const ReadEventCallback& cb){
			readCallback_=cb;
		}
		void setWriteCallback(const EventCallback& cb){
			writeCallback_=cb;
		}
		void getErrorCallback(const EventCallback& cb){
			errorCallback_=cb;
		}

		int fd()const{return fd_;}
		int events()const{return events_;}
		void set_revents(int revt){revents_=revt;}
		bool isNoneEvent()const{return events_==kNoneEvent;}
		void enableReading(){
			events_|=kReadEvent;
			update();
		}
		int index(){return index_;}
		void set_index(int idx){index_=idx;}

		EventLoop* ownerLoop(){return loop_;}

		
	private:
		void update();
		static const int kNoneEvent;
		static const int kReadEvent;
		static const int kWriteEvent;

		EventLoop* loop_;
		const int fd_;
		int events_;
		int revents_;
		int index_;
		bool eventHandling_;
		ReadEventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback errorCallback_;

	};
}
