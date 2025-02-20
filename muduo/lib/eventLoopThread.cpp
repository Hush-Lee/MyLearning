#include "eventLoopThread.hpp"
#include "eventLoop.hpp"
#include <mutex>
muduo::EventLoop* muduo::EventLoopThread::startLoop(){
	assert(!thread_.started());
	thread_start();
	{
		std::lock_guard<std::mutex>lock(mutex_);
		while(loop_==nullptr){
			cond_.wait();
		}
	}
	return loop_;
}
void muduo::EventLoopThread::threadFunc(){
	EventLoop loop;
	{
		std::lock_guard<std::mutex>lock(mutex_);
		loop_=&loop;
		cond_.notify();
	}
	loop.loop();
}
