#pragma ocne
#define MYTBF_MAX 1024
typedef void mytbf_t;

mytbf_t *mytbf_init(int cpas,int brust);
int mytbt_fetchtoken(mytbf_t *,int );
int mytbf_returntoken(mytbf_t *,int );
int mytbf_destory(mytbf_t *);
