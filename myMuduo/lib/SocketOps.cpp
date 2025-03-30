#include "SocketOps.hpp"
#include "Classes.hpp"
#include "Logging.hpp"
#include "Endian.hpp"
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
