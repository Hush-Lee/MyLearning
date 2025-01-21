#include "mytbf.hpp"
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <pthread.h>

struct mytbf_st{
	int cps;
	int brust;
	int token;
	int pos;
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

static struct mytbf_st *job[MYTBF_MAX];
static pthread_mutex_t mut_job=PTHREAD_MUTEX_INITIALIZER;

static int get_free_pos_unlocked(void){
	int i;
	for(i=0;i<MYTBF_MAX;++i){
		if(job[i]==nullptr){
			return i;
		}
	}
	return -1;
}


mytbf_t *mytbf_init(int cps,int brust){
	struct mytbf_st *me = new mytbf_st();
	me->cps=cps;
	me->brust=brust;
	me->token=0;
	pthread_mutex_init(&me->mut, nullptr);
	pthread_cond_init(&me->cond,nullptr);
	pthread_mutex_lock(&mut_job);
	int pos=get_free_pos_unlocked();
	if(pos<0){
		pthread_mutex_unlock(&mut_job);
		delete me;
		return nullptr;
	}

	me->pos=pos;
	job[me->pos]=me;
	pthread_mutex_unlock(&mut_job);
	return me;
}
