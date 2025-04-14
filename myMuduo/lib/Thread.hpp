#pragma once
#include "Classes.hpp"
#include <string>
#include <atomic>
#include <cstdint>
#include <functional>
#include <pthread.h>
#include <latch>
class Thread:noncopyable{
public:
	using ThreadFunc=std::function<void()>;
	explicit Thread(ThreadFunc,const std::string& name=std::string());
	~Thread();
	void start();
	int join();
	bool started()const;
	pid_t tid()const;
	const std::string& name()const{return name_;}
	static int numCreated(){return numCreated_.get();}
private:
	void setDefaultName();
	bool started_;
	bool joined_;
	pthread_t pthreadId_;
	pid_t tid_;
	ThreadFunc func_;
	std::string name_;
	std::latch  latch_;
	static std::atomic<int32_t> numCreated_;
};
