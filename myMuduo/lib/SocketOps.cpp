#include "SocketOps.hpp"
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

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}


