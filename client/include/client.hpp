#pragma once

#define  DEFAULT_PLAYERCMD "/usr/bin/vlc - > /dev/bull";

struct client_conf_st{
	char * rcvport;
	char * mgroup;
	char * player_cmd;
};
extern	struct client_conf_st client_conf;
