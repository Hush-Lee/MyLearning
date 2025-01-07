#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <glob.h>

/*static int64_t dir_s(const char * dirname){
	glob_t gt;
	int res=0;
	if(glob(dirname,0,nullptr,&gt)<0){
		perror("error in glob!");
		exit(1);
	}
	for(int i=0; i<gt.gl_pathc;++i){
		struct stat st;
		if(lstat(gt.gl_pathv[i],&st)<0){
			perror("lstat() error");
		}
		if(S_ISDIR(st.st_mode)){
			res+=dir_s(gt.gl_pathv[i]);
		}else{
			res+=st.st_blocks/2;
		}
	}
	return res;
}
*/
const int pathsize=1024;

static bool path_skip(char * path){
	char *pos;
	pos=strrchr(path,'/');
	if(pos==nullptr){
		exit(1);
	}
	if(strcmp(pos+1,".")==0||strcmp(pos+1,"..")==0){
		return true;
	}
	return false;
}


static void mydu(const char * path,int64_t& res){
	struct stat st;
	if(lstat(path,&st)<0){
		perror("lstat() error");
		exit(1);
	}
	if(!S_ISDIR(st.st_mode)){
		res+=st.st_blocks;
		return;
	}
	char nextpath[pathsize];
	strncpy(nextpath,path,pathsize);
	strcat(nextpath,"/*");
	glob_t gt;
	if(glob(nextpath,0,nullptr,&gt)<0){
		perror("glob() error");
		exit(1);
	}
	strncpy(nextpath,path,pathsize);
	strcat(nextpath,"/.*");
	if(glob(nextpath,GLOB_APPEND,nullptr,&gt)<0){
		perror("glob() error");
		exit(1);
	}
	res+=st.st_blocks;
	for(int i=0;i<gt.gl_pathc;++i){
		if(path_skip(gt.gl_pathv[i])){
			continue;
		}else{
			mydu(gt.gl_pathv[i],res);
		}
	}

}


int main(int argc,char**argv){
	if(argc<2){
		printf("Usage mydu [FilePath]");
		exit(1);
	}
	int64_t res=0;
	mydu(argv[1],res);
	printf("%ld\t%s",res/2,argv[1]);
}
