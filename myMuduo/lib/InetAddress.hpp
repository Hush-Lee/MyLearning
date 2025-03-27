#pragma once
#include <cstdint>
#include <netinet/in.h>	
#include <sys/socket.h>

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);

class InetAddress{
public:
	explicit InetAddress(uint16_t port=0,bool loopbackOnly=false,bool ipv6=false);
	InetAddress(std::string_view ip,uint16_t port,bool ipv6=false);
	explicit InetAddress(struct sockaddr_in addr):addr_(addr){}
	explicit InetAddress(struct sockaddr_in6 addr):addr6_(addr){}

	sa_family_t family()const{return addr_.sin_family;}
	uint16_t port()const;
	std::string toIp()const;
	std::string toPortIp()const;
	const struct sockaddr* getSockAddr()const{return sockaddr_cast(&addr6_);}
	void setSockAddrInet6(const struct sockaddr_in6&addr){addr6_=addr;}

	uint32_t ipv4NetEndian()const;
	uint16_t portNetEndian()const{return addr_.sin_port;}
	static bool resolve(std::string_view hostname,InetAddress* result);


	void getScopeId(uint32_t scope_id);
private:
	union{
		struct sockaddr_in addr_;
		struct sockaddr_in6 addr6_;
	};
};
