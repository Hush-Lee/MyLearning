#pragma once
#include "common.hpp"
#include <chrono>
#include <filesystem>
#include <ratio>
#include <string>
#include <format>
void timer(std::string(*fun)()){
	auto t1=std::chrono::high_resolution_clock::now();
	std::string res=fun();
	auto t2=std::chrono::high_resolution_clock::now();
	std::chrono::duration<double,std::milli>dura=t2-t1;
	print("{}\n",res);
	print("duration:{}\n",dura);
	
}

std::string append_str(){
	std::string a{"a"};
	std::string b{"b"};
	long n=0;
	while(++n){
		std::string res;
		res.append(a);
		res.append(", ");
		res.append(b);
		res.append("\n");
		if(n>1000000){
			return res;
		}
	}
	return "error\n";
}
std::string add_str(){
	std::string a{"a"};
	std::string b{"b"};
	long n=0;
	while(++n){
		std::string res=a+", "+b+"\n";
		if(n>10000000){
			return res;
		}
	}
	return "error\n";
}

std::string format_str(){
	std::string a{"a"};
	std::string b{"b"};
	long n=0;
	while(++n){
		std::string res;
		res=std::format("{}, {} \n",a,b);
		if(n>1000000){
			return res;
		}
	}
	return "error\n";
}

void test_string(char**argv){
	timer(append_str);
	timer(add_str);
	timer(format_str);
}

