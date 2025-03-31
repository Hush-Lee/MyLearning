#include "SocketOps.hpp"
#include "Classes.hpp"
#include "Logging.hpp"
#include "Endian.hpp"
#include <asm-generic/socket.h>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <netinet/in.h>
using SA=struct sockaddr;

#if VALGRIND|| defined (NO_ACCEPT4)//保证原子性,防止accept和fcntl之间exec
void setNonBlockAndCloseOnExec(int sockfd){
	int flags=::fcntl(sockfd,F_GETFL,0);
	flags|=O_NONBLOCKING;
	int ret=::fcntl(sockfd,F_SETFL,flags);

	flags=::fcntl(sockfd,F_GETFL,0);
	flags|=FD_CLOEXEC;
	ret=::fcntl(sockfd,F_SETFL,flags);
	(void)res;
}
//确保sockfd不会被子进程继承,当exec时直接关闭

#endif

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* sockets::sockaddr_in6_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}



int sockets::createNonblockingOrDie(sa_family_t family){
#if(VALGRIND)
	int sockfd=::socket(family,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd<0){
		LOG_SYSFATAL<<"createNonblockingOrDie()";
	}
	setNonBlockAndCloseOnExec(sockfd);
#else
	int sockfd=::socket(family,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_TCP);
	if(sockfd<0){
		LOG_SYSFATAL<<"createNonblockingOrDie()";
	}
#endif
	return sockfd;
}

void sockets::bindOrDie(int sockfd,const struct sockaddr* addr){
	int ret=::bind(sockfd, addr, sizeof(*addr));
	if(ret<0){
		LOG_SYSFATAL<<"bindOrDie()";
	}
}

void sockets::listenOrDie(int sockfd){
	int ret=::listen(sockfd,SOMAXCONN);
	if(ret<0){
		LOG_SYSFATAL<<"listenOrDie()";
	}
}

int sockets::accept(int sockfd, struct sockaddr_in6 *addr){
	socklen_t len=static_cast<socklen_t>(sizeof(*addr));
#if VALGRIND||defined (NO_ACCEPT4)
	int connfd=::accept(sockfd,sockaddr_cast(addr),&len);
	setNonBlockAndCloseOnExec(sockfd);
#else
	int connfd=::accept4(sockfd,sockaddr_cast(addr),&len,SOCK_NONBLOCK|SOCK_CLOEXEC);
#endif
	if(connfd<0){
		int savedErrno=errno;
		LOG_SYSFATAL<<"accept()";
		switch (savedErrno) {
			case EAGAIN:	
			case ECONNABORTED:
			case EINTR:
			case EPERM:
			case EMFILE:
				errno=savedErrno;
				break;
			case EBADF:
			case EFAULT:
			case EINVAL:
			case ENFILE:
			case ENOBUFS:
			case ENOMEM:
			case ENOTSOCK:
			case EOPNOTSUPP:
				LOG_FATAL<<"unexpect Error of accept()";
				break;
			default:
				LOG_FATAL<<"unknown error of accept()";
				break;
		}
	}
	return connfd;
}


int sockets::connect(int sockfd, const struct sockaddr *addr){
	return ::connect(sockfd,addr,implicit_cast<socklen_t>(sizeof(*addr)));
}

ssize_t sockets::read(int sockfd, void *buf, size_t acount){
	return ::read(sockfd, buf, acount);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt){
	return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count){
	return ::write(sockfd,buf, count);
}

void sockets::close(int sockfd){
	if(::close(sockfd)<0){
		LOG_SYSFATAL<<"close()";
	}
}

void sockets::shutdownWrite(int sockfd){
	if(::shutdown(sockfd,SHUT_WR)<0){
		LOG_SYSERR<<"shutdownWrite()";
	}
}

void sockets::toIp(char *buf, size_t size, const struct sockaddr *addr){
	if(addr->sa_family==AF_INET){
		assert(size>=INET_ADDRSTRLEN);
		const struct sockaddr_in*addr4=sockaddr_in_cast(addr);
		::inet_ntop(AF_INET,&addr4->sin_addr,buf,static_cast<socklen_t>(size));
	}else{
		assert(size>=INET6_ADDRSTRLEN);
		const struct sockaddr_in6* addr6=sockaddr_in6_cast(addr);
		::inet_ntop(AF_INET6,&addr6->sin6_addr,buf, static_cast<socklen_t>(size));
	}

}


void sockets::toIpPort(char *buf, size_t size, const struct sockaddr *addr){
	if(addr->sa_family==AF_INET6){
		buf[0]='[';
		toIp(buf+1, size-1, addr);
		size_t end=::strlen(buf);
		const struct sockaddr_in6* addr6=sockaddr_in6_cast(addr);
		uint16_t port=networkToHost16(addr6->sin6_port);
		assert(size>end);
		snprintf(buf+end,size-end,"]:%u",port);
		return;
	}
	toIp(buf, size, addr);
	size_t end=::strlen(buf);
	const struct sockaddr_in*addr4=sockaddr_in_cast(addr);
	uint16_t port=networkToHost16(addr4->sin_port);
	assert(size>end);
	snprintf(buf+end,size-end,":%u",port);

}


void sockets::fromIpPort(const char*ip,uint16_t port,struct sockaddr_in*addr){
	addr->sin_family=AF_INET;
	addr->sin_port=hostToNetwork16(port);
	if(::inet_pton(AF_INET,ip,&addr->sin_addr)<=0){
		LOG_SYSERR<<"fromIpPort()";
	}
}
void sockets::fromIpPort(const char *ip, uint16_t port, struct sockaddr_in6 *addr){
	addr->sin6_family=AF_INET6;
	addr->sin6_port=hostToNetwork16(port);
	if(::inet_pton(AF_INET6,ip,&addr->sin6_addr)<=0){
		LOG_SYSERR<<"fromIpPort()"; 
	}
}

int sockets::getSocketError(int sockfd){
	int optval;
	socklen_t optlen=static_cast<socklen_t>(sizeof(optval));
	if(::getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&optval,&optlen)<0){
		return errno;
	}else{
		return optval;
	}
}

struct sockaddr_in6 sockets::getLocalAddr(int sockfd){
	struct sockaddr_in6 localAddr;
	memZero(&localAddr, sizeof(localAddr));
	socklen_t addrlen=static_cast<socklen_t>(sizeof(localAddr));
	if(::getsockname(sockfd,sockaddr_cast(&localAddr),&addrlen)<0){
		LOG_SYSERR<<"getLocalAddr()";
	}
	return localAddr;
}

struct sockaddr_in6 sockets::getPeerAddr(int sockfd){
	struct sockaddr_in6 peerAddr;
	memZero(&peerAddr,sizeof(peerAddr));
	socklen_t addrlen=static_cast<socklen_t>(sizeof(peerAddr));
	if(::getpeername(sockfd,sockaddr_cast(&peerAddr),&addrlen)<0){
		LOG_SYSERR<<"getPeerAddr()";
	}

	return peerAddr;
}

bool sockets::isSlefConnect(int sockfd){
	struct sockaddr_in6 localaddr=getLocalAddr(sockfd);
	struct sockaddr_in6 peeraddr=getPeerAddr(sockfd);
	if(localaddr.sin6_family==AF_INET){
		struct sockaddr_in* localaddr4=reinterpret_cast<struct sockaddr_in*>(&localaddr);
		struct sockaddr_in* peeraddr4=reinterpret_cast<struct sockaddr_in*>(&peeraddr);
		return localaddr4->sin_port==peeraddr4->sin_port&&localaddr4->sin_addr.s_addr==peeraddr4->sin_addr.s_addr;
	}else if(localaddr.sin6_family==AF_INET6){
		return localaddr.sin6_port==peeraddr.sin6_port&&
			memcmp(&localaddr.sin6_addr,&peeraddr.sin6_addr, sizeof(localaddr.sin6_addr))==0;
	}else{
		return false;
	}
}
