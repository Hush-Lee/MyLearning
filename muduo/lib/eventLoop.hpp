#pragma once
#include "channel.hpp"
#include"common.hpp"
#include <atomic>
#include <thread>
class EventLoop:nocopyable{
public:
	EventLoop();
	~EventLoop();
	void loop();
	void assertInLoopThread(){
		if(!isINLoopThread()){
			abortNotInLoopThread();
		}
	}
	bool isINLoopThread()const{return threadID_==std::this_thread::get_id();}
	static EventLoop* getEventLoopOfCurrentThread();
	void updateChannel(Channel* ch);

private:
	void abortNotInLoopThread();

	std::atomic_bool looping_;
	std::thread::id threadID_;

};
