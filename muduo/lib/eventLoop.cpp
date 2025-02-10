#include "eventLoop.hpp"
#include <cassert>
#include <poll.h>
__thread EventLoop* t_loopInThisThread =0;
//EventLoop由IO线程创建,由这个线程运行loop(),生命周期和线程一样,不需要额外关注;
EventLoop::EventLoop():looping_(false),threadID_(std::this_thread::get_id()){
	LOG_TRACE<<"EventLoop created"<<this<<"in thread "<< threadID_;
	if(t_loopInThisThread){
		LOG_FATAL<<"Another EventLoop"<<t_loopInThisThread<<" exists in this thread "<<threadID_;
	}else {
		t_loopInThisThread = this;
	}
}

EventLoop::~EventLoop(){
	assert(!looping_);
	t_loopInThisThread=nullptr;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread(){
	return t_loopInThisThread;
}

void EventLoop::loop(){
	assert(!looping_);
	assertInLoopThread();
	looping_=true;
	::poll(nullptr,0,5*1000);
	LOG_TRACE<<"EventLoop "<<this<<" stop looping";
	looping_=false;
}

