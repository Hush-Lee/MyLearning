#pragma once
#include "Poller.hpp"
#include <vector>

struct epoll_event;
class EpollPoller:Poller{
public:
	EpollPoller(EventLoop* loop);
	~EpollPoller()override;
	Timestamp poll(int timeoutMs,ChannelList*activeChannels)override;
	void updateChannel(Channel* channel)override;
	void removeChannel(Channel* channel)override;
private:
	static const int kInitEventListSize=16;
	static const char* operationToString(int op);
	void fillActiveChannels(int numEvents,ChannelList*activeChannels)const;
	void update(int operation,Channel*channel);
	using EventList = std::vector<struct epoll_event>;
	int epollfd_;
	EventList events_;
};
