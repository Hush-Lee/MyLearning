#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <unistd.h>

#include "medialib.hpp"
#include "mytbf.hpp"
#include "../include/proto.hpp"

#define PATHSIZE 1024
struct channel_context_st{
	chnid_t chnid;
	char * desc;
	glob_t mp3glob;
	int pos;
	int fd;
	off_t offset;
	mytbf_t *tbf;
};
struct channel_context_st channel[MAXCHNID];
int mlib_getchnlist(struct mlib_listentry_st **result, int *resnum){
	char path[PATHSIZE];
	for(int i=0;i<=MAXCHNID;++i){
		channel[i].chnid=-1;
			
	}
	snprintf(path,PATHSIZE,"%s",server_conf.media_dir);
	glob_t globes;
	if(glob(path,0,nullptr,&globes)!=0){
		return -1;
	}
	int num=0;
	for(int i=0;i<globes.gl_pathc;++i){
		path2entry(globes.gl_pathv[i]);
		++num;
	}
	*resnum=num;
	
}
int mlib_freechnlist(struct mlib_listentry_st *){

}

ssize_t mlib_readchn(uint8_t, void *, size_t)
