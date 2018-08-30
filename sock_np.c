#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include "sock_np.h"

int 
sock_pton(int family, const char* addrstr, struct sockaddr* sock_addr)
{
    int n = -1;
    char inet4addr[INET_ADDRSTRLEN];
    char inet6addr[INET6_ADDRSTRLEN];
    
    switch (family) {
    case AF_INET: {
        struct sockaddr_in *addr = (struct sockaddr_in*)sock_addr;
        if ((n = inet_pton(AF_INET, addrstr, &(addr->sin_addr))) == 0) {
            n = inet_aton(addrstr, &(addr->sin_addr));
        }
        addr->sin_family = AF_INET;
        break;
    }
    case AF_INET6: {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6*)sock_addr;
        if ( (n = inet_pton(AF_INET6, addrstr, &(addr6->sin6_addr))) == 0 ) {
            struct sockaddr_in *addr = (struct sockaddr_in*)sock_addr;
            if ( (n = inet_aton(addrstr, &(addr->sin_addr))) == 1 ) {
                const u_char* p = (u_char*)&(addr->sin_addr);
                snprintf(inet6addr, INET6_ADDRSTRLEN, "::ffff:%d.%d.%d.%d",
                         p[0], p[1], p[2], p[3]);
                n = inet_pton(AF_INET6, inet6addr, &(addr6->sin6_addr));
            }
        }
        addr6->sin6_family = AF_INET6;
        break;
    }
    default:
        break;
    }
    return n;
}

char*
sock_ntop(struct sockaddr* sock_addr, socklen_t socklen)
{
    char portstr[8];
    static char str[128];

    switch (sock_addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in*)sock_addr;
        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL) {
            return NULL;
        }
        if (ntohs(sin->sin_port) != 0) {
            snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            strcat(str, portstr);
        }
        return str;
    }
    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)sock_addr;
        if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL) {
            return NULL;
        }
        if (ntohs(sin6->sin6_port) != 0) {
            snprintf(portstr, sizeof(portstr), "[:%d]", ntohs(sin6->sin6_port));
            strcat(str, portstr);
        }
        return str;
    }
    default:
        break;
    }
    return NULL;
}
