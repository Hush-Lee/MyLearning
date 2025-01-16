#include <bits/getopt_core.h>
#include <cstdlib>
#include <stdio.h>
#include <server_conf.hpp>
#include <proto.h>
#include <unistd.h>
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


		}
	}
}

int main(int argc,char **argv){

}
