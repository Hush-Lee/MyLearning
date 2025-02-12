#pragma once

class nocopyable{
public:
	nocopyable()=default;
private:
	nocopyable(nocopyable& t);
	nocopyable(nocopyable&& t);
	nocopyable& operator =(nocopyable& t);
	nocopyable& operator =(nocopyable&& t);
};
