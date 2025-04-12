#include "Thread.hpp"
#include <sched.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sys/unistd.h>
#include <sys/types.h>
pid_t gettid(){
	return static_cast<pid_t>(::syscall(SYS_gettid));
}


