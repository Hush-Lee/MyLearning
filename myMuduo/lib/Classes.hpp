#pragma once
#include <cstring>

inline void memZero(void * p,size_t n){
	memset(p, 0, n);
}

template <typename To,typename From>
inline To implicit_cast(From const &f){
	return f;
}

class noncopyable{
protected:
	noncopyable()=default;
	~noncopyable()=default;
public:
	noncopyable(const noncopyable&)=delete;
	void  operator=(const noncopyable&)=delete;
};
