#pragma once
#include "Classes.hpp"
#include <condition_variable>
#include <mutex>
class CountDownLatch:noncopyable{
public:
	explicit CountDownLatch(int count);
	void wait();
	void countDown();
	int getCount();
private:
	mutable std::mutex mutex_;
	std::condition_variable condition_ GUARDED_BY(mutex_);
	int count_ GUARDED_BY(mutex_);
};
