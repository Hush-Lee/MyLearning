#include "poller.hpp"
#include "common.hpp"
#include <cassert>
muduo::Poller::Poller(EventLoop* loop):ownerLoop_(loop){
}
muduo::Poller::~Poller(){}

muduo::Poller::poll(int timeoutMs,ChannelList* activeChannels){
	int numEvents=::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
	Timestamp now(Timestamp::now());
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
