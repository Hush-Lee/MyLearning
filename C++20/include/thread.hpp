#pragma once
#include <chrono>
#include <cmath>
#include <cstdint>
#include <future>
#include <thread>
void sleepForMs(const unsigned ms){
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
void fthread(const int n){
	print("this is t {}",n);
	for(size_t i=0;i<5;++i){
		sleepForMs(100*n);
		print("t{}:{}\n",n,i+1);
	}
	print("Finshing t{}\n",n);
}



using launch=std::launch;
using secs = std::chrono::duration<double>;
struct prime_time{
	secs dur{};
	uint64_t count;
};
prime_time count_prime(const uint64_t&max){
	prime_time ret;
	constexpr auto isPrime=[](const uint64_t&n){
		for(uint64_t i{2};i<std::sqrt(n);++i){
			if(n%i==0){
				return false;
			}
		}
		return true;
	};
	uint64_t start{2};
	uint64_t end{max};
	auto t1=std::chrono::steady_clock::now();
	for(uint64_t i{start};i<=end;++i){
		if(isPrime(i)){
			++ret.count;
		}
	}
	ret.dur=std::chrono::steady_clock::now()-t1;
	return ret;
}

void test_async(){
	constexpr uint64_t MAX_PRIME{0x1FFFF};
	auto primes1=std::async(count_prime,MAX_PRIME);
	auto pt=primes1.get();
	print("primes: {} {:.3}\n",pt.count,pt.dur);
}





void test_thread(char** argv){
	test_async();
	std::thread t1(fthread,1);
	t1.join();
	print("end of test()\n");
}
