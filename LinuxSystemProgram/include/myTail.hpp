#pragma once
#include <unistd.h>
#include <string>

#include <fcntl.h>

const int BUFSIZE=4096;
int getnums(int argc,char **argv){
	int opt;
	while((opt=getopt(argc,argv, ":n"))!=-1){
		switch(opt){
			case 'n':
				return std::stoi(optarg);
			default:
				return 10;

		}
	}
	return 10;
}

void tail(int argc,char**argv){
	int n=getnums(argc,argv);
	std::string filename=argv[optind];
	int fd;
	if((fd=open(filename.c_str(),O_RDONLY))<0){
		perror("Failed to call open()");
		close(fd);
		return ;
	}
	int filesize;

	char buf[BUFSIZE];
	if((filesize=lseek(fd,0,SEEK_END))==-1){
		perror("Failed to call lseek");
		close(fd);
		return;
	}
	int lineCount=0;
	int pos=filesize;
	while(pos>0&&lineCount<=n){
		ssize_t to_read=pos<BUFSIZE?pos:BUFSIZE;
		pos-=to_read;
		if(lseek(fd,to_read,SEEK_SET)==-1){
			perror("Failed to call lseek");
			close(fd);
			return ;
		}
	}

}
