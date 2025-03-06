#pragma once
#include "common.hpp"
#include <string>
#include <sys/types.h>
namespace muduo {
	class Buffer{
		public:
			ssize_t readFd(int fd,int * savedErrno);
		private:
			std::string buffer_;
	};
};
