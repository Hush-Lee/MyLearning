#include <unistd.h>
#include <sys/types.h>
#include "tcpConnection.hpp"
void TcpConnection::handleRead(){
	char buf[65535];
	ssize_t n =::read(channel_->fd(),buf,sizeof(buf));
	messageCallback_(shared_from_this(),buf,n);
}
