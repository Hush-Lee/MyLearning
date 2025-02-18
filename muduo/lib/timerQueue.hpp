#pragma  once
#include"common.hpp" 
#include <memory>
#include <set>
#include <vector>
namespace muduo{
	class TimerQueue:muduo::noncopyable{
		public:
			TimerQueue(EventLoop*loop);
			~TimerQueue();
			TimerID addTimer(const TimerCallback& cb,Timestamp wehn,double interval);
			typedef std::pair<Timestamp,Timer> Entry ;;
			typedef std::set<Entry> TimerList ;
			void handleRead();
			std::vector<Entry> getExpired(Timestamp now);
			bool insert(std::unique_ptr<Timer> timer);

			EventLoop* loop_;
			const int timerfd_;
			Channel timerfdChannel_;
			TimerList timers_;
	};
}
