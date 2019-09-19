#include "event_dispatcher.h"
#include "event_loop.h"
#include "log.h"

#define INIT_POLL_SIZE 1024

struct pollidx {
    int idxplus1;
};

struct poll_dispatcher_data {
    int event_count;
    int nfds;
    int realloc_copy;
    struct pollfd *event_set;
    struct pollfd *event_set_copy;
};

static void *poll_init(struct event_loop *);

static int poll_add(struct event_loop *, struct channel *channel1);

static int poll_del(struct event_loop *, struct channel *channel1);

static int poll_update(struct event_loop *, struct channel *channel1);

static int poll_dispatch(struct event_loop *, struct timeval *);

static void poll_clear(struct event_loop *);

const struct event_dispatcher poll_dispatcher = {
        "poll",
        poll_init,
        poll_add,
        poll_del,
        poll_update,
        poll_dispatch,
        poll_clear,
};

void *poll_init(struct event_loop *eventLoop) {
    struct poll_dispatcher_data *pollDispatcherData = malloc(sizeof(struct poll_dispatcher_data));

    //初始化pollfd数组，这个数组的第一个元素是listen_fd，其余的用来记录将要连接的connect_fd
    pollDispatcherData->event_set = malloc(sizeof(struct pollfd) * INIT_POLL_SIZE);
    // 用-1表示这个数组位置还没有被占用
    int i;
    for (i = 0; i < INIT_POLL_SIZE; i++)
        pollDispatcherData->event_set[i].fd = -1;
    pollDispatcherData->event_count = 0;
    pollDispatcherData->nfds = 0;
    pollDispatcherData->realloc_copy = 0;

    return pollDispatcherData;
}


int poll_add(struct event_loop *eventLoop, struct channel *channel1) {

    struct poll_dispatcher_data *pollDispatcherData = (struct poll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;

    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | POLLRDNORM;
    }

    if (channel1->events & EVENT_WRITE) {
        events = events | POLLWRNORM;
    }

    //找到一个可以记录该连接套接字的位置
    int i = 0;
    for (i = 0; i < INIT_POLL_SIZE; i++) {
        if (pollDispatcherData->event_set[i].fd < 0) {
            pollDispatcherData->event_set[i].fd = fd;
            pollDispatcherData->event_set[i].events = events;
            break;
        }
    }

    yolanda_msgx("poll added channel fd==%d, %s", fd, eventLoop->thread_name);
    if (i > INIT_POLL_SIZE) {
        LOG_ERR("too many clients, just abort it");
    }

    return 0;
}

int poll_del(struct event_loop *eventLoop, struct channel *channel1) {
    struct poll_dispatcher_data *pollDispatcherData = (struct poll_dispatcher_data *) eventLoop->event_dispatcher_data;
    int fd = channel1->fd;

    //找到fd对应的记录
    int i = 0;
    for (i = 0; i < INIT_POLL_SIZE; i++) {
        if (pollDispatcherData->event_set[i].fd == fd) {
            pollDispatcherData->event_set[i].fd = -1;
            break;
        }
    }

    yolanda_msgx("poll delete channel fd==%d, %s", fd, eventLoop->thread_name);
    if (i > INIT_POLL_SIZE) {
        LOG_ERR("can not find fd, poll delete error");
    }

    return 0;
}

int poll_update(struct event_loop *eventLoop, struct channel *channel1) {
    struct poll_dispatcher_data *pollDispatcherData = (struct poll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;

    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | POLLRDNORM;
    }

    if (channel1->events & EVENT_WRITE) {
        events = events | POLLWRNORM;
    }

    //找到fd对应的记录
    int i = 0;
    for (i = 0; i < INIT_POLL_SIZE; i++) {
        if (pollDispatcherData->event_set[i].fd == fd) {
            pollDispatcherData->event_set[i].events = events;
            break;
        }
    }

    yolanda_msgx("poll updated channel fd==%d, %s", fd, eventLoop->thread_name);
    if (i > INIT_POLL_SIZE) {
        LOG_ERR("can not find fd, poll updated error");
    }

    return 0;
}

int poll_dispatch(struct event_loop *eventLoop, struct timeval *timeval) {
    struct poll_dispatcher_data *pollDispatcherData =
            (struct poll_dispatcher_data *) eventLoop->event_dispatcher_data;


    int ready_number = 0;
    int timewait = timeval->tv_sec * 1000;
    if ((ready_number = poll(pollDispatcherData->event_set, INIT_POLL_SIZE, timewait)) < 0) {
        error(1, errno, "poll failed ");
    }

    if (ready_number <= 0) {
        return 0;
    }

    int i;
    for (i = 0; i <= INIT_POLL_SIZE; i++) {
        int socket_fd;
        struct pollfd pollfd = pollDispatcherData->event_set[i];
        if ((socket_fd = pollfd.fd) < 0)
            continue;

        //有事件发生
        if (pollfd.revents > 0) {
            yolanda_msgx("get message channel i==%d, fd==%d, %s", i, socket_fd, eventLoop->thread_name);

            if (pollfd.revents & POLLRDNORM) {
                channel_event_activate(eventLoop, socket_fd, EVENT_READ);
            }

            if (pollfd.revents & POLLWRNORM) {
                channel_event_activate(eventLoop, socket_fd, EVENT_WRITE);
            }

            if (--ready_number <= 0)
                break;
        }

//        if (pollfd.revents & (POLLRDNORM | POLLERR)) {
//            if ((n = read(socket_fd, buf, MAXLINE)) < 0) {
//                if (errno == ECONNRESET) {
//                    close(socket_fd);
//                    pollfd.fd = -1;
//                } else
//                    error(1, errno, "read error");
//            } else if (n == 0) {
//                close(socket_fd);
//                pollfd.fd = -1;
//            } else {
//                if (write(socket_fd, buf, n) < 0) {
//                    error(1, errno, "write error");
//                }
//            }

    }

    return 0;
}

void poll_clear(struct event_loop *eventLoop) {
    struct poll_dispatcher_data *pollDispatcherData =
            (struct poll_dispatcher_data *) eventLoop->event_dispatcher_data;

    free(pollDispatcherData->event_set);
    pollDispatcherData->event_set = NULL;
    free(pollDispatcherData);
    eventLoop->event_dispatcher_data = NULL;

    return;
}