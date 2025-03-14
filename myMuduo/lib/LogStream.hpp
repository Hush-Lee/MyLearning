#pragma once

#include "Classes.hpp"

const int kSmallBuffer=4000;
const int kLargerBuffer=4000*1000;

template<int SIZE>
class FixedBuffer:noncopyable{

private:
	const char* end()const{return data_+sizeof(data_);}
	
	static void cookieStart();
	static void cookieEnd();

	void(*cookie_)();
	char data_[SIZE];
	char*cur_;
};
