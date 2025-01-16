#include <cerrno>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <sys/wait.h>
#define __ACCESS_ONCE(x) ({ \
         __maybe_unused typeof(x) __var = (__force typeof(x)) 0; \
        (volatile typeof(x) *)&(x); })
#define ACCESS_ONCE(x) (*__ACCESS_ONCE(x))
void waitall(void){
	int pid;
	int status;
	for(;;){
		pid = wait(&status);
		if(pid == -1){
			if(errno == ECHILD){
				break;
			}
			perror("wait");
			exit(-1);
		
		}
	}
}

void test_f(){
	int x=0;
	int pid;
	pid =fork();
	if(pid == 0){
		x=1;
		printf("Child process set x = 1");
		exit(0);
	}
	if(pid<0){
		perror("fork ()");
		exit(-1);
	}
	waitall();
	printf("Parent process sees x=%d\n",x);
}
int x=1;
void * mythread(void *arg){
	x=1;
	printf("Child process set x = 1 \n");
	return nullptr;
}
void test_p(){
	pthread_t tid;
	void *vp;
	if(pthread_create(&tid,nullptr,mythread,nullptr)!=0){
		perror("pthread_create");
		exit(1);
	}
	if(pthread_join(tid, &vp)!=0){
		perror("pthread_join()");
		exit(-1);
	}
	printf("Parent process sees x = %d\n",x);
}

void * lock_reader(void *arg){
	int i;
	int newx=-1;
	int oldx=-1;
	pthread_mutex_t *pmlp = (pthread_mutex_t*)arg;
	if(pthread_mutex_lock(pmlp)!=0){
		perror("lock_reader:pthread_mutex_lock");
		exit(-1);
	}
	for(i=0;i<100;++i){
		newx = ACCESS_ONCE(x);
		if(newx != oldx){
			printf("lock_reader() : x = %d\n",newx);
		}
		oldx=newx;
		poll(nullptr,0,1);
	}
	if(pthread_mutex_unlock(pmlp)!=0){
		perror("lock_reader : pthread_mutex_unlock");
		exit(-1);
	}
	return nullptr;
}

void * lock_writer(void * arg){
	int i;
	pthread_mutex_t *pmlp=(pthread_mutex_t*)arg;
	if(pthread_mutex_lock(pmlp)!=0){
		perror("lock_writer : pthread_mutex_lock");
		exit(-1);
	}
	for(i=0;i<3;++i){
		++ACCESS_ONCE(x);
		poll(nullptr,0,5);
	}
	if(pthread_mutex_unlock(pmlp)!=0){
		perror("lock_write : pthread_mutex_unlock");
		exit(-1);
	}
	return nullptr;
}


void test_lock(){
	pthread_mutex_t* locker;
	lock_reader(locker);
	lock_writer(locker);
}

void test(){
	test_lock();
}
