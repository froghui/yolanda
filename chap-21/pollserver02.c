#include "lib/common.h"

#define INIT_SIZE 24

struct pollop {
    int event_count;
    int nfds;
    int realloc_copy;
    struct pollfd *event_set;
    struct pollfd *event_set_copy;
};

static struct pollop *my_pollop;

void init_event_set() {
    my_pollop = malloc(sizeof(struct pollop));
    my_pollop->event_count = my_pollop->nfds = my_pollop->realloc_copy = 0;

    my_pollop->event_set = malloc(INIT_SIZE * sizeof(struct pollfd));

}

void add_event_set(int fd, int event) {
    //找到一个可以记录该连接套接字的位置
    for (i = 1; i < FOPEN_MAX; i++) {
        if (client[i].fd < 0) {
            client[i].fd = connected_fd;
            client[i].events = POLLRDNORM;
            break;
        }
    }
}


int main(int argc, char **argv) {
    int listen_fd, connected_fd;
    int ready_number;
    ssize_t n;
    char buf[MAXLINE];
    struct sockaddr_in client_addr;

    listen_fd = tcp_server_listen(SERV_PORT);

    //初始化pollfd数组，这个数组的第一个元素是listen_fd，其余的用来记录将要连接的connect_fd
    struct pollfd client[FOPEN_MAX];
    client[0].fd = listen_fd;
    client[0].events = POLLRDNORM;

    // 用-1表示这个数组位置还没有被占用
    int i;
    for (i = 1; i < FOPEN_MAX; i++)
        client[i].fd = -1;
    int maxi = 0;


    for (;;) {
        if ((ready_number = poll(client, maxi + 1, -1)) < 0) {
            error(1, errno, "poll failed ");
        }

        if (client[0].revents & POLLRDNORM) {
            socklen_t client_len = sizeof(client_addr);
            connected_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);

            add_event_set(connected_fd,POLLRDNORM);

            if (i == FOPEN_MAX) {
                error(1, errno, "can not hold so many clients");
            }

            if (i > maxi)
                maxi = i;

            if (--ready_number <= 0)
                continue;
        }

        for (i = 1; i <= maxi; i++) {
            int socket_fd;
            if ((socket_fd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(socket_fd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(socket_fd);
                        client[i].fd = -1;
                    } else
                        error(1, errno, "read error");
                } else if (n == 0) {
                    close(socket_fd);
                    client[i].fd = -1;
                } else {
                    if (write(socket_fd, buf, n) < 0) {
                        error(1, errno, "write error");
                    }
                }

                if (--ready_number <= 0)
                    break;
            }
        }
    }
}
