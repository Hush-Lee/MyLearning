#pragma once
#include <cstddef>
class Timer;

class TimerId{
public:
	TimerId():timer_(nullptr),sequence_(0){}
	TimerId(Timer *timer,int64_t seq):timer_(timer),sequence_(seq){}
	friend class TimerQueue;
private:
	int64_t sequence_;
	Timer* timer_;
};
