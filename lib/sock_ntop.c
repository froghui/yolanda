//
// Created by shengym on 2019-07-07.
//

#include    "common.h"

char *
sock_ntop(const struct sockaddr_in *sin, socklen_t salen) {
    char portstr[8];
    static char str[128];        /* Unix domain is largest */


    if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
        return (NULL);
    if (ntohs(sin->sin_port) != 0) {
        snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
        strcat(str, portstr);
    }
    return (str);
}