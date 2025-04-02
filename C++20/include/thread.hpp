#pragma once
#include <chrono>
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

void test_thread(char** argv){
	std::thread t1(fthread,1);
	t1.join();
	print("end of test()\n");
}
