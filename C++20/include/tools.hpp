#pragma once
#include <cmath>
#include <optional>
std::optional<int> isprime(int num){
	int end=std::sqrt(num);
	for(int i=2;i<=num;++i){
		if(num%i==0){
			return {};
		}
	}
	return num;
}
void test_optional(char**argv){
	std::optional<int>a{};
	std::optional<int>b{2};
	print("{}+{}={}\n",*a,*b,*a+*b);
	print("{} is {}\n",18,isprime(18)?"prime":"not prime");
	//a.reset();
	//print("a={}\n",a.value());
	//above code can result runtime exception,because value() will check optional is not null
}
void test_tools(char**argv){
	test_optional(argv);
};

