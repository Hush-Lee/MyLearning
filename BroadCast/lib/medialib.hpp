#pragma once 

struct mlib_listentry_st{
	chnid_t chnid;
	char * desc;
};

int mlib_getchnlist(struct mlib_listentry_st**,int *);


