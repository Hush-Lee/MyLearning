#include "eventLoop.hpp"
#include "common.hpp"
#include <cassert>
#include <poll.h>
__thread muduo::EventLoop* t_loopInThisThread =0;
//EventLoop由IO线程创建,由这个线程运行loop(),生命周期和线程一样,不需要额外关注;
muduo::EventLoop::EventLoop():looping_(false),threadId_(std::this_thread::get_id()){
	LOG_TRACE<<"EventLoop created"<<this<<"in thread "<< threadId_;
	if(t_loopInThisThread){
		LOG_FATAL<<"Another EventLoop"<<t_loopInThisThread<<" exists in this thread "<<threadId_;
	}else {
		t_loopInThisThread = this;
	}
}

muduo::EventLoop::~EventLoop(){
	assert(!looping_);
	t_loopInThisThread=nullptr;
}

muduo::EventLoop* muduo::EventLoop::getEventLoopOfCurrentThread(){
	return t_loopInThisThread;
}

void muduo::EventLoop::loop(){
	assert(!looping_);
	assertInLoopThread();
	looping_=true;
	quit_=false;
	while(!quit_){
		activeChannels_.clear();
		poller_->poll(kPollTimeMs,&activeChannels_);
		for(auto it=activeChannels_.begin();it!=activeChannels_.end();++it){
			(*it)->handleEvent();
		}
	}
	LOG_TRACE<<"EventLoop "<<this<<" stop looping";
	looping_=false;
}
void  muduo::EventLoop::quit(){
	quit_=true;
}

void muduo::EventLoop::updateChannel(muduo::Channel * channel){
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}
