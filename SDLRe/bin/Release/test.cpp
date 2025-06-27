#include<nlohmann/json.hpp>
#include<fstream>
#include<iostream>
int main(){
	std::fstream in("assets/Role.json");
	nlohmann::json config;
	in>>config;
	std::cout<<config["locate"]["x"].get<int>();
}

