#include "Timestamp.hpp"
#include "Classes.hpp"
#include <bits/types/struct_timeval.h>
#include <sys/time.h>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <inttypes.h>
#include <sys/select.h>
static_assert(sizeof(Timestamp)==sizeof(int64_t),"Timestamp should be same size as int64_t");

std::string Timestamp::toString()const{
	char buf[32]={0};
	auto seconds=microSecondSinceEpoch_/kMicrosecondsPerSecond;
	auto microseconds=microSecondSinceEpoch_%kMicrosecondsPerSecond;
	snprintf(buf,sizeof(buf), "%" PRId64 ".%06" PRId64,seconds,microseconds);
	return std::string(buf);
}

std::string Timestamp::toFormattedString(bool showMicroseconds)const{
	char buf[64]={0};
	auto seconds=static_cast<time_t>(microSecondSinceEpoch_/kMicrosecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds,&tm_time);
	if(showMicroseconds){
		auto microseconds=static_cast<int>(microSecondSinceEpoch_/kMicrosecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d%04d",
				tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,
				tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec,microseconds);
	}else{
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
				tm_time.tm_year+1900,tm_time.tm_mon+1,tm_time.tm_mday,
				tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);
	}
	return buf;

}

Timestamp Timestamp::now(){
	struct timeval tv;
	gettimeofday(&tv,nullptr);
	auto secons=tv.tv_sec;
	return Timestamp(secons*Timestamp::kMicrosecondsPerSecond+tv.tv_usec);
}
