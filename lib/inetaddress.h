#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

struct inet_address{
    int port;
};

struct inet_address * init_inet_address(int port);

#endif