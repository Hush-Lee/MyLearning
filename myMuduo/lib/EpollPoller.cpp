#include "EpollPoller.hpp"
#include "Classes.hpp"
#include "Poller.hpp"
#include "Logging.hpp"
#include "Timestamp.hpp"
#include <cerrno>
#include <cstddef>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <unistd.h>

static_assert(EPOLLIN == POLLIN, "epoll use same flag value as poll");
static_assert(EPOLLPRI == POLLPRI, "epoll use same flag value as poll");
static_assert(EPOLLOUT == POLLOUT, "epoll use same flag value as poll");
static_assert(EPOLLRDHUP == POLLRDHUP, "epoll use same flag value as poll");
static_assert(EPOLLERR == POLLERR, "epoll use same flag value as poll");
static_assert(EPOLLHUP == POLLHUP, "epoll use same flag value as poll");


namespace {
const int kNew=1;
const int kAdded=1;
const int kDeleted=2;
}
//类似static变量,编译器自动添加using语句

EpollPoller::EpollPoller(EventLoop*loop):
	Poller(loop),
	epollfd_(::epoll_create(EPOLL_CLOEXEC)),
	events_(kInitEventListSize){
	
		if(epollfd_<0){
			LOG_SYSFATAL<<"EpollPoller::EpollPoller";
		}

}

EpollPoller::~EpollPoller(){
	::close(epollfd_);
}


Timestamp EpollPoller::poll(int timeoutMs,ChannelList*activeChannels){
	LOG_TRACE<<"fd total count "<<channels_.size();
	int numEvents =::epoll_wait(epollfd_, &*events_.begin(),static_cast<int>(events_.size()),timeoutMs);
	int savedErrno=errno;
	Timestamp now(Timestamp::now());
	if(numEvents>0){
		LOG_TRACE<<numEvents<<" events happened";
		fillActiveChannels(numEvents, activeChannels);
		if(implicit_cast<size_t>(numEvents)==events_.size()){
			events_.resize(events_.size()*2);
		}
	}else if(numEvents==0){
		LOG_TRACE<<"nothing happend";
	}else{
		if(errno!=EINTR){
			errno=savedErrno;
			LOG_SYSERR<<"EpollPoller::poll()";
		}
	}
	return now;

}

void EpollPoller::fillActiveChannels()

