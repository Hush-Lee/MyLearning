#pragma once


#include "medialib.hpp"
int thr_channel_create(struct mlib_listentry_st*);
int thr_channel_destroy(struct mlib_listentry_st*);
int thr_channel_destroyall(void);
