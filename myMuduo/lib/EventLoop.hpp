#pragma once
#include "Classes.hpp"
class EventLoop:noncopyable{

private:
	void abortNotInLoopThread();
	void handleRead();//wake up
};
