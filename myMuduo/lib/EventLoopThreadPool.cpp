#include "EventLoopThreadPool.hpp"

EventLoopThreadPool::EventLoopThreadPool(EventLoop*baseLoop,const std::string &nameArg):
	baseLoop_(baseLoop),
	name_(nameArg),
	started_(false),
	numThreads_(0),
	next_(0){

	}
