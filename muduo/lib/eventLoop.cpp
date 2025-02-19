#include "eventLoop.hpp"
#include "common.hpp"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <mutex>
#include <poll.h>
#include <pthread.h>
#include <vector>
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
		doPendingFunctors();
	}
	LOG_TRACE<<"EventLoop "<<this<<" stop looping";
	looping_=false;
}
void  muduo::EventLoop::quit(){
	quit_=true;
	if(!isINLoopThread()){
		wakeup();
	}
}

void muduo::EventLoop::updateChannel(muduo::Channel * channel){
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}
void muduo::EventLoop::runInLoop(const std::function<void()>& cb){
	if(isINLoopThread()){
		cb();
	}else{
		queueInLoop(cb);
	}
}
void muduo::EventLoop::queueInLoop(const std::function<void()>&cb){
	{
		std::lock_guard<std::mutex>lock(mutex_);
		pendingFunctors_.push_back(cb);
	}		
	if(!isINLoopThread()||callingPendingFunctors_){
		wakeup();
	}
}
void muduo::EventLoop::doPendingFunctors(){
	std::vector<std::function<void()>> functors;
	callingPendingFunctors_=ture;
	{
		std::lock_guard<std::mutex>lock(mutex_);
		functors.swap(pendingFunctors_);
	}
	for(size_t i=0;i<functors.size();++i){
		functors[i]();
	}
	callingPendingFunctors_=false;
}
