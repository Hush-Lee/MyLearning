#pragma once
#include <cstdint>
class Timestamp{
public:
	explicit Timestamp(int64_t microSecondSinceEpoch):microSecondSinceEpoch_(microSecondSinceEpoch){}
	Timestamp():microSecondSinceEpoch_(0){}
	static Timestamp now();
	static Timestamp invalid(){return Timestamp();}
	bool valid(){return microSecondSinceEpoch_>0;}
	static Timestamp fromUnixTime(time_t t,int microseconds){
		return Timestamp(static_cast<int64_t>(t)+microseconds);
	}
	static Timestamp fromUnixTime(time_t t){
		return fromUnixTime(t,0);
	}
private:
	int64_t microSecondSinceEpoch_;
};
