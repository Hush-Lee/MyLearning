#include "TimerQueue.hpp"
#include "Logging.hpp"
#include "Timestamp.hpp"
#include <cstdint>
#include <ctime>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>
int creatTiemrFd(){
	int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
	if(timerfd<0){
		LOG_SYSFATAL<<"Failed in timerfd_create";
	}
	return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when){
	int64_t microseconds=when.microSecondSinceEpoch()-Timestamp::now().microSecondSinceEpoch();
	if(microseconds<100){
		microseconds=100;
	}
	struct timespec ts;
	ts.tv_sec=static_cast<time_t>(microseconds/Timestamp::kMicrosecondsPerSecond);
	ts.tv_nsec=static_cast<long>((microseconds%Timestamp::kMicrosecondsPerSecond)*1000);
	return ts;
}

void readTimerfd(int timerfd,Timestamp now){
	uint64_t howmany;
	ssize_t n=::read(timerfd,&howmany,sizeof(howmany));
	LOG_TRACE<<"TimerQueue::handleRead()"<<howmany<<" at "<<now.toString();
	if(n!=sizeof(howmany)){
		LOG_ERROR<<"TimerQueue::handleRead() reads"<<n<<" bytes instead of 8";
	}
}

void resetTimerfd(int timerfd,Timestamp expired){
	struct itimerspec newValue;
	struct itimerspec oldValue;
	memZero(&newValue,sizeof(newValue));
	memZero(&oldValue,sizeof(oldValue));
}

