#include "EventLoop.hpp"
#include <algorithm>
#include <cassert>
#include <mutex>
#include <sys/signal.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include "Callback.hpp"
#include "CurrentThread.hpp"
#include "Logging.hpp"
#include "Poller.hpp"
#include "TimerQueue.hpp"
#include "Channel.hpp"
#include "Timestamp.hpp"
#include "SocketOps.hpp"
#include "TimerId.hpp"
namespace {

thread_local EventLoop* t_loopInThisThread=0;
const int kPollTimeMs=10000;
int creatEventfd(){
	int evtfd=::eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK );
	if(evtfd<0){
		LOG_SYSERR<<"fail in eventfd";
		abort();
	}
	return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe{
public:
	IgnoreSigPipe(){
		::signal(SIGPIPE,SIG_IGN);
	}
};
#pragma GCC diagnostic error "-Wold-style-cast"
IgnoreSigPipe initObj;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread(){
	return t_loopInThisThread; 
}

EventLoop::EventLoop():
	looping_(false),
	quit_(false),
	eventHandling_(false),
	callingPendingFunctors_(false),
	iteration_(0),
	threadId_(tid()),
	poller_(Poller::newDefaultPoller(this) ),
	timerQueue_(new TimerQueue(this)),
	wakeupFd_(creatEventfd()),
	wakeupChannel_(new Channel(this,wakeupFd_)),
	currentActivceChannel_(nullptr)
{
	LOG_DEBUG<<"EventLoop created "<<this<<" in thread "<<threadId_;
	if(t_loopInThisThread){
		LOG_FATAL<<"Another EventLoop "<<t_loopInThisThread<<" exists in this thread "<<threadId_;
	}else{
		t_loopInThisThread=this;
	}
	wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead,this));
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop(){
	LOG_DEBUG<<"EventLoop "<<this<<" of thread "<<threadId_<<" destructs in thread "<<tid();
	wakeupChannel_->disableAll();
	wakeupChannel_->remove();
	::close(wakeupFd_);
	t_loopInThisThread=nullptr;
}

void EventLoop::loop(){
	assert(!looping_);
	assertInLoopThread();
	looping_=true;
	quit_=false;
	LOG_TRACE<<"EventLoop "<<this<<" start looping";

	while (!quit_) {
		activeChannel_.clear();
		pollReturnTime_=poller_->poll(kPollTimeMs, &activeChannel_);
		++iteration_;
		if(Logger::logLevel()<=Logger::TRACE){
			printActiveChannels();
		}
		eventHandling_=true;
		for(Channel* channel:activeChannel_){
			currentActivceChannel_=channel;
			currentActivceChannel_->handleEvent(pollReturnTime_);
		}
		currentActivceChannel_=nullptr;
		eventHandling_=false;
		doPendingFunctors();
	}
	LOG_TRACE<<"EventLoop "<<this<<" stop looping";
	looping_=false;
}

void EventLoop::quit(){
	quit_=true;
	if(isInLoopThread()){
		wakeup();
	}
}

void EventLoop::runInLoop(Functor cb){
	if(isInLoopThread()){
		cb();
	}else{
		queueInLoop(std::move(cb));
	}
}

void EventLoop::queueInLoop(Functor cb){
	{
		std::lock_guard<std::mutex>lock(mutex_);
		pendingFunctors_.push_back(std::move(cb));
	}

	if(!isInLoopThread()||callingPendingFunctors_){
		wakeup();
	}
}

size_t EventLoop::queueSize()const{
	std::lock_guard<std::mutex>lock(mutex_);
	return pendingFunctors_.size();
}

TimerId EventLoop::runAt(Timestamp time,TimerCallback cb){
	return timerQueue_->addTimer(std::move(cb),time,0.0);
}


TimerId EventLoop::runAfter(double delay,TimerCallback cb){
	Timestamp time(addTime(Timestamp::now(),delay));
	return runAt(time,std::move(cb));
}

TimerId EventLoop::runEvery(double interval,TimerCallback cb){
	Timestamp time(addTime(Timestamp::now(),interval ));
	return timerQueue_->addTimer(std::move(cb),time, interval);
}

void EventLoop::cancel(TimerId timerid){
	timerQueue_->cancel(timerid);
}

void EventLoop::updateChannel(Channel*channel){
	assert(channel->ownerLoop()==this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel*channel){
	assert(channel->ownerLoop()==this);
	assertInLoopThread();
	if(eventHandling_){
		assert(currentActivceChannel_==channel||std::find(activeChannel_.begin(),activeChannel_.end(),channel)==activeChannel_.end());
	}
	poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel*channel){
	assert(channel->ownerLoop()==this);
	assertInLoopThread();
	return poller_->hasChannel(channel);
}

void EventLoop::abortNotInLoopThread(){
	LOG_FATAL<<"EventLoop::abortNotInLoopThread - EventLoop "<<this
		<<" was created in threadId_ = "<<threadId_
		<<",current thread id = "<<tid();
}

void EventLoop::wakeup(){
	uint64_t one = 1;
	ssize_t n =sockets::write(wakeupFd_,&one,sizeof(one));
	if(n!=sizeof(one)){
		LOG_ERROR<<"EventLoop::wakeup() reads "<<n<<"bytes instead of 8";
	}
}

void EventLoop::handleRead(){
	uint64_t one = 1;
	ssize_t n =sockets::read(wakeupFd_,&one,sizeof(one));
	if(n!=sizeof(one)){
		LOG_ERROR<<"EventLoop::handleRead() reads "<<n<<"bytes instead of 8";
	}
}


void EventLoop::doPendingFunctors(){
	std::vector<Functor>functors;
	callingPendingFunctors_=true;
	{
		std::lock_guard<std::mutex>lock(mutex_);
		functors.swap(pendingFunctors_);
	}
	for(const auto& functor:functors){
		functor();	
	}
	callingPendingFunctors_=false;
}
void EventLoop::printActiveChannels()const{
	for(const Channel * channel:activeChannel_){
		LOG_TRACE<<"{"<<channel->reventsToString()<<"}";
	}
}
