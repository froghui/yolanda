#include "channel.h"


struct channel *
channel_new(int fd, int events, event_read_callback eventReadCallback, event_write_callback eventWriteCallback,
            void *data) {
    struct channel *chan = malloc(sizeof(struct channel));
    chan->fd = fd;
    chan->events = events;
    chan->eventReadCallback = eventReadCallback;
    chan->eventWriteCallback = eventWriteCallback;
    chan->data = data;
    return chan;
}

int channel_write_event_is_enabled(struct channel *channel) {
    return channel->events & EVENT_WRITE;
}

int channel_write_event_enable(struct channel *channel) {
    struct event_loop *eventLoop = (struct event_loop *) channel->data;
    channel->events = channel->events | EVENT_WRITE;
    event_loop_update_channel_event(eventLoop, channel->fd, channel);
}

int channel_write_event_disable(struct channel *channel) {
    struct event_loop *eventLoop = (struct event_loop *) channel->data;
    channel->events = channel->events & ~EVENT_WRITE;
    event_loop_update_channel_event(eventLoop, channel->fd, channel);
}