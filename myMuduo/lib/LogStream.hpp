#pragma once

#include "Classes.hpp"
#include <cstring>

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

	void append(const char* buf,size_t len){
		if(static_cast<size_t>(avail())>len){
			memcpy(cur_, buf, len);
			cur_+=len;
		}
	}
	const char* current(){return cur_;}
	int avail()const{return static_cast<int>(end()-cur_);}


	const char* debugString();
	const char* data(){return data_;}
	void reset(){cur_=data_;}
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





class LogStream:noncopyable{
	using self=LogStream;
public:
	using Buffer=FixedBuffer<kSmallBuffer>;
	self& operator<<(bool v){
	    buffer_.append(v ? "1" : "0", 1);
	    return *this;
	  }

	  self& operator<<(short);
	  self& operator<<(unsigned short);
	  self& operator<<(int);
	  self& operator<<(unsigned int);
	  self& operator<<(long);
	  self& operator<<(unsigned long);
	  self& operator<<(long long);
	  self& operator<<(unsigned long long);

	  self& operator<<(const void*);

	  self& operator<<(float v)
	  {
	    *this << static_cast<double>(v);
	    return *this;
	  }
	  self& operator<<(double);
	  // self& operator<<(long double);

	  self& operator<<(char v)
	  {
	    buffer_.append(&v, 1);
	    return *this;
	  }

	  // self& operator<<(signed char);
	  // self& operator<<(unsigned char);

	  self& operator<<(const char* str)
	  {
	    if (str)
	    {
	      buffer_.append(str, strlen(str));
	    }
	    else
	    {
	      buffer_.append("(null)", 6);
	    }
	    return *this;
	  }

	  self& operator<<(const unsigned char* str)
	  {
	    return operator<<(reinterpret_cast<const char*>(str));
	  }

	  self& operator<<(const std::string& v)
	  {
	    buffer_.append(v.c_str(), v.size());
	    return *this;
	  }

	  self& operator<<(const std::string_view& v)
	  {
	    buffer_.append(v.data(), v.size());
	    return *this;
	  }

	  self& operator<<(const Buffer& v)
	  {
	    *this << v.toString_view();
	    return *this;
	  }

	  void append(const char* data, int len) { buffer_.append(data, len); }
	  const Buffer& buffer() const { return buffer_; }
	  void resetBuffer() { buffer_.reset(); }


private:
	void staticCheck();
	template<typename T>
	void formatInteger(T);
	Buffer buffer_;
	static const  int kMaxNumberSize{48};
};

