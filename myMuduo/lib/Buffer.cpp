#include "Buffer.hpp"
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <iterator>
#include <sys/types.h>
#include <sys/uio.h>
ssize_t Buffer::readFd(int fd,int* savedErr){
	char extrabuf[65535];
	struct iovec vec[2];
	const size_t writable=writableBytes();
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


void Buffer::retrieve(size_t len){
	if(len<readableBytes()){
		readIndex_+=len;
	}else{
		retrieveAll();
	}
}



void Buffer::makeSpace(size_t len)
  {
    if (writableBytes() + prependableBytes() < len + kCheapPrepend)
    {
      // FIXME: move readable data
      buffer_.resize(writeIndex_+len);
    }
    else
    {
      // move readable data to the front, make space inside buffer
      assert(kCheapPrepend < readIndex_);
      size_t readable = readableBytes();
      std::copy(begin()+readIndex_,
                begin()+writeIndex_,
                begin()+kCheapPrepend);
      readIndex_ = kCheapPrepend;
      writeIndex_ = readIndex_ + readable;
      assert(readable == readableBytes());
    }
  }


