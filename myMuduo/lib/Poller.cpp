#include "Poller.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"
Poller::Poller(EventLoop* loop):ownerLoop_(loop){

}

Poller::~Poller()=default;
bool Poller::hasChannel(Channel*channel)const{
	assertInLoopThread();
	ChannelMap::const_iterator it=channels_.find(channel->fd());
	return it!=channels_.end()&&it->second == channel;
}
