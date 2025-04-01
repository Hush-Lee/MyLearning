#pragma once
#include "Classes.hpp"
#include "CurrentThread.hpp"
#include <any>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include "Callback.hpp"
#include <mutex>
#include <vector>
#include <pthread.h>
class EventLoop:noncopyable{
public:
	using Functor=std::function<void()>;
	EventLoop();
	~EventLoop();
	void loop();
	void quit();

	Timestamp pollReturnTime()const{return pollReturnTime_;};
	int64_t iteration()const{return iteration_;}
	void runInLoop(Functor cb);
	void queueInLoop(Functor cb);

	size_t queueSize()const;
	TimerId runAt(Timestamp time,TimerCallback cb);
	TimerId runAfter(Timestamp delay,TimerCallback cb);
	TimerId runEvery(Timestamp interval,TimerCallback cb);

	void cancel(TimerId timerid);
	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel*channel);
	bool hasChannel(Channel*channel);
	void assertInLoopThread(){
		if(!isInLoopThread()){
			abortNotInLoopThread();
		}
	}
	bool isInLoopThread(){return threadId_==tid();}
	bool eventHandling(){return eventHandling_;}
	void setContext(const std::any&context){
		context_=context;
	}
	const std::any&getContext(){return context_;}

	std::any&getMutableContext(){
		return context_;
	}
	static EventLoop* getEventLoopOfCurrentThread();
private:

	void abortNotInLoopThread();
	void handleRead();//wake up
	void doPendingFunctors();

	void printActiveChannels()const;
	using ChannelList = std::vector<Channel*> ;
	bool looping_;

	std::atomic_bool quit_;
	bool eventHandling_;
	bool callingPendingFunctors_;
	int64_t iteration_;
	const pid_t threadId_;
	Timestamp pollReturnTime_;
	std::unique_ptr<Poller>poller_;
	std::unique_ptr<TimerQueue>timerQueue_;

	int wakeupFd_;
	std::unique_ptr<Channel>wakeupChannel_;
	std::any context_;
	ChannelList activeChannel_;
	Channel * currentActivceChannel_;

	mutable std::mutex mutex_;
	std::vector<Functor>pendingFunctors_ GUARDED_BY(mutex_);
};
