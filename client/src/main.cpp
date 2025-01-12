#include <client.hpp>
#include <proto.hpp>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
 *  -M --mgroup 指定多播组
 *  -P --port   指定接受端口
 *  -p --player 指定播放器
 *  -H --help   显示帮助
 */

struct client_conf_st client_conf={\
	.rcvport=DEFAULT_RCVPORT,\
	.mgroup=DEFAULT_MGROUP,\
	.player_cmd=DEFAULT_PLAYERCMD};

static void print_help(){
	printf("-P --port 指定接收端口\n -M --mgroup 指定多播组\n -p --player 指定播放器命令行\n -H --help 显示帮助\n");
}
int main(int argc,char**argv){
	int index=0;
	struct option argarr[]={{"port",1,nullptr,'P'},\
		{"mgroup",1,nullptr,'M'},\
		{"player",1,nullptr,'p'},\
		{"help",0,nullptr,'H'},\
		{nullptr,0,nullptr,0}};
	while(true){
		int c=getopt_long(argc,argv,"MPpH",argarr,&index);
		if(c<0){
			break;
		}
		switch(c){
			case 'P':
				client_conf.rcvport = optarg;
				break;
			case 'M':
				client_conf.mgroup = optarg;
				break;
			case 'p':
				client_conf.player_cmd = optarg;
				break;
			case 'H':
				print_help();
				exit(0);
				break;
			default:
				abort();
				break;
		}
	}
	
	int sd=socket(AF_INET,SOCK_DGRAM,0);
	if(sd<0){
		perror("socket() error");
		exit(1);
	}

	struct ip_mreqn mreq;
	if(inet_pton(AF_INET,client_conf.mgroup,&mreq.imr_multiaddr)!=1){
		perror("inet_pton error!");
		exit(1);
	}
	if(inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address)!=1){
		perror("inet_pton() error");
		exit(1);
	}
	mreq.imr_ifindex=if_nametoindex("eth0");

	if(setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))<0){
		perror("setsockopt()");
		exit(1);
	}
	int val = 1;
	if(setsockopt(sd, IPPROTO_IP,IP_MULTICAST_LOOP,&val,sizeof(val))<0){
		perror("setsockopt");
		exit(1);
	}
	struct sockaddr_in laddr;
	laddr.sin_family=AF_INET;
	laddr.sin_port=htons(atoi(client_conf.rcvport));
	inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
	
	if(bind(AF_INET,(struct sockaddr*)&laddr,sizeof(laddr))<0){
		perror("bind()");
		exit(1);
	}

	int pd[2];
	if(pipe(pd)<0){
		perror("pipe()");
		exit(1);
	}

	int pid = fork();
	if(pid<0){
		perror("fork()");
		exit(1);
	}
	if(pid==0){

	}




}
