#include "Exception.hpp"
#include "CurrentThread.hpp"
#include <string>
Exception::Exception(std::string message):
	message_(std::move(message)),
	stack_(CurrentThread::stackTrace(/*demangle*/false)){
	
}
