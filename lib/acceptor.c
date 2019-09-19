#include <assert.h>
#include "acceptor.h"

struct acceptor *acceptor_init(int port) {
    struct acceptor *acceptor1 = malloc(sizeof(struct acceptor));
    acceptor1->listen_port = port;
    acceptor1->listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    make_nonblocking(acceptor1->listen_fd);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int on = 1;
    setsockopt(acceptor1->listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rt1 = bind(acceptor1->listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rt1 < 0) {
        error(1, errno, "bind failed ");
    }

    int rt2 = listen(acceptor1->listen_fd, LISTENQ);
    if (rt2 < 0) {
        error(1, errno, "listen failed ");
    }

//    signal(SIGPIPE, SIG_IGN);

    return acceptor1;
}