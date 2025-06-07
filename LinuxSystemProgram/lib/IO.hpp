#pragam once
//write() read() 都是从缓冲区读取和写入，中间异步会导致数据污染
//缓冲区大小会影响读写的速度，一般越大越快，但是过大反而降低
//setvbuf(FILE* stream, chr * buf,int mode,size_t size);设置缓冲区
#include <stdio.h>
#include <stdlib.h>
void Setvbuf(){
    static char buf[1024];
    if(setvbuf(stdout,buf,_IOFBF,1024)!=0){
        exit(0);
    }
}

//
