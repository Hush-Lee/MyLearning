#pragma once


#define		DEFALUT_MEDIADIR	"/var/media/" 
#define		DEFALUT_IF 		"eth0" 

enum{
	RUN_DAEMON=1,
	RUN_FOREGROUND
};

struct server_conf_st{
	char * rcvport;
	char * mgroup;
	char * media_dir;
	char   runmode;
	char * ifname;
};
extern server_conf_st server_conf;
