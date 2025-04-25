#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdlib.h>
#include <sys/types.h>
#include <utility>
#include <vector>
#include <string>

#define BUFSIZE 8092

class Buffer{
public:
	explicit Buffer(size_t initialSize=kInitialiSize)
		:buffer_(kCheapPrepend+initialSize),
		writeIndex_(kCheapPrepend),
		readIndex_(kCheapPrepend){
		assert(writableBytes()==initialSize);
		assert(readableBytes()==0);
		assert(prependableBytes()==kCheapPrepend);
		}
	static const int kCheapPrepend=8;
	static const size_t kInitialiSize=1024;

	void swap(Buffer &rhs){
		buffer_.swap(rhs.buffer_);
		std::swap(this->writeIndex_,rhs.writeIndex_);
		std::swap(this->readIndex_,rhs.readIndex_);
	}
	ssize_t readFd(int fd,int *savedErr);
	const char* peek()const{
		return begin()+readIndex_;
	}
	size_t readableBytes()const{
		return writeIndex_-readIndex_;
	}
	void retrieve(size_t len);
	void retrieveAll(){
		writeIndex_=kCheapPrepend;
		readIndex_=kCheapPrepend;
	}

	std::string retrieveAsString(size_t len){
		assert(len<=readableBytes());
		std::string res(peek(),len);
		retrieve(len);
		return res;
	}
	std::string retrieveAllAsString(){
		return retrieveAsString(readableBytes());
	}

	char* beginWrite(){
		return begin()+writeIndex_;
	}
	const char* beginWrite()const{
		return begin()+writeIndex_;
	}
	size_t writableBytes()const{
		return buffer_.size()-writeIndex_;
	}
	size_t prependableBytes()const{
		return readIndex_;
	}
	void hasWriten(size_t len){
		assert(len<=writableBytes());
		writeIndex_+=len;
	}

	const char* findCRLF()const{
		const char*crlf=std::search(peek(),beginWrite(),kCRLF,kCRLF+2);
		return crlf==beginWrite()?nullptr:crlf;
	}

	const char* findCRLF(char * start)const{
		assert(start>=peek());
		assert(start<=beginWrite());
		const char*crlf=std::search(peek(),beginWrite(),kCRLF,kCRLF+2);
		return crlf==beginWrite()?nullptr:crlf;
	}

	void append(const char* data,size_t len){
		ensureWritableBytes(len);
		std::copy(data,data+len,beginWrite());
	}
	void append(const std::string&str){
		append(static_cast<char *>(str),str.size());
	}
	
	void makeSpace(size_t len);


	void ensureWritableBytes(size_t len){
		if(len>writableBytes()){
			makeSpace(len);
		}
		assert(len<=writableBytes());
	}
	char * begin(){
		return buffer_.begin().base();
	}
	const char* begin()const{
		return buffer_.begin().base();
	}

private:

	std::vector<char> buffer_; 
	size_t readIndex_;//从这里开始读消息
	size_t writeIndex_;//从这里开始写消息,也就是可读消息到此为止
	
	static const char kCRLF[];
};
