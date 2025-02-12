#include <poll.h>
#include "channel.hpp"
const int Channel::kNoneEvent=0;
const int Channel::kReadEvent=POLLIN|POLLPRI;
const int Channel::kWriteEvent=POLLOUT;

Channel::Channel(EventLoop* loop,int fdArg)
	:loop_(loop),
	fd_(fdArg),
	events_(0),
	revents_(0),
	index_(-1){

	}
void Channel::update(){
	loop_->updateChannel(this);
}
