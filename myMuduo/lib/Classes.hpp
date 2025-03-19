#pragma once
class Timer;
class TimerId;
class Timestamp;
class Logger;
class Buffer;
class EventLoop;
class Poller;
class Channel;
class TimerQueue;
class noncopyable{
protected:
	noncopyable()=default;
	~noncopyable()=default;
public:
	noncopyable(const noncopyable&)=delete;
	noncopyable& operator=(const noncopyable&)=delete;
};
