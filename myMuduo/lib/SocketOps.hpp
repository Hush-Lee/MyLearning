#pragma once

#include <sys/socket.h>
int createNonblockingOrDie(sa_family_t family);
int connect(int sockfd, const struct sockaddr *addr);
