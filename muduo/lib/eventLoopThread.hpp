#pragma once
#include "common.hpp"
namespace muduo {
	class EventLoopThread{
		EventLoop* startLoop();
		void threadFunc();
	};
}
