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
}

