#pragma once
#include <cstring>
class Timer;
class TimerId;
class Timestamp;
class Logger;
class Buffer;
class EventLoop;
class Poller;
class Channel;
class TimerQueue;

inline void memZero(void * p,size_t n){
	memset(p, 0, n);
}

class noncopyable{
protected:
	noncopyable()=default;
	~noncopyable()=default;
public:
	noncopyable(const noncopyable&)=delete;
	noncopyable& operator=(const noncopyable&)=delete;
};
