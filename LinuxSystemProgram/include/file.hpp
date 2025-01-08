#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glob.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <dirent.h>
#include <pwd.h>

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



void f_type(const char * fname){
	struct stat statres;
	if(stat(fname,&statres)<0){
		perror("stat{}");
		exit(1);
	}
	if(S_ISREG(statres.st_mode)){
		printf("_");
	}else if(S_ISDIR(statres.st_mode)){
		printf("d");
	}else if(S_ISSOCK(statres.st_mode)){
		printf("s");
	}else if(S_ISBLK(statres.st_mode)){
		printf("b");
	}else if(S_ISCHR(statres.st_mode)){
		printf("c");
	}else if(S_ISFIFO(statres.st_mode)){
		printf("p");
	}else{
		printf("?");
	}

}

void globL(const char * path){
	glob_t gt;
	if(glob(path,0,nullptr,&gt)){
		printf("Error");
		exit(1);
	}
	for(int i=0;i<gt.gl_pathc;++i){
		puts(gt.gl_pathv[i]);
	}
	globfree(&gt);
	exit(0);
}

void dirL(const char * path){
	DIR *dp;
	struct dirent *cur;
	dp=opendir(path);
	if(dp==nullptr)	{
		perror("opendir()");
		exit(1);
	}
	while((cur=readdir(dp))!=nullptr){
		puts(cur->d_name);
	}
	closedir(dp);
}

void uidL(char * uid){
	struct passwd *pwdline;
	pwdline=getpwuid(atoi(uid));
	puts(pwdline->pw_name);
	exit(0);
}






void test_file(char **argv){
//	dupL("tmp/test");
//	fileLength("tmp/test");
//	f_type(("tmp/file"));
//	globL("tmp/*/*");
//	dirL("tmp");
	uidL(argv[1]);
}
