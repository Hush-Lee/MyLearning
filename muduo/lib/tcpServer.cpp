#include "tcpServer.hpp"
#include "tcpConnection.hpp"
#include <functional>

void muduo::TcpServe::removeConnection(const TcpConnectionPtr& conn){
	loop_->assertInLoopThread();
	LOG_INFO<<"TcpServe::removeConnection ["<<name_<<" ] - connections"<<conn->name();
	size_t n = connections_.erase(conn->name());
	asser(n == 1);
	(void)n;
	loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed,conn));
}
