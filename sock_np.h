#ifndef  _SOCK_NP
#define _SOCK_NP

#include <netinet/in.h>

int sock_pton(int family, const char* addrstr, struct sockaddr* sock_addr);
char* sock_ntop(struct sockaddr* sock_addr, socklen_t socklen);

#endif
