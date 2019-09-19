#include "tcp_connection.h"
#include "utils.h"


int handle_connection_closed(struct tcp_connection *tcpConnection) {
    struct event_loop *eventLoop = tcpConnection->eventLoop;
    struct channel *channel = tcpConnection->channel;
    event_loop_remove_channel_event(eventLoop, channel->fd, channel);
    if (tcpConnection->connectionClosedCallBack != NULL) {
        tcpConnection->connectionClosedCallBack(tcpConnection);
    }
}

int handle_read(void *data) {
    struct tcp_connection *tcpConnection = (struct tcp_connection *) data;
    struct buffer *input_buffer = tcpConnection->input_buffer;
    struct channel *channel = tcpConnection->channel;

    if (buffer_socket_read(input_buffer, channel->fd) > 0) {
        //应用程序真正读取Buffer里的数据
        if (tcpConnection->messageCallBack != NULL) {
            tcpConnection->messageCallBack(input_buffer, tcpConnection);
        }
    } else {
        handle_connection_closed(tcpConnection);
    }
}

//发送缓冲区可以往外写
//把channel对应的output_buffer不断往外发送
int handle_write(void *data) {
    struct tcp_connection *tcpConnection = (struct tcp_connection *) data;
    struct event_loop *eventLoop = tcpConnection->eventLoop;
    assertInSameThread(eventLoop);

    struct buffer *output_buffer = tcpConnection->output_buffer;
    struct channel *channel = tcpConnection->channel;

    ssize_t nwrited = write(channel->fd, output_buffer->data + output_buffer->readIndex,
                            buffer_readable_size(output_buffer));
    if (nwrited > 0) {
        //已读nwrited字节
        output_buffer->readIndex += nwrited;
        //如果数据完全发送出去，就不需要继续了
        if (buffer_readable_size(output_buffer) == 0) {
            channel_write_event_disable(channel);
        }
        //回调writeCompletedCallBack
        if (tcpConnection->writeCompletedCallBack != NULL) {
            tcpConnection->writeCompletedCallBack(tcpConnection);
        }
    } else {
        yolanda_msgx("handle_write for tcp connection %s", tcpConnection->name);
    }

}

struct tcp_connection *
tcp_connection_new(int connected_fd, struct event_loop *eventLoop,
                   connection_completed_call_back connectionCompletedCallBack,
                   connection_closed_call_back connectionClosedCallBack,
                   message_call_back messageCallBack, write_completed_call_back writeCompletedCallBack) {
    struct tcp_connection *tcpConnection = malloc(sizeof(struct tcp_connection));
    tcpConnection->writeCompletedCallBack = writeCompletedCallBack;
    tcpConnection->messageCallBack = messageCallBack;
    tcpConnection->connectionCompletedCallBack = connectionCompletedCallBack;
    tcpConnection->connectionClosedCallBack = connectionClosedCallBack;
    tcpConnection->eventLoop = eventLoop;
    tcpConnection->input_buffer = buffer_new();
    tcpConnection->output_buffer = buffer_new();


    char *buf = malloc(16);
    sprintf(buf, "connection-%d\0", connected_fd);
    tcpConnection->name = buf;

    // add event read for the new connection
    struct channel *channel1 = channel_new(connected_fd, EVENT_READ, handle_read, handle_write, tcpConnection);
    tcpConnection->channel = channel1;

    //connectionCompletedCallBack callback
    if (tcpConnection->connectionCompletedCallBack != NULL) {
        tcpConnection->connectionCompletedCallBack(tcpConnection);
    }

    event_loop_add_channel_event(tcpConnection->eventLoop, connected_fd, tcpConnection->channel);
    return tcpConnection;
}

//应用层调用入口
int tcp_connection_send_data(struct tcp_connection *tcpConnection, void *data, int size) {
    size_t nwrited = 0;
    size_t nleft = size;
    int fault = 0;

    struct channel *channel = tcpConnection->channel;
    struct buffer *output_buffer = tcpConnection->output_buffer;

    //先往套接字尝试发送数据
    if (!channel_write_event_is_enabled(channel) && buffer_readable_size(output_buffer) == 0) {
        nwrited = write(channel->fd, data, size);
        if (nwrited >= 0) {
            nleft = nleft - nwrited;
        } else {
            nwrited = 0;
            if (errno != EWOULDBLOCK) {
                if (errno == EPIPE || errno == ECONNRESET) {
                    fault = 1;
                }
            }
        }
    }

    if (!fault && nleft > 0) {
        //拷贝到Buffer中，Buffer的数据由框架接管
        buffer_append(output_buffer, data + nwrited, nleft);
        if (!channel_write_event_is_enabled(channel)) {
            channel_write_event_enable(channel);
        }
    }

    return nwrited;
}

int tcp_connection_send_buffer(struct tcp_connection *tcpConnection, struct buffer *buffer) {
    int size = buffer_readable_size(buffer);
    int result = tcp_connection_send_data(tcpConnection, buffer->data + buffer->readIndex, size);
    buffer->readIndex += size;
    return result;
}

void tcp_connection_shutdown(struct tcp_connection *tcpConnection) {
    if (shutdown(tcpConnection->channel->fd, SHUT_WR) < 0) {
        yolanda_msgx("tcp_connection_shutdown failed, socket == %d", tcpConnection->channel->fd);
    }
}