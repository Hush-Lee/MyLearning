#pragma once

#include "Classes.hpp"
#include <map>
#include <vector>

class Poller:noncopyable{
public:
	using ChannelList=std::vector<Channel*>;
	Poller(EventLoop*loop);
	virtual ~Poller();
	virtual Timestamp poll(int timeoutMs,ChannelList*activeChannel)=0;
	virtual void updateChannel(Channel* channel)=0;
	virtual void removeChannel(Channel* channel)=0;
	virtual bool hasChannel(Channel* channel)const;
	static Poller* newDefaultPoller(EventLoop*loop);
	void assertInLoopThread()const{
		ownerLoop_->asserInLoopThread();
	}
protected:
	using ChannelMap=std::map<int,Channel*>;
	ChannelMap channels_;
private:
	EventLoop* ownerLoop_;
}
