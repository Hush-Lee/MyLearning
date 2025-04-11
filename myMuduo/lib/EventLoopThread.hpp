#pragma once
#include "Classes.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
class EventLoop

class EventLoopThread:noncopyable{
	public:
		using ThreadInitCallback=std::function<void(EventLoop*)>;
		EventLoopThread(const ThreadInitCallback& cb=ThreadInitCallback(),const string& name=string());
		~EventLoopThread();
		EventLoop* startLoop();
	private:
		void threadFunc();

		EventLoop* loop_ GUARDED_BY(mutex_);
		std::mutex mutex_;
		std::thread thread_;
		std::condition_variable cond_ GUARDED_BY(mutex_);
		ThreadInitCallback callback_;
		bool exiting_;

};
