#pragma once
#include "Callback.hpp"
#include <atomic>
#include <cstdint>
class Timer{
public:
	Timer(TimerCallback&cb);
private:
	const TimerCallback callback_;
	TimeStamp expiration_;
	const double interval_;
	const bool repeat_;
	const int64_t sequence_;

	const std::atomic_int64_t s_numCreate_;
};
