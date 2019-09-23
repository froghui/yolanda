#ifndef BUFFER_H
#define BUFFER_H

#define INIT_BUFFER_SIZE 65536
//数据缓冲区
struct buffer {
    char *data;          //实际缓冲
    int readIndex;       //缓冲读取位置
    int writeIndex;      //缓冲写入位置
    int total_size;      //总大小
};


struct buffer *buffer_new();

void buffer_free(struct buffer *buffer);

int buffer_writeable_size(struct buffer *buffer);

int buffer_readable_size(struct buffer *buffer);

int buffer_front_spare_size(struct buffer *buffer);

//往buffer里写数据
int buffer_append(struct buffer *buffer, void *data, int size);

//往buffer里写数据
int buffer_append_char(struct buffer *buffer, char data);

//往buffer里写数据
int buffer_append_string(struct buffer*buffer, char * data);

//读socket数据，往buffer里写
int buffer_socket_read(struct buffer *buffer, int fd);

//读buffer数据
char buffer_read_char(struct buffer *buffer);

//查询buffer数据
char * buffer_find_CRLF(struct buffer * buffer);

#endif