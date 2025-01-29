#include <bits/getopt_core.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <syslog.h>
#include <cstdlib>
#include <stdio.h>
#include <server_conf.hpp>
#include <proto.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <net/if.h>
// -M	指定多播组
// -P	指定端口
// -F	前台运行
// -D	指定媒体库位置
// -I	指定网络设备
// -H	显示帮助

struct server_conf_st server_conf = {.rcvport=DEFALUT_RCVPORT,\
					.mgroup=DEFALUT_MGROUP,\
					.media_dir=DEFALUT_MEDIADIR,\
					.runmode=RUN_DAEMON,\
					.ifname=DEFALUT_IF};

static void print_help(){
	fprintf(stdout,"-M	指定多播组\n -P	指定端口\n -F	前台运行\n -D	指定媒体库存位置\n -I	指定网络设备\n  -H	显示帮助\n");
}


static void set_opt(int argc,char ** argv){
	while (1) {
		int c=getopt(argc,argv, "M:P:F:D:I:H");
		switch (c) {
			case 'M':
				server_conf.mgroup=optarg;
				break;
			case 'P':
				server_conf.rcvport=optarg;
				break;
			case 'F':
				server_conf.runmode=RUN_FOREGROUND;
				break;
			case 'D':
				server_conf.media_dir=optarg;
				break;
			case 'I':
				server_conf.ifname=optarg;
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
}
static void daemon_exit(int s){
	closelog();
	exit(0);
}

static int daemonize(void){
	pid_t pid;
	pid = fork();
	if(pid < 0){
//		perror("fork()");
		syslog(LOG_ERR,"fork():%s",strerror(errno))
		return -1;
	}
	if(pid > 0 ){
		exit(0);
	}
	int fd = open("/dev/null",O_RDWR);
	if(fd<0){
//		perror("open");
		syslog(LOG_WARNING,"open() %s",strerror(errno));
		return -2;
	}
	dup2(fd,0);
	dup2(fd,1);
	dup2(fd,2);
	if(fd>2){
		close(fd);
	}
	setsid();
	chdir("/");
	umask(0);
}
static int socket_init(void){
	int serv_sd=socket(AF_INET,SOCK_DGRAM,0);
	if(serv_sd<0){
		syslog(LOG_ERR,"socket():%s",strerror(errno));
		exit(1);
	}
	struct ip_mreqn mreq;
	inet_pton(AF_INET,server_conf.mgroup,&mreq.imr_multiaddr);
	inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
	mreq.imr_ifindex = if_nametoindex(server_conf.ifname);


	if(setsockopt(serv_sd,IPPROTO_IP,IP_MULTICAST_IF, &mreq,sizeof(mreq))<0){
		syslog(LOG_ERR, "setsockopt()%s",strerror(errno));
		exit(1);
	}
}

int main(int argc,char **argv){

	struct sigaction sa;
	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGINT);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGTERM);
	

	sigaction(SIGTERM,&sa,nullptr);
	sigaction(SIGINT,&sa,nullptr);
	sigaction(SIGQUIT,&sa,nullptr);

	set_opt(argc,argv);
	openlog("netradio",LOG_PID|LOG_PERROR,LOG_DAEMON);
	if(server_conf.runmode==RUN_DAEMON){
		if(daemonize()!=0){
			exit(1);
		}
	}else if(server_conf.runmode==RUN_FOREGROUND){

	}else{
//		fprintf(stderr,"EINVAL\n");
		syslog(LOG_ERR,"EINVAL server_conf.runmode.");
		exit(1);
	}
	socket_init();
	
	struct mlib_listentry_st *list;
	int list_size;
	int err=mlib_getchnlist( &list, &list_size);

	thr_list_create(list,list_size);

	int i;
	for(i = 0;i<list_size;++i){
		thr_channel_create(list + i);
	}

	syslog(LOG_DEBUG, "%d channel threads created.",i);
	while(1){
		pause();
	}
	closelog();
}
