#pragma once
#include"common.hpp"
#include <atomic>
#include <memory>
#include <thread>
#include <vector>
namespace muduo {
	class EventLoop:noncopyable{
	public:
		typedef std::vector<Channel*> ChannelList;
		EventLoop();
		~EventLoop();
		void loop();
		void assertInLoopThread(){
			if(!isINLoopThread()){
				abortNotInLoopThread();
			}
		}
		bool isINLoopThread()const{return threadId_==std::this_thread::get_id();}
		static EventLoop* getEventLoopOfCurrentThread();
		void updateChannel(Channel* ch);
		void quit();
	private:
		void abortNotInLoopThread();

		std::atomic_bool looping_;
		const std::thread::id threadId_;
		std::unique_ptr<Poller> poller_;
		std::atomic_bool quit_;
		ChannelList activeChannels_;

	};
}
