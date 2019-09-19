#include "lib/common.h"

int socket_init()
{
    struct protoent* protocol = NULL;
    protocol=getprotobyname("icmp");
    int sock=socket(AF_INET,SOCK_RAW,protocol->p_proto);
    if(sock == -1){
        error(1,errno,"create socket failed");
    }
    return sock;
}

int main(int c, char **v) {
    printf("fd == %ld\n", htonl((uint32_t)2000));
    printf("socket %d \n", socket_init());
    printf("socket %d \n", socket_init());
}