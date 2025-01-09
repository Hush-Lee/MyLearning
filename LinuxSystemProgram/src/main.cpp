#include "../include/file.hpp"
#include "../include/fun.hpp"
int main(int argc,char**argv){
	if(argc<2){
		fprintf(stderr,"Usage ....");
		exit(1);
	}
//	test_file(argv);
	test(argv);
	return 0;
}
