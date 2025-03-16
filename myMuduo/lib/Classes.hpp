#pragma once
class Timer;
class Timestamp;
class Logger;

class noncopyable{
protected:
	noncopyable()=default;
	~noncopyable()=default;
public:
	noncopyable(const noncopyable&)=delete;
	noncopyable& operator=(const noncopyable&)=delete;
};
