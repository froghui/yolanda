# include "lib/common.h"


# define    NDG         2000    /* datagrams to send */
# define    DGLEN       1400    /* length of each datagram */
# define    MAXLINE     4096


int main(int argc, char **argv) {
    if (argc != 2) {
        err_quit("usage: udpclient <IPaddress>");
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);


    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    socklen_t server_len = sizeof(server_addr);
    struct sockaddr *reply_addr;
    reply_addr = malloc(server_len);

    char send_line[MAXLINE], recv_line[MAXLINE + 1];
    socklen_t len;
    int n;

    while (fgets(send_line, MAXLINE, stdin) != NULL) {
        int i = strlen(send_line);
        if (send_line[i - 1] == '\n'){
            send_line[i - 1] = 0;
        }

        printf("now sending %s\n", send_line);
        sendto(socket_fd, send_line, strlen(send_line), 0, (struct sockaddr *) &server_addr, server_len);

        len = 0;
        n = recvfrom(socket_fd, recv_line, MAXLINE, 0, &reply_addr, &len);
        recv_line[n] = 0;
        fputs(recv_line, stdout);
        fputs("\n",stdout);
    }

    exit(0);
}


