#pragma once
#include"common.hpp"
#include <algorithm>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
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
		void doPendingFunctors();
		void queueInLoop(const std::function<void()>& cb);
		bool isINLoopThread()const{return threadId_==std::this_thread::get_id();}
		static EventLoop* getEventLoopOfCurrentThread();
		void updateChannel(Channel* ch);
		void quit();
		void runInLoop(const std::function<void()>&cb );
		void wakeup();
	private:
		void abortNotInLoopThread();
		std::atomic_bool looping_;
		const std::thread::id threadId_;
		std::unique_ptr<Poller> poller_;
		std::atomic_bool quit_;
		std::atomic_bool callingPendingFunctors_;
		Timestamp pollReturnTime_;
		std::unique_ptr<TimerQueue> timerQueue_;
		int wakeupFd_;
		std::unique_ptr<Channel> wakeupChannel_;
		std::vector<std::function<void()>> pendingFunctors_;
		std::mutex mutex_;

		ChannelList activeChannels_;

	};
}
