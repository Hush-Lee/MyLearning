#include "TimerQueue.hpp"
#include "Callback.hpp"
#include "Classes.hpp"
#include "Logging.hpp"
#include "TimerId.hpp"
#include "Timer.hpp"
#include "Timestamp.hpp"
#include <cassert>
#include <cstdint>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>
int creatTiemrFd(){
	int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
	if(timerfd<0){
		LOG_SYSFATAL<<"Failed in timerfd_create";
	}
	return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when){
	int64_t microseconds=when.microSecondSinceEpoch()-Timestamp::now().microSecondSinceEpoch();
	if(microseconds<100){
		microseconds=100;
	}
	struct timespec ts;
	ts.tv_sec=static_cast<time_t>(microseconds/Timestamp::kMicrosecondsPerSecond);
	ts.tv_nsec=static_cast<long>((microseconds%Timestamp::kMicrosecondsPerSecond)*1000);
	return ts;
}

void readTimerfd(int timerfd,Timestamp now){
	uint64_t howmany;
	ssize_t n=::read(timerfd,&howmany,sizeof(howmany));
	LOG_TRACE<<"TimerQueue::handleRead()"<<howmany<<" at "<<now.toString();
	if(n!=sizeof(howmany)){
		LOG_ERROR<<"TimerQueue::handleRead() reads"<<n<<" bytes instead of 8";
	}
}

void resetTimerfd(int timerfd,Timestamp expired){
	struct itimerspec newValue;
	struct itimerspec oldValue;
	memZero(&newValue,sizeof(newValue));
	memZero(&oldValue,sizeof(oldValue));
	newValue.it_value=howMuchTimeFromNow(expired);
	int ret=::timerfd_settime(timerfd, 0,&newValue,&oldValue);
	if(ret){
		LOG_SYSERR<<"timerfd_settime";
	}
}



TimerQueue::TimerQueue(EventLoop* loop):loop_(loop),
	timerfd_(creatTiemrFd()),
	timerfdChannel_(loop,timerfd_),
	timers_(),
	callingExpiredTimer_(false){
		timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead,this));
		timerfdChannel_.enableReading();
	}
TimerQueue::~TimerQueue(){
	timerfdChannel_.disableAll();
	timerfdChannel_.remove();
	::close(timerfd_);
	for(const Entry& timer:timers_){
		delete timer.second;
	}
}


TimerId TimerQueue::addTimer(TimerCallback cb,Timestamp when,double interval){
	Timer* timer=new Timer(std::move(cb),when,interval);
	loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop,this,timer));
	return TimerId(timer,timer->sequence());
}

void TimerQueue::cancel(TimerId timerid){
	loop_->runInLoop(std::bind(&TimerQueue::cancelInLoop,this,timerid));
}

void TimerQueue::addTimerInLoop(Timer* timer){
	loop_->assertInLoopThread();
	bool earliestChanged = insert(timer);
	if(earliestChanged){
		resetTimerfd(timerfd_, timer->expiration());
	}
}

void TimerQueue::cancelInLoop(TimerId timerid){
	loop_->assertInLoopThread();
	assert(timers_.size() == activeTimers_.size());
	ActiveTimer timer(timerid.timer_,timerid.sequence_);
	auto it=activeTimers_.find(timer);
	if(it!=activeTimers_.end()){
		size_t n=timers_.erase(Entry(it->first->expiration(),it->first));
	}
}



