#ifndef UTILS_H
#define UTILS_H

#include "event_loop.h"

void assertInSameThread(struct event_loop *eventLoop);

//1： same thread: 0： not the same thread
int isInSameThread(struct event_loop *eventLoop);
#endif