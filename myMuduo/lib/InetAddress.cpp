#include "InetAddress.hpp"
#include "Classes.hpp"
#include "Endian.hpp"
#include "SocketOps.hpp"
#include "Logging.hpp"
#include <netdb.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <string_view>
#include <sys/socket.h>


#pragma GCC diagnostic ignored "-Wold-style-cast"//这段代码告诉编译器后面出现c风格转换不再报错
static const in_addr_t kInaddrAny= INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;
#pragma GCC diagnostic error "-Wold-style-cast"//这段代码告诉编译器再出现c风格转换不是警告而是直接报错

static_assert(sizeof(InetAddress)==sizeof(struct sockaddr_in6),"InetAddress is same size as sockaddr_in");
static_assert(offsetof(sockaddr_in,sin_family)==0,"sin_family offset 0");
static_assert(offsetof(sockaddr_in6,sin6_family)==0,"sin6_family offset 0" );
static_assert(offsetof(sockaddr_in,sin_port)==2,"sin_port offset 2");
static_assert(offsetof(sockaddr_in6,sin6_port)==2,"sin6_port offset 2");

InetAddress::InetAddress(uint16_t portArg,bool loopbackOnly,bool ipv6){
	static_assert(offsetof(InetAddress,addr_)==0,"addr_ offset 0" );
	static_assert(offsetof(InetAddress, addr6_)==0,"addr6_ offset 0" );
	if(ipv6){
		memZero(&addr6_,sizeof(addr6_));
		addr6_.sin6_family = AF_INET6;
		addr6_.sin6_port = hostToNetwork16(portArg);
		in6_addr ip=loopbackOnly?in6addr_loopback:in6addr_any;
		addr6_.sin6_addr = ip;
	}else{
		memZero(&addr_, sizeof(addr_));
		addr_.sin_family=AF_INET;
		addr_.sin_port=hostToNetwork16(portArg);
		in_addr_t ip=loopbackOnly?kInaddrLoopback:kInaddrAny;
		addr_.sin_addr.s_addr=hostToNetwork32(ip);
	}
}

InetAddress::InetAddress(std::string_view ip,uint16_t portArg,bool ipv6){
	if(ipv6||strchr(ip.data(),':')){
		memZero(&addr6_,sizeof(addr6_));
		sockets::fromIpPort(ip.data(),portArg,&addr6_);
	}else{
		memZero(&addr_, sizeof(addr_));
		sockets::fromIpPort(ip.data(),portArg,&addr_);
	}
}

std::string InetAddress::toPortIp()const{
	char buf[64];
	sockets::toIpPort(buf, sizeof(buf),getSockAddr());
	return buf;
}

std::string InetAddress::toIp()const{
	char buf[64];
	sockets::toIp(buf,sizeof(buf),getSockAddr());
	return buf;
}

uint32_t InetAddress::ipv4NetEndian()const{
	assert(addr_.sin_family==AF_INET);
	return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::port()const{
	return networkToHost16(portNetEndian());
}

static thread_local char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(std::string_view hostname, InetAddress *out){
	assert(out!=nullptr);
	struct hostent hent;
	struct hostent* he=nullptr;
	int herrno=0;
	memZero(&hent, sizeof(hent));
	int ret=gethostbyname_r(hostname.data(),&hent,t_resolveBuffer,sizeof(t_resolveBuffer), &he, &herrno);
	if(ret==0&&he!=nullptr){
		assert(he->h_addrtype==AF_INET&&he->h_length==sizeof(uint32_t));
		out->addr_.sin_addr=*reinterpret_cast<struct in_addr*>(he->h_addr);
		return true;
	}else{
		if(ret){
			LOG_SYSERR<<"resolve()";
		}
		return false;
	}
}

void InetAddress::setScopeId(uint32_t scope_id){
	if(family()==AF_INET6){
		addr6_.sin6_scope_id=scope_id;
	}
}
