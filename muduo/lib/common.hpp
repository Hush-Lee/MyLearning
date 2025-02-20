#pragma once
namespace muduo {
	class Channel;
	class EventLoop;
	class Poller;
	class Buffer;
	class Timer;
	class Socket;
	class Timestamp;
	class TimerId;
	class TimerQueue;

	class noncopyable{
	public:
		noncopyable()=default;
	private:
		noncopyable(noncopyable& t);
		noncopyable(noncopyable&& t);
		noncopyable& operator =(noncopyable& t);
		noncopyable& operator =(noncopyable&& t);
	};

}
