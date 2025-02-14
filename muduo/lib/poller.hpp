#pragma once
#include <vector>
struct pollfd;
#include "common.hpp"
namespace muduo {
	class Poller:noncopyable{
		public:
			typedef std::vector<Channel*>ChannelList;
			Poller(EventLoop *loop);
			~Poller();
			Timestamp poll(int timeoutMs,ChannelList* activeChannels);
			void updateChannel(Channel * channel);
			void assertInLoopThread(){
				ownerLoop_->assertInLoopThread();
			}
		private:
			void fillActiveChannels(int numEvents,ChannelList* activeChannels)const;
			typedef std::vector<struct pollfd> PollFdList;
			typedef std::map<int,Channel*> ChannelMap;

			EventLoop* ownerLoop_;
			PollFdList pollfds_;
			ChannelMap channels_;

	}
}
