#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <shadow.h>
#include <unistd.h>
#include <crypt.h>

void chkpass(char ** argv){
	char *pass;
	pass=getpass("Password:");
	struct spwd *shadowline;
	shadowline=getspnam(argv[1]);
	char* crypted_pass=crypt(pass,shadowline->sp_pwdp);
	if(strcmp(shadowline->sp_pwdp,crypted_pass)==0){
		puts("ok!");
	}else{
		puts("faile!");
	}
	exit(0);
}












void test(char **argv){
	chkpass(argv);
}
