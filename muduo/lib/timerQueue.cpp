#include "timerQueue.hpp"
#include "common.hpp"
#include <cassert>
#include <cstdint>
#include <ctime>
#include <iterator>
#include <utility>
#include <vector>
std::vector<Entry>muduo::TimerQueue::getExpired(muduo::Timestamp now){
	std::vector<Entry> expired;
	Entry sentry =std::make_pair(now,reinterpret_cast<Timer* >(UINTPTR_MAX));
	auto it=std::lower_bound(timers_.begin(),timers_.end(),sentry);
	assert(it ==timers_.end()||now <it->first);
	std::copy(timers_.begin(),it,std::back_inserter(expired));
	timers_.erase(timers_.begin(),it);
	return expired;
}
