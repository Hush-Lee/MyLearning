#pragma once

#include "medialib.hpp"
#include <sys/types.h>
int thr_list_create(struct mlib_listentry_st*,int );
void thr_list_destroy();
