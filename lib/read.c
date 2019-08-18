#include "common.h"

size_t readn(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int length = size;

    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            if (errno == EINTR)
                continue;     /* 考虑非阻塞的情况，这里需要再次调用read */
            else
                return (-1);
        } else if (result == 0)
            break;                /* EOF(End of File)表示套接字关闭 */

        length -= result;
        buffer_pointer += result;
    }
    return (size - length);        /* 返回的是实际读取的字节数*/
}


size_t readline_2(int fd, char *buffer, size_t length) {
    char *buf_first = buffer;

    char c;
    while (length > 0 && recv(fd, &c, 1, 0) == 1) {
        *buffer++ = c;
        length--;
        if (c == '\n') {
            *buffer = '\0';
            return buffer - buf_first;
        }
    }

    return -1;

}

/* readline - read a newline terminated record */
/* 123456789\n */
size_t readline(int fd, char *buffer, size_t length) {
    char *buf_first = buffer;
    static char *buffer_pointer;
    int nleft = 0;
    static char read_buffer[512];
    char c;

    while (--length > 0) {
        // nread == 0
        if (nleft <= 0) {
            int nread = recv(fd, read_buffer, sizeof(read_buffer), 0);
            if (nread < 0) {
                if (errno == EINTR) {
                    length++;
                    continue;
                }
                return -1;
            }
            if (nread == 0)
                return 0;
            buffer_pointer = read_buffer;
            nleft = nread;
        }
        c = *buffer_pointer++;
        *buffer++ = c;
        nleft--;
        if (c == '\n') {
            *buffer = '\0';
            return buffer - buf_first;
        }
    }
    return -1;
}

size_t read_message(int fd, char *buffer, size_t length) {
    u_int32_t msg_length;
    u_int32_t msg_type;
    int rc;

    /* Retrieve the length of the record */

    rc = readn(fd, (char *) &msg_length, sizeof(u_int32_t));
    if (rc != sizeof(u_int32_t))
        return rc < 0 ? -1 : 0;
    msg_length = ntohl(msg_length);

    rc = readn(fd, (char *) &msg_type, sizeof(msg_type));
    if (rc != sizeof(u_int32_t))
        return rc < 0 ? -1 : 0;

    /* 判断buffer是否可以容纳下数据  */
    if (msg_length > length) {
        return -1;
    }

    /* Retrieve the record itself */
    rc = readn(fd, buffer, msg_length);
    if (rc != msg_length)
        return rc < 0 ? -1 : 0;
    return rc;
}


int read_line(int fd, char *buf, int size) {
    int i = 0;
    char c = '\0';
    int n;

    while ((i < size - 1) && (c != '\n')) {
        n = recv(fd, &c, 1, 0);
        if (n > 0) {
            if (c == '\r') {
                n = recv(fd, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                    recv(fd, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        } else
            c = '\n';
    }
    buf[i] = '\0';

    return (i);
}
