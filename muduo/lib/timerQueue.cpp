#include "timerQueue.hpp"
#include "common.hpp"
#include <cassert>
#include <cstdint>
#include <vector>
#include <functional>
std::vector<Entry>muduo::TimerQueue::getExpired(muduo::Timestamp now){
	std::vector<Entry> expired;
	Entry sentry =std::make_pair(now,reinterpret_cast<Timer* >(UINTPTR_MAX));
	auto it=std::lower_bound(timers_.begin(),timers_.end(),sentry);
	assert(it ==timers_.end()||now <it->first);
	std::copy(timers_.begin(),it,std::back_inserter(expired));
	timers_.erase(timers_.begin(),it);
	return expired;
}

muduo::TimerId muduo::TimerQueue::addTimer(const TimerCallback&cb ,Timestamp when,double interval){
	Timer* timer =new Timer(cb,when,interval);
	loop_->runInLoop(std::bind(&muduo::TimerQueue::addTimerInLoop,this,timer));
	return muduo::TimerId(timer);
}
void muduo::TimerQueue::addTimerInLoop(muduo::Timer* timer){
	loop_->assertInLoopThread();
	bool earliestChanged = insert(timer);
	if(earliestChanged){
		resetTimerfd(timerfd_,timer->expiration());
	}
}
