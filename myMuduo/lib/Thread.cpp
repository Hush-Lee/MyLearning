#include "Thread.hpp"
#include "CurrentThread.hpp"
#include "Exception.hpp"
#include <exception>
#include <linux/prctl.h>
#include <string>
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <utility>
pid_t gettid(){
	return static_cast<pid_t>(::syscall(SYS_gettid));
}
void after_fork(){
	t_cachedTid=0;
	t_threadName="main";
	tid();
}
class ThreadNameInitializet{
public:
	ThreadNameInitializet(){
		t_threadName="main";
		tid();
		pthread_atfork(nullptr,nullptr,&after_fork );
	}
};

struct ThreadData{
	using ThreadFunc = Thread::ThreadFunc;
	ThreadFunc func_;
	std::string name_;
	pid_t* tid_;
	std::latch* latch_;
	ThreadData(ThreadFunc func,const std::string& name,pid_t* tid ,std::latch* latch):
		func_(std::move(func)),
		tid_(tid),
		latch_(latch){
		
		}
	void runInThread(){
		*tid_ = CurrentThread::tid();
		tid_=nullptr;
		latch_->count_down();	
		latch_=nullptr;
		CurrentThread::t_threadName=name_.empty()?"muduo Thread":name_.c_str();
		::prctl(PR_SET_NAME,CurrentThread::t_threadName);
		try{
			func_();
			CurrentThread::t_threadName="finished";
		}catch(const Exception&ex){
			CurrentThread::t_threadName="crashed";
       
		 fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
		 fprintf( stderr, "reason: %s\n", ex.what());
		 fprintf( stderr, "stack trace: %s\n", ex.stackTrace());
		 abort();
	       }catch (const std::exception& ex) {
		 CurrentThread::t_threadName = "crashed";
		 fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
		 fprintf( stderr, "reason: %s\n", ex.what());
		 abort();
	       }catch (...){
		 CurrentThread::t_threadName = "crashed";
		 fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
		 throw; // rethrow
	       }
	}
};
