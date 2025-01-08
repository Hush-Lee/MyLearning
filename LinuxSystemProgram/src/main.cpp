#include "../include/file.hpp"
int main(int argc,char**argv){
	if(argc<2){
		printf("Usage ....");
		exit(1);
	}
	test_file(argv);
	return 0;
}
