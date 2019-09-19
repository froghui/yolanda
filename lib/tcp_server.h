#ifndef TCP_SERVER_H
#define TCP_SERVER_H

typedef int (*connection_completed_call_back)(struct tcp_connection *tcpConnection);

typedef int (*message_call_back)(struct buffer *buffer, struct tcp_connection *tcpConnection);

typedef int (*write_completed_call_back)(struct tcp_connection *tcpConnection);

typedef int (*connection_closed_call_back)(struct tcp_connection *tcpConnection);

#include "acceptor.h"
#include "event_loop.h"
#include "thread_pool.h"
#include "buffer.h"
#include "tcp_connection.h"

struct TCPserver {
    int port;
    struct event_loop *eventLoop;
    struct acceptor *acceptor;
    connection_completed_call_back connectionCompletedCallBack;
    message_call_back messageCallBack;
    write_completed_call_back writeCompletedCallBack;
    connection_closed_call_back connectionClosedCallBack;
    int threadNum;
    struct thread_pool *threadPool;
    void * data; //for callback use: http_server
};


//准备监听套接字
struct TCPserver *
tcp_server_init(struct event_loop *eventLoop, struct acceptor *acceptor,
                connection_completed_call_back connectionCallBack,
                message_call_back messageCallBack,
                write_completed_call_back writeCompletedCallBack,
                connection_closed_call_back connectionClosedCallBack,
                int threadNum);

//开启监听
void tcp_server_start(struct TCPserver *tcpServer);

//设置callback数据
void tcp_server_set_data(struct TCPserver *tcpServer, void * data);

#endif
