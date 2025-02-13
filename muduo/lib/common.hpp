#pragma once

class Channel;
class EventLoop;
class Poller;
class Buffer;
class timer;
class socket;

class noncopyable{
public:
	noncopyable()=default;
private:
	noncopyable(noncopyable& t);
	noncopyable(noncopyable&& t);
	noncopyable& operator =(noncopyable& t);
	noncopyable& operator =(noncopyable&& t);
};
