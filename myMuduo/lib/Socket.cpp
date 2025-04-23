#include "Socket.hpp"
#include "Classes.hpp"
#include "SocketOps.hpp"
#include <sys/socket.h>
#include <netinet/tcp.h>

Socket::~Socket(){
	sockets::close(sockfd_);
}

bool Socket::getTcpInfo(struct tcp_info* tcpi)const{
	socklen_t len=sizeof(*tcpi);
	memZero(tcpi,len);
	return ::getsockopt(sockfd_,SOL_TCP, TCP_INFO, tcpi, &len)==0;
}
