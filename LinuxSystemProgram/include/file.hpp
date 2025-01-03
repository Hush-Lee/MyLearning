#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <system_error>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

void dupL(std::string name){
	int fd;
	fd=open(name.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0600);
	if(fd<0){
		perror("open failed!");
		exit(1);
	}
	int so=dup(1);
	close(1);
	dup(fd);
	close(fd);
	puts("Hello world!\n");
	fflush( stdout );
	dup2(so,1);
	puts("Hello world\n");
}


void cntlL(){}
	//fcntl manipulate file
	//ioctl manipulate device
	// /dev/fd virtual dir	

void fileLength(const char *fname){
	struct stat statres;
	if(stat(fname,&statres)<0){
		perror("stat()");
		exit(1);
	}
	printf("%ld",statres.st_size);
}









void test_file(){
	dupL("tmp/test");
	fileLength("tmp/test");
}
