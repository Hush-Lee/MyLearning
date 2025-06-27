#include <filesystem>
#include <iostream>
int main(){
	for(auto fd:std::filesystem::directory_iterator(".")){
		if(fd.is_directory()){
			std::cout<<fd.path()<<std::endl;
		}
	}
	auto items=std::filesystem::directory_entry();
	std::cout<<items;
}
