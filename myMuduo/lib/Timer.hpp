#pragma once
#include "Callback.hpp"
#include "Classes.hpp"
#include <atomic>
#include <cstdint>
class Timer{
public:
	Timer(TimerCallback cb,Timestamp when,double interval):
		callback_(std::move(cb)),expiration_(when),interval_(interval),repeat_(interval>0.0),sequence_(s_numCreate_.fetch_add(1))
	{}

	void restart(Timestamp now);

	void run()const{
		callback_();
	}
	double interval()const{return interval_;}
	bool repeat()const{return repeat_;}
	Timestamp expiration()const{return expiration_;}
	int64_t sequence()const{return sequence_;}
	static int64_t numCreate(){return s_numCreate_;}
private:
	const TimerCallback callback_;
	Timestamp expiration_;
	const double interval_;
	const bool repeat_;
	const int64_t sequence_;
	static std::atomic_int64_t s_numCreate_;
};
