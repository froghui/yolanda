//
// Created by shengym on 2019-07-13.
//

//
// Created by shengym on 2019-07-07.
//

#include "lib/common.h"

static int count;

static void recvfrom_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}


int main(int argc, char **argv) {
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    socklen_t client_len;
    char message[MAXLINE];
    message[0] = 0;
    count = 0;

    signal(SIGINT, recvfrom_int);

    struct sockaddr_in client_addr;
    client_len = sizeof(client_addr);

    int n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *) &client_addr, &client_len);
    if (n < 0) {
        error(1, errno, "recvfrom failed");
    }
    message[n] = 0;
    printf("received %d bytes: %s\n", n, message);

    if (connect(socket_fd, (struct sockaddr *) &client_addr, client_len)) {
        error(1, errno, "connect failed");
    }

    while (strncmp(message, "goodbye", 7) != 0) {
        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);

        size_t rt = send(socket_fd, send_line, strlen(send_line), 0);
        if (rt < 0) {
            error(1, errno, "send failed ");
        }
        printf("send bytes: %zu \n", rt);

        size_t rc = recv(socket_fd, message, MAXLINE, 0);
        if (rc < 0) {
            error(1, errno, "recv failed");
        }

        count++;
    }

    exit(0);
}


