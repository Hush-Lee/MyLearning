#pragma once
#include <cstdint>
#include <utility>
class Timestamp{
public:
	explicit Timestamp(int64_t microSecondSinceEpoch):microSecondSinceEpoch_(microSecondSinceEpoch){}
	Timestamp():microSecondSinceEpoch_(0){}
	static Timestamp now();
	static Timestamp invalid(){return Timestamp();}
	bool valid(){return microSecondSinceEpoch_>0;}
	
	void swap(Timestamp& target){
		std::swap(this->microSecondSinceEpoch_,target.microSecondSinceEpoch_);
	}

	std::string toString()const;

	std::string toFormattedString(bool showMicroseconds=true)const;

	time_t secondsSinceEpoch(){
		return static_cast<time_t>(microSecondSinceEpoch_/kMicrosecondsPerSecond);
	}

	static Timestamp fromUnixTime(time_t t,int microseconds){
		return Timestamp(static_cast<int64_t>(t)+microseconds);
	}
	static Timestamp fromUnixTime(time_t t){
		return fromUnixTime(t,0);
	}
	static const int kMicrosecondsPerSecond=1000*1000;
	int64_t microSecondSinceEpoch(){
		return microSecondSinceEpoch_;
	}
private:
	int64_t microSecondSinceEpoch_;
};

inline bool operator <(Timestamp lhs,Timestamp rhs){
	return lhs.microSecondSinceEpoch()<rhs.microSecondSinceEpoch();
}

inline bool operator ==(Timestamp lhs,Timestamp rhs){
	return lhs.microSecondSinceEpoch()==rhs.microSecondSinceEpoch();
}

inline double timeDifference(Timestamp high,Timestamp low){
	auto delta=high.microSecondSinceEpoch()-low.microSecondSinceEpoch();
	return static_cast<double>(delta)/Timestamp::kMicrosecondsPerSecond;
}

inline Timestamp addTime(Timestamp t,int seconds){
	auto delta=static_cast<int64_t>(seconds)*Timestamp::kMicrosecondsPerSecond;
	return Timestamp(delta+t.microSecondSinceEpoch());
}
