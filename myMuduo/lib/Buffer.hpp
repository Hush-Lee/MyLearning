#pragma once
#include <stdlib.h>
#include <vector>

#define BUFSIZE 8092

class Buffer{
public:
	static const int kCheapPrepend=8;
	static const size_t kInitialiSize=1024;

	ssize_t readFd(int fd,int *savedErr);
	ssize_t writeFd(int fd,int *savedErr);
	char* peek();
	size_t readableBytes()const;
	void retrieve(size_t len);
	void retrieveAll();
	std::string retrieveAllAsString();

	char*beginWrite();
	size_t writableByess()const;
	void hasWriten(size_t len);

	void append(const char* data,size_t lean);
	void append(const std::string&str);
	

private:
	std::vector<char> buffer_; 
	size_t readIndex_;//从这里开始读消息
	size_t writeIndex_;//从这里开始写消息,也就是可读消息到此为止
};
