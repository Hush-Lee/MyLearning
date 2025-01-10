#pragma once

#include <stdlib.h>
#include "site_type.hpp"
const char* DEFAULT_MGROUP = R"224.2.2.2";
const char* DEFAULT_RCVPORT = R"1989";
const int CHNNR = 100;
const int MINCHNID = 1;
const int LISTCHNID =0;
const int MAXCHNID = MINCHNID+CHNNR-1;
const int MSG_CHANNEL_MAX=65536-20-8;
const int MSG_LIST_MAX = 65535 -20-8;
const int MAX_ENTRY=MSG_LIST_MAX-sizeof(chnid_t);

inline int maxData(){
	return MSG_CHANNEL_MAX - sizeof(chnid_t);
}


struct msg_channel_st{
	chnid_t chnid;
	uint8_t data[1];
}__attribute__((packed));

struct msg_listentry_st{
	chnid_t chnid;
	uint8_t desc[1];
}__attribute__((packed));


struct msg_list_st{
	chnid_t chnid;
	struct msg_listentry_st entry[1];
}__attribute__((packed));
