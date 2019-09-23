#include "buffer.h"
#include "common.h"

const char *CRLF = "\r\n";

struct buffer *buffer_new() {
    struct buffer *buffer1 = malloc(sizeof(struct buffer));
    if (!buffer1)
        return NULL;

    buffer1->data = malloc(INIT_BUFFER_SIZE);
    buffer1->total_size = INIT_BUFFER_SIZE;
    buffer1->readIndex = 0;
    buffer1->writeIndex = 0;
    return buffer1;
}

void buffer_free(struct buffer *buffer1) {
    free(buffer1->data);
    free(buffer1);
}

int buffer_writeable_size(struct buffer *buffer) {
    return buffer->total_size - buffer->writeIndex;
}

int buffer_readable_size(struct buffer *buffer) {
    return buffer->writeIndex - buffer->readIndex;
}

int buffer_front_spare_size(struct buffer *buffer) {
    return buffer->readIndex;
}

void make_room(struct buffer *buffer, int size) {
    if (buffer_writeable_size(buffer) >= size) {
        return;
    }
    //如果front_spare和writeable的大小加起来可以容纳数据，则把可读数据往前面拷贝
    if (buffer_front_spare_size(buffer) + buffer_writeable_size(buffer) >= size) {
        int readable = buffer_readable_size(buffer);
        int i;
        for (i = 0; i < readable; i++) {
            memcpy(buffer->data + i, buffer->data + buffer->readIndex + i, 1);
        }
        buffer->readIndex = 0;
        buffer->writeIndex = readable;
    } else {
        //扩大缓冲区
        void *tmp = realloc(buffer->data, buffer->total_size + size);
        if (tmp == NULL) {
            return;
        }
        buffer->data = tmp;
        buffer->total_size += size;
    }
}

int buffer_append(struct buffer *buffer, void *data, int size) {
    if (data != NULL) {
        make_room(buffer, size);
        //拷贝数据到可写空间中
        memcpy(buffer->data + buffer->writeIndex, data, size);
        buffer->writeIndex += size;
    }
}

int buffer_append_char(struct buffer *buffer, char data) {
    make_room(buffer, 1);
    //拷贝数据到可写空间中
    buffer->data[buffer->writeIndex++] = data;
}

int buffer_append_string(struct buffer *buffer, char *data) {
    if (data != NULL) {
        int size = strlen(data);
        buffer_append(buffer, data, size);
    }
}


int buffer_socket_read(struct buffer *buffer, int fd) {
    char additional_buffer[INIT_BUFFER_SIZE];
    struct iovec vec[2];
    int max_writable = buffer_writeable_size(buffer);
    vec[0].iov_base = buffer->data + buffer->writeIndex;
    vec[0].iov_len = max_writable;
    vec[1].iov_base = additional_buffer;
    vec[1].iov_len = sizeof(additional_buffer);
    int result = readv(fd, vec, 2);
    if (result < 0) {
        return -1;
    } else if (result <= max_writable) {
        buffer->writeIndex += result;
    } else {
        buffer->writeIndex = buffer->total_size;
        buffer_append(buffer, additional_buffer, result - max_writable);
    }
    return result;
}

char buffer_read_char(struct buffer *buffer) {
    char c = buffer->data[buffer->readIndex];
    buffer->readIndex++;
    return c;
}

char *buffer_find_CRLF(struct buffer *buffer) {
    char *crlf = memmem(buffer->data + buffer->readIndex, buffer_readable_size(buffer), CRLF, 2);
    return crlf;
}