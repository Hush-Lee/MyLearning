#pragma once

#include "Classes.hpp"

const int kSmallBuffer=4000;
const int kLargerBuffer=4000*1000;

template<int SIZE>
class FixedBuffer:noncopyable{
	FixedBuffer():cur_(data_){
		setCookie(cookieStart);
	}
	~FixedBuffer(){
		setCookie(cookieEnd);
	}

	void bzero(){memZero(data_,sizeof(data_));}
	void add(size_t len){
		cur_+=len;
	}
	int length()const{return static_cast<int>(cur_-data_);}
	void setCookie(void (*cookie)()){
		cookie_=cookie;
	}
	std::string toString()const{return std::string(data_,length());}
	std::string_view toString_view()const{return std::string_view(data_,length());}
private:
	const char* end()const{return data_+sizeof(data_);}
	
	static void cookieStart();
	static void cookieEnd();

	void(*cookie_)();
	char data_[SIZE];
	char*cur_;
};
