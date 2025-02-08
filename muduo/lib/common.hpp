class nocopyable{
	nocopyable(nocopyable& t);
	nocopyable(nocopyable&& t);
	nocopyable& operator =(nocopyable& t);
	nocopyable& operator =(nocopyable&& t);
};
