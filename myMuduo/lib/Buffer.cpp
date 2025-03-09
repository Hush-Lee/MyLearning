#include "Buffer.hpp"
#include <cerrno>
#include <cstddef>
#include <iterator>
#include <sys/types.h>
#include <sys/uio.h>
ssize_t Buffer::readFd(int fd,int* savedErr){
	char extrabuf[65535];
	struct iovec vec[2];
	const size_t writable=writableByess();
	vec[0].iov_base =beginWrite();
	vec[0].iov_len = writable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len=sizeof(extrabuf);
	const ssize_t n  =readv(fd,vec,2);
	if(n<0){
		*savedErr=errno;
	}else if(static_cast<size_t>(n)<writable){
		writeIndex_+=n;
	}else{
		writeIndex_=buffer_.size();
		append(extrabuf,n-writable);
	}
	return n;
}

ssize_t Buffer::writeFd(int fd,int* savedErr){

}

void Buffer::retrieve(size_t len){
	if(len<readableBytes()){
		readIndex_+=len;
	}else{
		retrieveAll();
	}
}

void Buffer::retrieveAll(){
	writeIndex_=kCheapPrepend;
	readIndex_=kCheapPrepend;
}

char* Buffer::beginWrite(){
	return buffer_.data()+writeIndex_;
}

char* Buffer::peek(){
	return buffer_.data()+readIndex_;
}


size_t Buffer::writableByess()const{
	return buffer_.size()-writeIndex_;
}

size_t Buffer::readableBytes()const{
	return writeIndex_-readIndex_;
}
