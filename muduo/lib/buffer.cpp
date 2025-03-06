#include "buffer.hpp"
#include <sys/types.h>
ssize_t muduo::Buffer::readFd(int fd,int * savedErrno){
	char extrabuf[65536];
	struct iovec vec[2];
	const size_t writable = writableBytes();
	vec[0].iov_base=begin()+writerIndex_;
	vec[0].iov_len = writeable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof(extrabuf);
	const ssize_t n =readv(fd,vec,2);
	if(n<0){
		*savedErrno = errno;
	}else if(implicit_cast<size_t>(n)<= writable){
		writerIndex_+=n;
		append(extrabuf,n-writable);
	}
	return n;
}
