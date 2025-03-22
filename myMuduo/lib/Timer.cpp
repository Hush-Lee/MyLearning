#include "Timer.hpp"
#include "Timestamp.hpp"
void Timer::restart(Timestamp now){
	if(repeat_){
		expiration_=addTime(now,interval_);
	}else{
		expiration_=Timestamp::invalid();
	}
}
