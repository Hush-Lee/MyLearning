#include "CountDownLatch.hpp"
#include <condition_variable>
#include <mutex>
CountDownLatch::CountDownLatch(int count):
	mutex_(),
	condition_(),
	count_(count){

}
void CountDownLatch::wait(){
	std::unique_lock<std::mutex>lk(mutex_);
	condition_.wait(lk,count_<=0);
}
void CountDownLatch::countDown(){
	std::lock_guard<std::mutex>lg(mutex_);
	--count_;
	if(count_==0){
		condition_.notify_all();
	}
}

int CountDownLatch::getCount(){
	std::lock_guard<std::mutex>lg(mutex_);
	return count_;
}
