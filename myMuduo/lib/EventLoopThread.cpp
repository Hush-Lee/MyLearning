#include "EventLoopThread.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,const std::string& name)
	:loop_(nullptr),
	exiting_(false),
	thread_(std::bind(&EventLoopThread::threadFunc,this),name),
	mutex_(),
	cond_(),
	callback_(cb){

	}
