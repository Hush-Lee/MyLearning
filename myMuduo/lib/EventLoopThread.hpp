#pragma once
#include "Classes.hpp"
#include "Thread.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
class EventLoop

class EventLoopThread:noncopyable{
	public:
		using ThreadInitCallback=std::function<void(EventLoop*)>;
		EventLoopThread(const ThreadInitCallback& cb=ThreadInitCallback(),const std::string& name=std::string());
		~EventLoopThread();
		EventLoop* startLoop();
	private:
		void threadFunc();

		EventLoop* loop_ GUARDED_BY(mutex_);
		std::mutex mutex_;
		Thread thread_;
		std::condition_variable cond_ GUARDED_BY(mutex_);
		ThreadInitCallback callback_;
		bool exiting_;

};
