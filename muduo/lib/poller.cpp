#include "poller.hpp"
#include "common.hpp"
#include <cstdlib>
#include <sys/poll.h>
#include "channel.hpp"
#include <cassert>
muduo::Poller::Poller(EventLoop* loop):ownerLoop_(loop){
}
muduo::Poller::~Poller(){}

muduo::Timestamp muduo::Poller::poll(int timeoutMs,ChannelList* activeChannels){
	int numEvents=::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
	Timestamp now(muduo::Timestamp::now());
	if(numEvents>0){
		LOGTRACE<<numEvents<<" events happended";
		fillActiveChannels(numEvents,activeChannels);
	}else if(numEvents==0){
		LOGTRACE<<" nothing happended";
	}else{
		LOG_SYSERR<<"Poller::poll()";
	}
	return now;
}

void muduo::Poller::fillActiveChannels(int numEvents,ChannelList*activeChannels)const{
	for(PollFdList::const_iterator pfd=pollfds_.cbegin();pfd!=pollfds_.end()&&numEvents>0;++pfd){
		if(pfd->revents>0){
			--numEvents;
			ChannelMap::const_iterator ch = channels_.find(pfd->fd);
			assert(ch != channels_.end());
			Channel* channel=ch->second;
			assert(channel->fd()==pfd->fd);
			channel->set_revents(pfd->revents);
			activeChannels->push_back(channel);
		}
	}
}

void muduo::Poller::updateChannel(muduo::Channel* channel){
	assertInLoopThread();
	LOG_TRACE<< "fd = "<<channel->fd()<<" events = "<<channel->events();
	if(channel->index()<0){
		assert(channels_.find(channel->fd())==channels_.end());
		struct pollfd pfd;
		pfd.fd=channel->fd();
		pfd.events=static_cast<short>(channel->events());
		pfd.revents==0;
		pollfds_.push_back(pfd);
		int idx=static_cast<int>(pollfds_.size())-1;
		channel->set_index(idx);
		channels_.at(pfd.fd)=channel;
	}else{
		assert(channels_.find(channel->fd())!=channels_.end());
		assert(channels_.at(channel->fd())!=channel);
		int idx = channel->index();
		assert(0<=idx&&idx<static_cast<int>(pollfds_.size()));
		struct pollfd& pfd=pollfds_.at(idx);
		assert(pfd.fd==channel->fd()|| pfd.fd==-1);
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
		if(channel->isNoneEvent()){
			pfd.fd=-1;
		}
	}
}

