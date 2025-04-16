#include "EventLoopThread.hpp"
#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,const std::string& name)
	:loop_(nullptr),
	exiting_(false),
	thread_(std::bind(&EventLoopThread::threadFunc,this),name),
	mutex_(),
	cond_(),
	callback_(cb){
	}

EventLoopThread::~EventLoopThread(){
	exiting_=true;
	if(loop_!=nullptr){
		loop_->quit();
		thread_.join();
	}
}
EventLoop* EventLoopThread::startLoop(){
	assert(!thread_.started());
	thread_.start();
	EventLoop*loop=nullptr;
	std::unique_lock<std::mutex>ul(mutex_);
	cond_.wait(ul,loop==nullptr);
	loop_=loop;
	return loop;
}


void EventLoopThread::threadFunc(){
	EventLoop loop;
	if(callback_){
		callback_(&loop);
	}
	{
		std::lock_guard<std::mutex>lg(mutex_);
		loop_=&loop;
		cond_.notify_all();
	}
	loop.loop();
	std::lock_guard<std::mutex>lg(mutex_);
	loop=nullptr;
}
