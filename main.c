#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#include "sock_np.h"

int main(const int args, char** argv)
{
    if (args < 2) {
        exit(0);
    }

    int family;
    if (strcmp(argv[1], "4") == 0) {
        family = AF_INET;
    } else if (strcmp(argv[1], "6") == 0) {
        family = AF_INET6;
    }

    struct sockaddr_storage addr;
    int n;
    if ( (n = sock_pton(family, argv[2], (struct sockaddr*)&addr)) < 0) {
        perror("sock_pton error");
    } else if (n == 0) {
        printf("invalid address\n");
    } else {
        ((struct sockaddr_in*)&addr)->sin_port = ntohs(10);
        printf("%s\n", sock_ntop((struct sockaddr*)&addr, addr.ss_len));
    }
    exit(0);
}
